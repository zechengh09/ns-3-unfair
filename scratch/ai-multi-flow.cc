//
// Network topology:
//
//       n0 ------------ (n1/router) -------------- n2
//            10.1.1.x                192.168.1.x
//       10.1.1.1    10.1.1.2   192.16.1.1     192.168.1.2
//
// - Flow from n0 to n2 using BulkSendApplication.
// - Tracing of queues and packet receptions to file "*.tr" and "*.pcap" when
//   tracing is turned on.
//

// System includes.
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

// ns-3 includes.
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/config-store-module.h"
#include "ns3/config.h"

#include <torch/script.h>

#include <regex>

using namespace ns3;

// Constants.
#define ENABLE_TRACE     false     // Set to "true" to enable trace
#define START_TIME       0.0       // Seconds
#define S_PORT           911       // Well-known port for server
#define PACKET_SIZE      1380      // Bytes; Assumes 60 bytes for the IP header
                                   // (20 bytes + up to 40 bytes for options)
                                   // and a maximum of 60 bytes for the TCP
                                   // header (20 bytes + up to 40 bytes for
                                   // options).
#define MTU              1500      // Bytes
#define PCAP_LEN         200       // Bytes

// Uncomment one of the below.
#define TCP_PROTOCOL     "ns3::TcpBbr"
// #define TCP_PROTOCOL     "ns3::TcpNewReno"
// #define TCP_PROTOCOL     "ns3::TcpCubic"

// For logging.
NS_LOG_COMPONENT_DEFINE ("main");

const int BBR_PRINT_PERIOD = 2; // sec
const bool USE_ATTAINED_TPUT = true;
/////////////////////////////////////////////////

extern std::unordered_set<ns3::TcpSocketBase*> tcpSocketBases;
extern bool useReno;

/////////////////////////////////////////////////

// Arguments read by functions
double durS = 20;
uint32_t recalcUS = 1<<30;
double targetTputMbps = 0;

/////////////////////////////////////////////////

Ptr<PacketSink> createNewFlow(uint16_t port, Ipv4InterfaceContainer i1i2, NodeContainer nodes) {

  BulkSendHelper source1("ns3::TcpSocketFactory",
                         InetSocketAddress(i1i2.GetAddress(1), port));
  // Set the amount of data to send in bytes (0 for unlimited).
  source1.SetAttribute("MaxBytes", UintegerValue(0));
  source1.SetAttribute("SendSize", UintegerValue(PACKET_SIZE));
  ApplicationContainer apps1 = source1.Install(nodes.Get(0));
  apps1.Start(Seconds(START_TIME));
  apps1.Stop(Seconds(START_TIME+durS));

  double random_start = 0.0;
//  random_start = (rand() % 50) / 10.0;

  // Sink (at node 2).
  PacketSinkHelper sink1("ns3::TcpSocketFactory",
                         InetSocketAddress(Ipv4Address::GetAny(), port));
  apps1 = sink1.Install(nodes.Get(2));
  apps1.Start(Seconds(START_TIME + random_start));
  apps1.Stop(Seconds(START_TIME+durS));
  return DynamicCast<PacketSink> (apps1.Get(0)); // 4 stats
}



/////////////////////////////////////////////////

// const double f_min_in = 1836972.2172854163;
// const double f_max_in = 31411450.90160741;
// const double o_min_in = 0.00016666666666666666;
// const double o_max_in = 0.002857142857142857;

double f_min_in = 0;
double f_max_in = 0;
double o_min_in = 0;
double o_max_in = 0;

void read_scale_params(const std::string& filename) {
    std::ifstream file(filename);
    std::string str((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    std::regex re("(.+),(.+),(.+),(.+)");
    std::smatch match;
    auto good = std::regex_search(str, match, re);
    assert(good);
    assert(match.size() == 5);
    f_min_in = stod(match[1]);
    f_max_in = stod(match[2]);
    o_min_in = stod(match[3]);
    o_max_in = stod(match[4]);
}

double do_scale(double x, double x_min_in, double x_max_in, double x_min_out, double x_max_out) {
    return x_min_out +
        (x - x_min_in) * (x_max_out - x_min_out) / (x_max_in - x_min_in);
}

double scale(double x) {
    return do_scale(x, f_min_in, f_max_in, 0, 1);
}

double unscale(double x) {
    return do_scale(x,  0, 1, o_min_in, o_max_in);
}

/////////////////////////////////////////////////
std::vector<Ptr<PacketSink>> sinks;

void updateAckPeriod (torch::jit::script::Module* net) {
  if (USE_ATTAINED_TPUT) {
      double total_tput = 0;
      for (auto& sink : sinks) {
          total_tput += sink->getBbrStats().tputMbps;
      }
      targetTputMbps = total_tput / sinks.size();
  }

  std::vector<torch::jit::IValue> input{};
  // The net takes (current throughput (Gbps), target throughput (Gbps), RTT (ss))
  // dummy.push_back(torch::tensor({stats.tputMbps/1e3, targetTputMbps/1e3, stats.avgLat.GetSeconds()*2.}));


  auto target = scale(targetTputMbps*1e6);
  input.push_back(torch::tensor({target}));

  auto periodTensor = net->forward(input).toTensor();

  // periodTensor.print();
  // Returns ack period (s)
  auto output = periodTensor.item().to<double>();

  printf("Raw output: %f, unscaled: %f\n", output, unscale(output));
  auto newPeriod = MicroSeconds (1./unscale(output));

  for (auto& sink : sinks) {
      // if (!sink->recvBbr()) { continue; }
      assert(sink->getSockets().size() == 1);
      auto sock = DynamicCast<ns3::TcpSocketBase>(sink->getSockets().front());
      assert(sock);
      auto stats = sink->getBbrStats();
      if (stats.tputMbps < (1.2 * targetTputMbps)) {
          sock->SetAckPeriod (Seconds (0));
          continue;
      }
    
      printf("Stats: tput (Gb/s): %f, RTT (s) %f; newPeriod (s): %.20f, target: %f Mbps\n", stats.tputMbps/1e3, stats.avgLat.GetSeconds()*2., newPeriod.GetSeconds(), targetTputMbps);
      sock->SetAckPeriod (newPeriod);
  }
  Simulator::Schedule( MicroSeconds(recalcUS), updateAckPeriod, net);
}



void printBbrStats() {
  for (auto& sink : sinks) {
      auto stats = sink->getBbrStats();
      std::cout << "BBR (" << Simulator::Now() << "): Throughput: " << stats.tputMbps << " Mb/s, Avg. Latency: " << stats.avgLat << "\n";

      printf("pendingAcks: ");
      assert(sink->getSockets().size() == 1);
      auto sock = DynamicCast<ns3::TcpSocketBase>(sink->getSockets().front());
      if (!sock->recvBbr()) { continue; }
      printf("%zu; ", sock->getNumPendingAcks());
      printf("\n");
  }
  Simulator::Schedule( Seconds(BBR_PRINT_PERIOD), printBbrStats);
}

int main (int argc, char *argv[])
{
  // Parse command line arguments.
  double bwMbps = 10;
  double delUs = 5000;
  uint32_t que = 100;
  double warmup_s = 5;
  bool pcap = false;
  std::string mdl_flp = "";
  std::string out_dir = ".";
  std::string scale_params_file = "";
  uint32_t num_other_flows = 0;

  CommandLine cmd;
  cmd.AddValue ("bandwidth_Mbps", "Bandwidth for both links (Mbps).", bwMbps);
  cmd.AddValue ("delay_us", "Link delay (us). RTT is 4x this value.", delUs);
  cmd.AddValue ("queue_capacity_p", "Router queue size (packets).", que);
  cmd.AddValue ("experiment_duration_s", "Simulation duration (s).", durS);
  cmd.AddValue ("recalc_us", "Between recalculating ack delay (us)", recalcUS);
  cmd.AddValue ("warmup_s", "Time before delaying acs (s)", warmup_s);
  cmd.AddValue ("pcap", "Record a pcap trace from each port (true or false).", pcap);
  cmd.AddValue ("model", "Path to the model file.", mdl_flp);
  cmd.AddValue ("out_dir", "Directory in which to store output files.", out_dir);
  cmd.AddValue ("scale_params_file", "CSV File with scaling parameters", scale_params_file);
  cmd.AddValue ("num_other_flows", "Number of non BBR senders", num_other_flows);
  cmd.AddValue ("use_reno", "Use reno (else cubic)", useReno);
  cmd.Parse (argc, argv);

  read_scale_params(scale_params_file);

  torch::jit::script::Module net;
  try {
      net = torch::jit::load(mdl_flp);
  } catch (const c10::Error& e) {
      // fprintf(stderr, "Error loading model %s\n", mdl_flp.c_str());
      fprintf(stderr, "Error loading model %s: %s\n", mdl_flp.c_str(), e.what());
      return 1;
  }


  Config::SetDefault ("ns3::PacketSink::maxBbrRecords", UintegerValue (1000));

  targetTputMbps = bwMbps / (num_other_flows + 1);

  uint32_t rttUs = delUs * 4;
  std::stringstream bw_ss;
  bw_ss << bwMbps << "Mbps";
  std::string bw = bw_ss.str ();
  std::stringstream del_ss;
  del_ss << delUs << "us";
  std::string del = del_ss.str ();


  // auto routerToDeviceBW = bwMbps*0.9;
  auto routerToDeviceBW = bwMbps;
  std::stringstream sndSS;
  sndSS << routerToDeviceBW << "Mbps";
  auto routerToDeviceBWStr = sndSS.str();

  /////////////////////////////////////////
  // Turn on logging and report parameters.
  // Note: For BBR', other components that may be of interest include "TcpBbr"
  //       and "BbrState".
  LogComponentEnable ("main", LOG_LEVEL_INFO);

  std::cout << "\n";
  std::cout << "TCP protocol: " << TCP_PROTOCOL << "\n";
  std::cout << "Server to Router Bandwidth (Mbps): " << bwMbps << "\n";
  std::cout << "Server to Router Delay (us): " << delUs << "\n";
  std::cout << "Router to Client Bandwidth (Mbps): " << bwMbps << "\n";
  std::cout << "Router to Client Delay (us): " << delUs << "\n";
  std::cout << "RTT (us): " << rttUs << "\n";
  std::cout << "Packet size (bytes): " << PACKET_SIZE << "\n";
  std::cout << "Router queue size (packets): "<< que << "\n";
  std::cout << "Warmup (s): " << warmup_s << "\n";
  std::cout << "Duration (s): " << durS << "\n";
  std::cout << "Target Throughput (Mbps): " << targetTputMbps << "\n";
  std::cout << "\n";

  /////////////////////////////////////////
  // Configure parameters.
  ConfigStore config;
  config.ConfigureDefaults ();
  // Select which TCP variant to use.
  Config::SetDefault ("ns3::TcpL4Protocol::SocketType",
                      StringValue (TCP_PROTOCOL));
  // Set the segment size (otherwise, ns-3's default is 536).
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (PACKET_SIZE));

  // Turn off delayed ack (so that every packet results in an ACK).
  // Note: BBR' still works without this.
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (0));
  // Increase the capacity of the send and receive buffers to make sure that the
  // experiment is not application-limited.
  // Config::SetDefault ("ns3::TcpSocket::SndBufSize", UintegerValue (262144));
  Config::SetDefault ("ns3::TcpSocket::SndBufSize", UintegerValue (1'000'000u));
  Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue (1'000'000u));
  // updateAckPeriod (MicroSeconds(0));
  Config::SetDefault ("ns3::TcpSocketBase::AckPeriod", TimeValue (MicroSeconds (0)));

  /////////////////////////////////////////
  // Create nodes.
  NS_LOG_INFO ("Creating nodes.");
  NodeContainer nodes;  // 0: source, 1: router, 2: sink
  nodes.Create (3);

  /////////////////////////////////////////
  // Create channels.
  NS_LOG_INFO ("Creating channels.");
  NodeContainer n0_to_r = NodeContainer (nodes.Get (0), nodes.Get (1));
  NodeContainer r_to_n1 = NodeContainer (nodes.Get (1), nodes.Get (2));

  /////////////////////////////////////////
  // Create links.
  NS_LOG_INFO ("Creating links.");

  // Server to Router.
  PointToPointHelper p2p(PCAP_LEN);
  // p2p.SetDeviceAttribute ("DataRate", StringValue (bw));
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Gbps"));
  p2p.SetChannelAttribute ("Delay", StringValue (del));
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (MTU));
  NetDeviceContainer devices1 = p2p.Install (n0_to_r);

  // Router to Client.
  p2p.SetDeviceAttribute ("DataRate", StringValue (routerToDeviceBWStr));
  // p2p.SetDeviceAttribute ("DataRate", StringValue (bw));
  p2p.SetChannelAttribute ("Delay", StringValue (del));
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (MTU));
  p2p.SetQueue ("ns3::DropTailQueue",
                "Mode", StringValue ("QUEUE_MODE_PACKETS"),
                "MaxPackets", UintegerValue (que));
  NetDeviceContainer devices2 = p2p.Install (r_to_n1);

  /////////////////////////////////////////
  // Install Internet stack.
  NS_LOG_INFO ("Installing Internet stack.");
  InternetStackHelper internet;
  internet.Install (nodes);

  /////////////////////////////////////////
  // Add IP addresses.
  NS_LOG_INFO ("Assigning IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i1 = ipv4.Assign (devices1);

  ipv4.SetBase ("191.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign (devices2);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  /////////////////////////////////////////
  // Create apps.
  NS_LOG_INFO ("Creating applications.");
  NS_LOG_INFO ("  Bulk send.");

  // Source (at node 0).
  BulkSendHelper source ("ns3::TcpSocketFactory",
                         InetSocketAddress (i1i2.GetAddress (1), S_PORT));
  // Set the amount of data to send in bytes (0 for unlimited).
  source.SetAttribute ("MaxBytes", UintegerValue (0));
  source.SetAttribute ("SendSize", UintegerValue (PACKET_SIZE));
  ApplicationContainer apps = source.Install (nodes.Get (0));
  apps.Start (Seconds (START_TIME));
  apps.Stop (Seconds (durS));

  // Sink (at node 2).
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), S_PORT));
  apps = sink.Install (nodes.Get (2));
  apps.Start (Seconds (START_TIME));
  apps.Stop (Seconds (durS));
  Ptr<PacketSink> p_sink = DynamicCast<PacketSink> (apps.Get (0)); // 4 stats
  sinks.push_back(p_sink);

  // Configure TcpSocketBase with the model filepath.
  Config::SetDefault ("ns3::TcpSocketBase::Model", StringValue (mdl_flp));

  uint32_t port = 101;
  for (uint32_t i = 0; i < num_other_flows; ++i) {
      Ptr<PacketSink> sink = createNewFlow(port + i, i1i2, nodes);
      sinks.push_back(sink);
  }

  /////////////////////////////////////////
  // Setup tracing (as appropriate).
  std::stringstream details_ss;
  // details_ss << ackPeriod << "us-" << delayToAckDelay << "s-" << bw << "-"
  //            << rttUs << "us-" << durS << "s";
  std::string details = details_ss.str ();
  if (ENABLE_TRACE) {
    NS_LOG_INFO ("Enabling trace files.");
    AsciiTraceHelper ath;
    std::stringstream trace_name;
    trace_name << out_dir << "/trace-" << details << ".tr";
    p2p.EnableAsciiAll (ath.CreateFileStream ("trace.tr"));
  }
  if (pcap) {
    NS_LOG_INFO ("Enabling pcap files.");
    std::stringstream pcap_name;
    pcap_name << out_dir << "/" << details;
    p2p.EnablePcapAll (pcap_name.str (), true);
  }

  Simulator::Schedule( Seconds (warmup_s), updateAckPeriod, &net);

  Simulator::Schedule (Seconds (BBR_PRINT_PERIOD), printBbrStats);

  /////////////////////////////////////////
  // Run simulation.
  std::chrono::time_point<std::chrono::steady_clock> start_time =
    std::chrono::steady_clock::now ();

  NS_LOG_INFO ("Running simulation.");
  Simulator::Stop (Seconds (durS));
  NS_LOG_INFO ("Simulation time: [" << START_TIME << "," << durS << "]");
  NS_LOG_INFO ("---------------- Start -----------------------");
  config.ConfigureAttributes ();
  Simulator::Run ();
  NS_LOG_INFO ("---------------- Stop ------------------------");

  std::chrono::time_point<std::chrono::steady_clock> stop_time =
    std::chrono::steady_clock::now ();
  std::chrono::duration<double> diff = stop_time - start_time;
  NS_LOG_INFO ("Real simulation time (s): " << diff.count ());

  /////////////////////////////////////////
  double total_throughput = 0;
  double sum_sq = 0;
  /*
  double tput = p_sink->GetTotalRx() * 8 / durS / 1000000.0;
  total_throughput += tput;
  sum_sq += pow(tput, 2);
  std::cout << "BBR Throughput: " << tput << " Mb/s\n";
  for (auto& other : others) {
      auto otherTput = other->GetTotalRx() * 8 / durS / 1000000.0;
      total_throughput += otherTput;
      sum_sq += pow(otherTput, 2);
      std::cout << "Other Throughput: " << otherTput << " Mb/s\n";
  }
  */
  for (auto& sink : sinks) {
      auto tput = sink->GetTotalRx() * 8 / durS / 1000000.0;
      total_throughput += tput;
      sum_sq += pow(tput, 2);
      std::cout << (sink->recvBbr() ? "Bbr" : "Other") << " Throughput: " << tput << " Mb/s\n";
  }

  std::cout << "Fairness: " << pow(total_throughput, 2) / (sinks.size() * sum_sq) << "\n";

  // Output stats.
  /*
  NS_LOG_INFO ("Total bytes received: " << p_sink->GetTotalRx ());
  double tput = p_sink->GetTotalRx () / (durS - START_TIME);
  tput *= 8;          // Convert to bits.
  tput /= 1000000.0;  // Convert to Mb/s
  NS_LOG_INFO ("Throughput: " << tput << " Mb/s");
  NS_LOG_INFO ("Done.");
  */

  // Done.
  Simulator::Destroy ();
  return 0;
}

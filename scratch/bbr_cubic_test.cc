//
// Network topology
//
//       n0 ------------ (n1/router) -------------- n2
//            10.1.1.x                192.168.1.x
//       10.1.1.1    10.1.1.2   192.16.1.1     192.168.1.2
//
// - Flow from n0 to n2 using BulkSendApplication.
//
// - Tracing of queues and packet receptions to file "*.tr" and
//   "*.pcap" when tracing is turned on.
//

// System includes.
#include <string>
#include <fstream>

// NS3 includes.
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

// Constants.
#define ENABLE_PCAP      false     // Set to "true" to enable pcap
#define ENABLE_TRACE     false     // Set to "true" to enable trace
#define BIG_QUEUE        2000      // Packets
#define QUEUE_SIZE       1000      // Packets
#define START_TIME       0.0       // Seconds
#define STOP_TIME        240.0       // Seconds
#define S_TO_R_BW        "100Mbps" // Server to router
#define S_TO_R_DELAY     "10ms"
#define R_TO_C_BW        "10Mbps"  // Router to client (bttlneck)
#define R_TO_C_DELAY     "10ms"
#define PACKET_SIZE      1000      // Bytes.
#define NUM_CUBIC_FLOW   16

// Uncomment one of the below.
#define TCP_PROTOCOL     "ns3::TcpBbr"

Ptr<PacketSink> createNewFlow(uint16_t port, Ipv4InterfaceContainer i1i2, NodeContainer nodes) {

  BulkSendHelper source1("ns3::TcpSocketFactory",
                         InetSocketAddress(i1i2.GetAddress(1), port));
  // Set the amount of data to send in bytes (0 for unlimited).
  source1.SetAttribute("MaxBytes", UintegerValue(0));
  source1.SetAttribute("SendSize", UintegerValue(PACKET_SIZE));
  ApplicationContainer apps1 = source1.Install(nodes.Get(0));
  apps1.Start(Seconds(START_TIME));
  apps1.Stop(Seconds(STOP_TIME));

  double random_start = 0.0;

  // Sink (at node 2).
  PacketSinkHelper sink1("ns3::TcpSocketFactory",
                         InetSocketAddress(Ipv4Address::GetAny(), port));
  apps1 = sink1.Install(nodes.Get(2));
  apps1.Start(Seconds(START_TIME + random_start));
  apps1.Stop(Seconds(STOP_TIME));
  return DynamicCast<PacketSink> (apps1.Get(0)); // 4 stats
}

NS_LOG_COMPONENT_DEFINE ("main");
int main (int argc, char *argv[]) {

  uint32_t nPackets = QUEUE_SIZE;

  CommandLine cmd;
  cmd.AddValue("nPackets", "Queue length in Number of packets", nPackets);
  cmd.Parse (argc, argv);

  LogComponentEnable("main", LOG_LEVEL_INFO);

  /////////////////////////////////////////
  // Setup environment
  Config::SetDefault("ns3::TcpL4Protocol::SocketType",
                     StringValue(TCP_PROTOCOL));

  Config::SetDefault ("ns3::TcpSocket::SndBufSize", UintegerValue (262144 * 2));
  Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue (262144 * 2));

  // Set segment size (otherwise, ns-3 default is 536).
  Config::SetDefault("ns3::TcpSocket::SegmentSize",
                     UintegerValue(PACKET_SIZE));

  // Turn off delayed ack (so, acks every packet).
  // Note, BBR' still works without this.
  Config::SetDefault("ns3::TcpSocket::DelAckCount", UintegerValue(0));

  /////////////////////////////////////////
  // Create nodes.
  NodeContainer nodes;  // 0=source, 1=router, 2=sink
  nodes.Create(3);

  /////////////////////////////////////////
  // Create channels.
  NodeContainer n0_to_r = NodeContainer(nodes.Get(0), nodes.Get(1));
  NodeContainer r_to_n1 = NodeContainer(nodes.Get(1), nodes.Get(2));

  /////////////////////////////////////////
  // Create links.
  // Server to Router.
  int mtu = 1500;
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate", StringValue (S_TO_R_BW));
  p2p.SetChannelAttribute("Delay", StringValue (S_TO_R_DELAY));
  p2p.SetDeviceAttribute ("Mtu", UintegerValue(mtu));
  NetDeviceContainer devices1 = p2p.Install(n0_to_r);

  // Router to Client.
  p2p.SetDeviceAttribute("DataRate", StringValue (R_TO_C_BW));
  p2p.SetChannelAttribute("Delay", StringValue (R_TO_C_DELAY));
  p2p.SetDeviceAttribute ("Mtu", UintegerValue(mtu));
  NS_LOG_INFO("Router queue size: "<< nPackets);

  p2p.SetQueue("ns3::DropTailQueue",
               "MaxSize", QueueSizeValue(QueueSize(std::to_string(nPackets) + "p")));
  NetDeviceContainer devices2 = p2p.Install(r_to_n1);

  /////////////////////////////////////////
  // Install Internet stack.
  InternetStackHelper internet;
  internet.Install(nodes);

  /////////////////////////////////////////
  // Add IP addresses.
  Ipv4AddressHelper ipv4;
  ipv4.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i1 = ipv4.Assign(devices1);

  ipv4.SetBase("191.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign(devices2);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  /////////////////////////////////////////
  // Create apps.

  // Well-known port for server.
  uint16_t port = 100;

  // Source (at node 0).
  BulkSendHelper source("ns3::TcpSocketFactory",
                        InetSocketAddress(i1i2.GetAddress(1), port));
  // Set the amount of data to send in bytes (0 for unlimited).
  source.SetAttribute("MaxBytes", UintegerValue(0));
  source.SetAttribute("SendSize", UintegerValue(PACKET_SIZE));
  ApplicationContainer apps = source.Install(nodes.Get(0));
  apps.Start(Seconds(START_TIME));
  apps.Stop(Seconds(STOP_TIME));

  // Sink (at node 2).
  PacketSinkHelper sink("ns3::TcpSocketFactory",
                        InetSocketAddress(Ipv4Address::GetAny(), port));
  apps = sink.Install(nodes.Get(2));
  apps.Start(Seconds(START_TIME));
  apps.Stop(Seconds(STOP_TIME));
  Ptr<PacketSink> p_sink = DynamicCast<PacketSink> (apps.Get(0)); // 4 stats

  /////////////////////////////////////////
  // Create other 16 flows

  std::list<Ptr<PacketSink>> sink_list;

  for (int i = 0; i < NUM_CUBIC_FLOW; i++) {
    sink_list.push_back(createNewFlow(101 + i, i1i2, nodes));
  }


  /////////////////////////////////////////
  // Setup tracing (as appropriate).
  if (ENABLE_TRACE) {
    NS_LOG_INFO("Enabling trace files.");
    AsciiTraceHelper ath;
    p2p.EnableAsciiAll(ath.CreateFileStream("trace.tr"));
  }
  if (ENABLE_PCAP) {
    NS_LOG_INFO("Enabling pcap files.");
    p2p.EnablePcapAll("shark", true);
  }

  /////////////////////////////////////////
  // Run simulation.
  Simulator::Stop(Seconds(STOP_TIME));
  NS_LOG_INFO("Simulation time: [" <<
                                   START_TIME << "," <<
                                   STOP_TIME << "]");
  Simulator::Run();


  /////////////////////////////////////////
  // Ouput stats.
  double tput = ((p_sink->GetTotalRx() / (1.0 * (STOP_TIME - START_TIME))) * 8 / 1000000.0);
  NS_LOG_INFO("BBR Throughput: " << tput << " Mb/s");

  double cubic_total = 0.0;
  double sum_of_square = 0.0;

  for (auto &it : sink_list) {
    NS_LOG_INFO("Cubic Throughput: " << ((it->GetTotalRx() /
                                         (1.0 * (STOP_TIME - START_TIME))) * 8 / 1000000.0) << " Mb/s");
    cubic_total += (it->GetTotalRx() / (1.0 * (STOP_TIME - START_TIME))) * 8 / 1000000.0;
    sum_of_square += pow(((it->GetTotalRx() / (1.0 * (STOP_TIME - START_TIME))) * 8 / 1000000.0), 2);
  }

  NS_LOG_INFO("Cubic Avg Throughput: " << cubic_total/(1.0 * NUM_CUBIC_FLOW) << " Mb/s");

  double total_throughput = cubic_total + tput;
  sum_of_square += pow(((p_sink->GetTotalRx() / (1.0 * (STOP_TIME - START_TIME))) * 8 / 1000000.0), 2);

  NS_LOG_INFO("Toal Throughput: " << total_throughput << " Mb/s");
  NS_LOG_INFO("Jain's Fairness Index: " << pow(total_throughput,2) / (17.0 * sum_of_square));

  // Done.
  Simulator::Destroy();
  return 0;
}

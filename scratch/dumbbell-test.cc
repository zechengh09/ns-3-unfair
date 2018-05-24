#include <iostream>
#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/applications-module.h"
#include "ns3/packet-sink.h"
#include "ns3/traffic-control-module.h"
#include "ns3/log.h"
#include "ns3/random-variable-stream.h"
#include "ns3/gtk-config-store.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/callback.h"

using namespace ns3;
Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
std::string dir = "results/dumbbell/";

void
CheckQueueSize (Ptr<QueueDisc> queue)
{
  uint32_t qSize = queue->GetCurrentSize ().GetValue ();

  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.001), &CheckQueueSize, queue);

  std::ofstream fPlotQueue (std::stringstream (dir + "queue-size.plotme").str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << qSize << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeG1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/G1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

/*
static void
CwndChangeG2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/G2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeG3 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/G3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeG4 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/G4.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeG5 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/G5.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
*/

static void
MarkTraceR1 (Ptr<const QueueDiscItem> item, const char* reason)
{
  static uint32_t i = 0;
  std::ofstream fPlotQueue (dir+"markTraces/R1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << ++i << std::endl;
  fPlotQueue.close ();
}

void
TraceCwnd (uint32_t node, uint32_t cwndWindow,
           Callback <void, uint32_t, uint32_t> CwndTrace)
{
  Config::ConnectWithoutContext ("/NodeList/" + std::to_string (node) + "/$ns3::TcpL4Protocol/SocketList/" + std::to_string (cwndWindow) + "/CongestionWindow", CwndTrace);
}

void InstallPacketSink (Ptr<Node> node, uint16_t port)
{
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps = sink.Install (node);
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (11.0));
}

void InstallBulkSend (Ptr<Node> node, Ipv4Address address, uint16_t port, 
                      uint32_t nodeId, uint32_t cwndWindow,
                      Callback <void, uint32_t, uint32_t> CwndTrace)
{
  BulkSendHelper source ("ns3::TcpSocketFactory", 
                         InetSocketAddress (address, port));

  source.SetAttribute ("MaxBytes", UintegerValue (0));
  ApplicationContainer sourceApps = source.Install (node);
  Time timeToStart = Seconds (uv->GetValue (0, 1));
  sourceApps.Start (timeToStart);
  Simulator::Schedule (timeToStart + Seconds (0.001), &TraceCwnd, nodeId, cwndWindow, CwndTrace);
  sourceApps.Stop (Seconds (10.0));
}

int main (int argc, char *argv[])
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%d-%m-%Y-%I-%M-%S",timeinfo);
  std::string currentTime (buffer);

  uint32_t    nLeftLeaf = 1;
  uint32_t    nRightLeaf = 1;
  uint32_t    nLeaf = 0; // If non-zero, number of both left and right

  CommandLine cmd;
  cmd.AddValue ("nLeftLeaf", "Number of left side leaf nodes", nLeftLeaf);
  cmd.AddValue ("nRightLeaf","Number of right side leaf nodes", nRightLeaf);
  cmd.AddValue ("nLeaf",     "Number of left and right side leaf nodes", nLeaf);
  cmd.Parse (argc,argv);
  if (nLeaf > 0)
    {
      nLeftLeaf = nLeaf;
      nRightLeaf = nLeaf;
    }

  // Create the point-to-point link helpers
  PointToPointHelper pointToPointRouter;
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("10Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("4ms"));
  PointToPointHelper pointToPointLeaf;
  pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("50Mbps"));
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("1ms"));

  PointToPointDumbbellHelper d (nLeftLeaf, pointToPointLeaf,
                                nRightLeaf, pointToPointLeaf,
                                pointToPointRouter);

  // Install Stack
  InternetStackHelper stack;
  d.InstallStack (stack);

  // Assign IP Addresses
  d.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"),
                         Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"),
                         Ipv4AddressHelper ("10.3.1.0", "255.255.255.0"));

  dir += (currentTime + "/");
  std::string dirToSave = "mkdir -p " + dir;
  system (dirToSave.c_str ());
  system ((dirToSave + "/pcap/").c_str ());
  system ((dirToSave + "/cwndTraces/").c_str ());
  system ((dirToSave + "/markTraces/").c_str ());

  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (1));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1446));
  Config::SetDefault ("ns3::TcpSocketBase::UseEcn", BooleanValue (true));
  Config::SetDefault ("ns3::PiQueueDisc::UseEcn", BooleanValue (true));
  Config::SetDefault ("ns3::PiQueueDisc::QueueRef", DoubleValue (94));
  Config::SetDefault ("ns3::PiQueueDisc::MaxSize", QueueSizeValue (QueueSize ("375p")));

  TrafficControlHelper tch;
  tch.SetRootQueueDisc ("ns3::PiQueueDisc");
  QueueDiscContainer qd;
  tch.Uninstall (d.GetLeft ()->GetDevice (0));
  qd.Add (tch.Install (d.GetLeft ()->GetDevice (0)).Get (0));
  Simulator::ScheduleNow (&CheckQueueSize, qd.Get (0));
  qd.Get (0)->TraceConnectWithoutContext ("Mark", MakeCallback(&MarkTraceR1));

  uint16_t port = 50000;
  InstallPacketSink (d.GetRight (0), port);      // A Sink 0 Applications
//  InstallPacketSink (d.GetRight (1), port);      // A Sink 0 Applications
//  InstallPacketSink (d.GetRight (2), port);      // A Sink 0 Applications
//  InstallPacketSink (d.GetRight (3), port);      // A Sink 0 Applications
//  InstallPacketSink (d.GetRight (4), port);      // A Sink 0 Applications

  InstallBulkSend (d.GetLeft (0), d.GetRightIpv4Address (0), port, 2, 0, MakeCallback (&CwndChangeG1));
//  InstallBulkSend (d.GetLeft (0), d.GetRightIpv4Address (1), port, 3, 0, MakeCallback (&CwndChangeG2));
//  InstallBulkSend (d.GetLeft (0), d.GetRightIpv4Address (2), port, 4, 0, MakeCallback (&CwndChangeG3));
//  InstallBulkSend (d.GetLeft (0), d.GetRightIpv4Address (3), port, 5, 0, MakeCallback (&CwndChangeG4));
//  InstallBulkSend (d.GetLeft (0), d.GetRightIpv4Address (4), port, 6, 0, MakeCallback (&CwndChangeG5));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

//  GtkConfigStore gcs;
//  gcs.ConfigureAttributes ();

  pointToPointLeaf.EnablePcapAll (dir + "pcap/N", true);

  Simulator::Stop (Seconds (11));
  Simulator::Run ();
  std::ofstream myfile;
  myfile.open (dir + "queueStats.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  for (uint32_t i = 0; i < qd.GetN (); i++)
    {
      myfile << std::endl;
      myfile << "Stat for " << i + 1 << " Queue";
      myfile << qd.Get (i)->GetStats ();
    }
  myfile.close ();

  Simulator::Destroy ();
  return 0;
}

/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 NITK Surathkal
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Vivek Jain <jain.vivek.anand@gmail.com>
 *          Viyom Mittal <viyommittal@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 */

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

using namespace ns3;
std::vector<std::stringstream> filePlotQueue;
std::vector<std::stringstream> filePlotPacketSojourn;
Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
std::string dir;

//Router 1
static void
CwndChangeR1A1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R1A1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR1B1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R1B1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR1D1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R1D1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
//Router 2
static void
CwndChangeR2E1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R2E1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR2E2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R2E2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR2A1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R2A1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR2B1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R2B1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
//Router 3
static void
CwndChangeR3A1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R3A1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR3F1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R3F1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
//Router 4
static void
CwndChangeR4B1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R4B1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR4H1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R4H1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR4H2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R4H2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
//Router 5
static void
CwndChangeR5C1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R5C1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR5C2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R5C2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR5C3 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R5C3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
//Router 6
static void
CwndChangeR6G1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R6G1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR6G2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R6G2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR6G3 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R6G3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR6G4 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R6G4.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR6G5 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R6G5.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR6G6 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R6G6.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeR6G7 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir+"cwndTraces/R6G7.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

void
TraceCwnd ()
{
//Router 1
  Config::ConnectWithoutContext ("/NodeList/7/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR1A1));
  Config::ConnectWithoutContext ("/NodeList/8/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR1B1));
  Config::ConnectWithoutContext ("/NodeList/9/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR1D1));
//Router 2
  Config::ConnectWithoutContext ("/NodeList/11/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR2E1));
  Config::ConnectWithoutContext ("/NodeList/11/$ns3::TcpL4Protocol/SocketList/1/CongestionWindow",
                                 MakeCallback (&CwndChangeR2E2));
  Config::ConnectWithoutContext ("/NodeList/12/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR2A1));
  Config::ConnectWithoutContext ("/NodeList/13/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR2B1));
//Router 3
  Config::ConnectWithoutContext ("/NodeList/15/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR3A1));
  Config::ConnectWithoutContext ("/NodeList/16/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR3F1));
//Router 4
  Config::ConnectWithoutContext ("/NodeList/18/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR4B1));
  Config::ConnectWithoutContext ("/NodeList/19/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR4H1));
  Config::ConnectWithoutContext ("/NodeList/19/$ns3::TcpL4Protocol/SocketList/1/CongestionWindow",
                                 MakeCallback (&CwndChangeR4H2));
//Router 5
  Config::ConnectWithoutContext ("/NodeList/21/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR5C1));
  Config::ConnectWithoutContext ("/NodeList/21/$ns3::TcpL4Protocol/SocketList/1/CongestionWindow",
                                 MakeCallback (&CwndChangeR5C2));
  Config::ConnectWithoutContext ("/NodeList/21/$ns3::TcpL4Protocol/SocketList/2/CongestionWindow",
                                 MakeCallback (&CwndChangeR5C3));
//Router 6
  Config::ConnectWithoutContext ("/NodeList/24/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow",
                                 MakeCallback (&CwndChangeR6G1));
  Config::ConnectWithoutContext ("/NodeList/24/$ns3::TcpL4Protocol/SocketList/1/CongestionWindow",
                                 MakeCallback (&CwndChangeR6G2));
  Config::ConnectWithoutContext ("/NodeList/24/$ns3::TcpL4Protocol/SocketList/2/CongestionWindow",
                                 MakeCallback (&CwndChangeR6G3));
  Config::ConnectWithoutContext ("/NodeList/24/$ns3::TcpL4Protocol/SocketList/3/CongestionWindow",
                                 MakeCallback (&CwndChangeR6G4));
  Config::ConnectWithoutContext ("/NodeList/24/$ns3::TcpL4Protocol/SocketList/4/CongestionWindow",
                                 MakeCallback (&CwndChangeR6G5));
  Config::ConnectWithoutContext ("/NodeList/24/$ns3::TcpL4Protocol/SocketList/5/CongestionWindow",
                                 MakeCallback (&CwndChangeR6G6));
  Config::ConnectWithoutContext ("/NodeList/24/$ns3::TcpL4Protocol/SocketList/6/CongestionWindow",
                                 MakeCallback (&CwndChangeR6G7));
}

void
CheckQueueSize (Ptr<QueueDisc> queue, uint32_t i)
{
  uint32_t qSize = queue->GetCurrentSize ().GetValue ();

  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.001), &CheckQueueSize, queue, i);

  std::ofstream fPlotQueue (filePlotQueue [i].str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << qSize << std::endl;
  fPlotQueue.close ();
}

void
CheckPacketSojourn (Ptr<QueueDisc> queue, uint32_t i)
{
  double qSize = queue->GetPacketSojournTime ().GetMilliSeconds ();

  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.001), &CheckPacketSojourn, queue, i);

  std::ofstream fPlotQueue (filePlotPacketSojourn [i].str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << qSize << std::endl;
  fPlotQueue.close ();
}

void InstallBulkSend (Ptr<Node> node, Ipv4Address address, uint16_t port)
{
  BulkSendHelper source ("ns3::TcpSocketFactory", 
                         InetSocketAddress (address, port));

  source.SetAttribute ("MaxBytes", UintegerValue (0));
  ApplicationContainer sourceApps = source.Install (node);
  sourceApps.Start (Seconds (uv->GetValue (0, 1)));
  sourceApps.Stop (Seconds (10.0));
}

void InstallPacketSink (Ptr<Node> node, uint16_t port)
{
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps = sink.Install (node);
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (10.0));
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

  uint32_t stream = 1;
  std::string transport_prot = "TcpNewReno";
  std::string queue_disc_type = "FifoQueueDisc";
  bool useEcn = false;
  uint32_t dataSize = 1446;
  CommandLine cmd;
  cmd.AddValue ("stream", "Seed value for random variable", stream);
  cmd.AddValue ("transport_prot", "Transport protocol to use: TcpNewReno, "
                "TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, "
                "TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus, TcpLedbat, "
                "TcpLp", transport_prot);
  cmd.AddValue ("queue_disc_type", "Queue disc type for gateway (e.g. ns3::CoDelQueueDisc)", queue_disc_type);
  cmd.AddValue ("useEcn", "Use ECN", useEcn);
  cmd.AddValue ("dataSize", "Data packet size", dataSize);
  cmd.Parse (argc, argv);

  uv->SetStream (stream);
  transport_prot = std::string ("ns3::") + transport_prot;
  queue_disc_type = std::string ("ns3::") + queue_disc_type;

  TypeId qdTid;
  NS_ABORT_MSG_UNLESS (TypeId::LookupByNameFailSafe (queue_disc_type, &qdTid), "TypeId " << queue_disc_type << " not found");

  // Select TCP variant
  if (transport_prot.compare ("ns3::TcpWestwoodPlus") == 0)
    {
      // TcpWestwoodPlus is not an actual TypeId name; we need TcpWestwood here
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpWestwood::GetTypeId ()));
      // the default protocol type in ns3::TcpWestwood is WESTWOOD
      Config::SetDefault ("ns3::TcpWestwood::ProtocolType", EnumValue (TcpWestwood::WESTWOODPLUS));
    }
  else
    {
      TypeId tcpTid;
      NS_ABORT_MSG_UNLESS (TypeId::LookupByNameFailSafe (transport_prot, &tcpTid), "TypeId " << transport_prot << " not found");
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName (transport_prot)));
    }

  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (dataSize));
  Config::SetDefault ("ns3::TcpSocketBase::UseEcn", BooleanValue (useEcn));
  Config::SetDefault ("ns3::PiQueueDisc::UseEcn", BooleanValue (useEcn));
  Config::SetDefault ("ns3::PiQueueDisc::QueueRef", DoubleValue (250));

  std::vector <std::pair <uint32_t, uint32_t>> nodes;
  nodes.push_back (std::make_pair (0,3));
  nodes.push_back (std::make_pair (1,3));
  nodes.push_back (std::make_pair (1,2));
  nodes.push_back (std::make_pair (1,2));
  nodes.push_back (std::make_pair (1,1));
  nodes.push_back (std::make_pair (2,1));
  nodes.push_back (std::make_pair (2,0));

  std::vector <PointToPointHelper> trunkLink;

  // Create the point-to-point link helpers
  PointToPointHelper pointToPointRouter;
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("50Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("13.33ms"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("100Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("6.66ms"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("50Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("3.33ms"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("150Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("3.33ms"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("150Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("3.33ms"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("50Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("6.66ms"));
  trunkLink.push_back (pointToPointRouter);

  PointToPointHelper pointToPointLeaf;
  pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("1000Mbps"));
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("0.001ms"));

  PointToPointGfcHelper gfc (7, nodes, trunkLink, pointToPointLeaf);


  // Install Stack
  InternetStackHelper stack;
  gfc.InstallStack (stack);

  gfc.AssignIpv4Address ();

  // Set up the acutal simulation
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  uint16_t port = 50000;

  InstallPacketSink (gfc.GetUp (5, 0), port);      // A Sink 0 Applications
  InstallPacketSink (gfc.GetUp (5, 0), port + 1);  // A Sink 1 Applications
  InstallPacketSink (gfc.GetUp (5, 0), port + 2);  // A Sink 2 Applications

  InstallPacketSink (gfc.GetUp (6, 1), port);      // B Sink 3 Applications
  InstallPacketSink (gfc.GetUp (6, 1), port + 1);  // B Sink 3 Applications
  InstallPacketSink (gfc.GetUp (6, 1), port + 2);  // B Sink 3 Applications

  InstallPacketSink (gfc.GetUp (5, 1), port);      // C Sink 0 Applications
  InstallPacketSink (gfc.GetUp (5, 1), port + 1);  // C Sink 1 Applications
  InstallPacketSink (gfc.GetUp (5, 1), port + 2);  // C Sink 2 Applications

  InstallPacketSink (gfc.GetUp (1, 0), port);      // D Sink 1 Application

  InstallPacketSink (gfc.GetUp (2, 0), port);      // E Sink 1 Applications
  InstallPacketSink (gfc.GetUp (2, 0), port + 1);  // E Sink 2 Applications

  InstallPacketSink (gfc.GetUp (3, 0), port);      // F Sink 1 Application

  for (uint8_t i = 0; i < 7; i++)
    {
      InstallPacketSink (gfc.GetUp (6, 0), port + i);      // G Sink 7 Applications
    }

  InstallPacketSink (gfc.GetUp (4, 0), port);      // H Sink 1 Applications
  InstallPacketSink (gfc.GetUp (4, 0), port + 1);  // H Sink 2 Applications

  // Application A
  InstallBulkSend (gfc.GetDown (0, 0), gfc.GetUpIpv4Address (5, 0), port);
  InstallBulkSend (gfc.GetDown (1, 1), gfc.GetUpIpv4Address (5, 0), port + 1);
  InstallBulkSend (gfc.GetDown (2, 0), gfc.GetUpIpv4Address (5, 0), port + 2);

  // Application B
  InstallBulkSend (gfc.GetDown (0, 1), gfc.GetUpIpv4Address (6, 1), port);
  InstallBulkSend (gfc.GetDown (1, 2), gfc.GetUpIpv4Address (6, 1), port + 1);
  InstallBulkSend (gfc.GetDown (3, 0), gfc.GetUpIpv4Address (6, 1), port + 2);

  // Application C
  InstallBulkSend (gfc.GetDown (4, 0), gfc.GetUpIpv4Address (5, 1), port);
  InstallBulkSend (gfc.GetDown (4, 0), gfc.GetUpIpv4Address (5, 1), port + 1);
  InstallBulkSend (gfc.GetDown (4, 0), gfc.GetUpIpv4Address (5, 1), port + 2);

  // Application D
  InstallBulkSend (gfc.GetDown (0, 2), gfc.GetUpIpv4Address (1, 0), port);

  // Application E
  InstallBulkSend (gfc.GetDown (1, 0), gfc.GetUpIpv4Address (2, 0), port);
  InstallBulkSend (gfc.GetDown (1, 0), gfc.GetUpIpv4Address (2, 0), port + 1);

  // Application F
  InstallBulkSend (gfc.GetDown (2, 1), gfc.GetUpIpv4Address (3, 0), port);

  // Application G
  for (uint8_t i = 0; i < 7; i++)
    {
      InstallBulkSend (gfc.GetDown (5, 0), gfc.GetUpIpv4Address (6, 0), port + i);
    }

  // Application H
  InstallBulkSend (gfc.GetDown (3, 1), gfc.GetUpIpv4Address (4, 0), port);
  InstallBulkSend (gfc.GetDown (3, 1), gfc.GetUpIpv4Address (4, 0), port + 1);

  dir = "results/" + currentTime + "/";
  std::string dirToSave = "mkdir -p " + dir;
  system (dirToSave.c_str ());
  system ((dirToSave + "/pcap/").c_str ());
  system ((dirToSave + "/cwndTraces/").c_str ());

  TrafficControlHelper tch;
  tch.SetRootQueueDisc (queue_disc_type);

  QueueDiscContainer qd;
  uint8_t x = 0;
  for (uint32_t i = 0; i < gfc.GetSwitchCount () - 1; i++)
    {
      tch.Uninstall (gfc.GetSwitch (i)->GetDevice (x));
      qd.Add (tch.Install (gfc.GetSwitch (i)->GetDevice (x)).Get (0));
      filePlotQueue.push_back (std::stringstream (dir + "/queue-" + std::to_string (i) + ".plotme"));
      remove (filePlotQueue [i].str ().c_str ());
      filePlotPacketSojourn.push_back (std::stringstream (dir + "/delay-" + std::to_string (i) + ".plotme"));
      remove (filePlotPacketSojourn [i].str ().c_str ());
      Simulator::ScheduleNow (&CheckQueueSize, qd.Get (i), i);
      Simulator::ScheduleNow (&CheckPacketSojourn, qd.Get (i), i);
      x = 1;
    }
  pointToPointRouter.EnablePcapAll (dir + "/pcap/N", false);

  Simulator::Schedule (Seconds (3.0000001), &TraceCwnd);

  Config::Set ("/$ns3::NodeListPriv/NodeList/0/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/0/$" + queue_disc_type + "/MaxSize", QueueSizeValue (QueueSize ("375p")));
  Config::Set ("/$ns3::NodeListPriv/NodeList/1/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$" + queue_disc_type + "/MaxSize", QueueSizeValue (QueueSize ("375p")));
  Config::Set ("/$ns3::NodeListPriv/NodeList/2/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$" + queue_disc_type + "/MaxSize", QueueSizeValue (QueueSize ("375p")));
  Config::Set ("/$ns3::NodeListPriv/NodeList/3/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$" + queue_disc_type + "/MaxSize", QueueSizeValue (QueueSize ("375p")));
  Config::Set ("/$ns3::NodeListPriv/NodeList/4/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$" + queue_disc_type + "/MaxSize", QueueSizeValue (QueueSize ("375p")));
  Config::Set ("/$ns3::NodeListPriv/NodeList/5/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$" + queue_disc_type + "/MaxSize", QueueSizeValue (QueueSize ("375p")));

  Config::Set ("/$ns3::NodeListPriv/NodeList/0/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/0/$ns3::PiQueueDisc/QueueRef", DoubleValue (94));
  Config::Set ("/$ns3::NodeListPriv/NodeList/1/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$ns3::PiQueueDisc/QueueRef", DoubleValue (94));
  Config::Set ("/$ns3::NodeListPriv/NodeList/2/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$ns3::PiQueueDisc/QueueRef", DoubleValue (94));
  Config::Set ("/$ns3::NodeListPriv/NodeList/3/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$ns3::PiQueueDisc/QueueRef", DoubleValue (94));
  Config::Set ("/$ns3::NodeListPriv/NodeList/4/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$ns3::PiQueueDisc/QueueRef", DoubleValue (94));
  Config::Set ("/$ns3::NodeListPriv/NodeList/5/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$ns3::PiQueueDisc/QueueRef", DoubleValue (94));

  // 8. Install FlowMonitor on all nodes
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();

//  GtkConfigStore gcs;
//  gcs.ConfigureAttributes ();
  std::ofstream myfile;
  myfile.open (dir + "config.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  myfile << "useEcn " << useEcn << "\n";
  myfile << "queue_disc_type " << queue_disc_type << "\n";
  myfile << "stream  " << stream << "\n";
  myfile << "transport_prot " << transport_prot << "\n";
  myfile << "dataSize " << dataSize << "\n";
  myfile.close();

  Simulator::Stop (Seconds (10));
  Simulator::Run ();

  myfile.open (dir + "flow.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  // 10. Print per flow statistics
  monitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
          Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
          myfile << "Flow " << i->first << " (" << t.sourceAddress << ":" << t.sourcePort << " -> " << t.destinationAddress << ":" << t.destinationPort << ")\n";
          myfile << "  Packets Dropped: " << i->second.packetsDropped.size () << "\n";
          myfile << "  Tx Packets: " << i->second.txPackets << "\n";
          myfile << "  Tx Bytes:   " << i->second.txBytes << "\n";
          myfile << "  TxOffered:  " << i->second.txBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
          myfile << "  Rx Packets: " << i->second.rxPackets << "\n";
          myfile << "  Rx Bytes:   " << i->second.rxBytes << "\n";
          myfile << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
    }
  myfile.close();

  myfile.open (dir + "queueStats.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  for (uint32_t i = 0; i < qd.GetN (); i++)
    {
      myfile << std::endl;
      myfile << "Stat for " << i + 1 << " Queue";
      myfile << qd.Get (i)->GetStats ();
    }
  myfile.close();

  Simulator::Destroy ();
  return 0;
}

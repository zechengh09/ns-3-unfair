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
#include "ns3/callback.h"

using namespace ns3;
std::vector<std::stringstream> filePlotQueue;
std::vector<std::stringstream> filePlotPacketSojourn;
Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
std::string dir;
double stopTime = 10;

static void
CwndChangeA1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/A1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeA2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/A2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeA3 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/A3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeB1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/B1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeB2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/B2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeB3 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/B3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeC1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/C1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeC2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/C2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeC3 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/C3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeD1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/D1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeD2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/D2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeD3 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/D3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeD4 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/D4.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeD5 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/D5.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeD6 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/D6.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeE1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/E1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeE2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/E2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeE3 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/E3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeE4 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/E4.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeE5 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/E5.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeE6 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/E6.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeF1 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/F1.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeF2 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/F2.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd / 1446.0 << std::endl;
  fPlotQueue.close ();
}

void
TraceCwnd (uint32_t node, uint32_t cwndWindow,
           Callback <void, uint32_t, uint32_t> CwndTrace)
{
  Config::ConnectWithoutContext ("/NodeList/" + std::to_string (node) + "/$ns3::TcpL4Protocol/SocketList/" + std::to_string (cwndWindow) + "/CongestionWindow", CwndTrace);
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
//  if ((nodeId == 19 && cwndWindow == 1) &&
//      (nodeId == 21 && cwndWindow == 2) &&
//      (nodeId == 24 && cwndWindow == 3)) //HACK: might not work with different seed
//    {
//      Simulator::Schedule (timeToStart + Seconds (0.3), &TraceCwnd, nodeId, cwndWindow, CwndTrace);
//    }
//  else
//    {
//      Simulator::Schedule (timeToStart + Seconds (0.001), &TraceCwnd, nodeId, cwndWindow, CwndTrace);
//    }
  Simulator::Schedule (Seconds (1), &TraceCwnd, nodeId, cwndWindow, CwndTrace);
  sourceApps.Stop (Seconds (stopTime));
}

void InstallPacketSink (Ptr<Node> node, uint16_t port)
{
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps = sink.Install (node);
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (stopTime));
}

static void
DropAtQueue (Ptr<OutputStreamWrapper> stream, Ptr<const QueueDiscItem> item)
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << " 1" << std::endl;
}

static void
MarkAtQueue (Ptr<OutputStreamWrapper> stream, Ptr<const QueueDiscItem> item, const char* reason)
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << " 1" << std::endl;
}

int main (int argc, char *argv[])
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,sizeof(buffer),"%d-%m-%Y-%I-%M-%S",timeinfo);
  std::string currentTime (buffer);

  uint32_t stream = 1;
  std::string transport_prot = "TcpNewReno";
  std::string queue_disc_type = "FifoQueueDisc";
  bool useEcn = false;
  uint32_t dataSize = 1446;
  uint32_t delAckCount = 2;

  CommandLine cmd;
  cmd.AddValue ("stream", "Seed value for random variable", stream);
  cmd.AddValue ("transport_prot", "Transport protocol to use: TcpNewReno, "
                "TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, "
                "TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus, TcpLedbat, "
                "TcpLp", transport_prot);
  cmd.AddValue ("queue_disc_type", "Queue disc type for gateway (e.g. ns3::CoDelQueueDisc)", queue_disc_type);
  cmd.AddValue ("useEcn", "Use ECN", useEcn);
  cmd.AddValue ("dataSize", "Data packet size", dataSize);
  cmd.AddValue ("delAckCount", "Delayed ack count", delAckCount);
  cmd.AddValue ("stopTime", "Stop time for applications / simulation time will be stopTime + 1", stopTime);
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

  Config::SetDefault ("ns3::TcpSocket::SndBufSize", UintegerValue (1 << 20));
  Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue (1 << 20));
  Config::SetDefault ("ns3::TcpSocket::InitialCwnd", UintegerValue (10));
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (delAckCount));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (dataSize));

  std::vector <std::pair <uint32_t, uint32_t> > nodes;
  nodes.push_back (std::make_pair (0,2));
  nodes.push_back (std::make_pair (1,2));
  nodes.push_back (std::make_pair (1,1));
  nodes.push_back (std::make_pair (2,1));
  nodes.push_back (std::make_pair (2,0));

  std::vector <PointToPointHelper> trunkLink;

  // Create the point-to-point link helpers
  PointToPointHelper pointToPointRouter;
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("50Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("0.25ms"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("150Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("0.25ms"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("150Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("0.25ms"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("100Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("0.25ms"));
  trunkLink.push_back (pointToPointRouter);

  PointToPointHelper pointToPointLeaf;
  pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("150Mbps"));
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("0.001ms"));

  PointToPointGfcHelper gfc (5, nodes, trunkLink, pointToPointLeaf);


  // Install Stack
  InternetStackHelper stack;
  gfc.InstallStack (stack);

  gfc.AssignIpv4Address ();

  // Set up the acutal simulation
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  uint16_t port = 50000;

  InstallPacketSink (gfc.GetUp (3, 0), port);      // A Sink 0 Applications
  InstallPacketSink (gfc.GetUp (3, 0), port + 1);  // A Sink 1 Applications
  InstallPacketSink (gfc.GetUp (3, 0), port + 2);  // A Sink 2 Applications

  InstallPacketSink (gfc.GetUp (4, 1), port);      // B Sink 3 Applications
  InstallPacketSink (gfc.GetUp (4, 1), port + 1);  // B Sink 3 Applications
  InstallPacketSink (gfc.GetUp (4, 1), port + 2);  // B Sink 3 Applications

  InstallPacketSink (gfc.GetUp (3, 1), port);      // C Sink 0 Applications
  InstallPacketSink (gfc.GetUp (3, 1), port + 1);  // C Sink 1 Applications
  InstallPacketSink (gfc.GetUp (3, 1), port + 2);  // C Sink 2 Applications

  for (uint8_t i = 0; i < 6; i++)
    {
      InstallPacketSink (gfc.GetUp (1, 0), port + i);      // D Sink 6 Applications
    }

  for (uint8_t i = 0; i < 6; i++)
    {
      InstallPacketSink (gfc.GetUp (4, 0), port + i);      // E Sink 6 Applications
    }

  InstallPacketSink (gfc.GetUp (2, 0), port);      // F Sink 1 Application
  InstallPacketSink (gfc.GetUp (2, 0), port + 1);      // F Sink 2 Application


  // Application A
  InstallBulkSend (gfc.GetDown (0, 0), gfc.GetUpIpv4Address (3, 0), port + 0, 5, 0, MakeCallback (&CwndChangeA1));
  InstallBulkSend (gfc.GetDown (0, 0), gfc.GetUpIpv4Address (3, 0), port + 1, 5, 1, MakeCallback (&CwndChangeA2));
  InstallBulkSend (gfc.GetDown (0, 0), gfc.GetUpIpv4Address (3, 0), port + 2, 5, 2, MakeCallback (&CwndChangeA3));

  // Application B
  InstallBulkSend (gfc.GetDown (1, 0), gfc.GetUpIpv4Address (4, 1), port + 0, 8, 0, MakeCallback (&CwndChangeB1));
  InstallBulkSend (gfc.GetDown (1, 0), gfc.GetUpIpv4Address (4, 1), port + 1, 8, 1, MakeCallback (&CwndChangeB2));
  InstallBulkSend (gfc.GetDown (1, 0), gfc.GetUpIpv4Address (4, 1), port + 2, 8, 2, MakeCallback (&CwndChangeB3));

  // Application C
  InstallBulkSend (gfc.GetDown (2, 0), gfc.GetUpIpv4Address (3, 1), port + 0, 11, 0, MakeCallback (&CwndChangeC1));
  InstallBulkSend (gfc.GetDown (2, 0), gfc.GetUpIpv4Address (3, 1), port + 1, 11, 1, MakeCallback (&CwndChangeC2));
  InstallBulkSend (gfc.GetDown (2, 0), gfc.GetUpIpv4Address (3, 1), port + 2, 11, 2, MakeCallback (&CwndChangeC3));

  // Application D
  InstallBulkSend (gfc.GetDown (0, 1), gfc.GetUpIpv4Address (1, 0), port + 0, 6, 0, MakeCallback (&CwndChangeD1));
  InstallBulkSend (gfc.GetDown (0, 1), gfc.GetUpIpv4Address (1, 0), port + 1, 6, 1, MakeCallback (&CwndChangeD2));
  InstallBulkSend (gfc.GetDown (0, 1), gfc.GetUpIpv4Address (1, 0), port + 2, 6, 2, MakeCallback (&CwndChangeD3));
  InstallBulkSend (gfc.GetDown (0, 1), gfc.GetUpIpv4Address (1, 0), port + 3, 6, 3, MakeCallback (&CwndChangeD4));
  InstallBulkSend (gfc.GetDown (0, 1), gfc.GetUpIpv4Address (1, 0), port + 4, 6, 4, MakeCallback (&CwndChangeD5));
  InstallBulkSend (gfc.GetDown (0, 1), gfc.GetUpIpv4Address (1, 0), port + 5, 6, 5, MakeCallback (&CwndChangeD6));

  // Application E
  InstallBulkSend (gfc.GetDown (3, 0), gfc.GetUpIpv4Address (4, 0), port + 0, 14, 0, MakeCallback (&CwndChangeE1));
  InstallBulkSend (gfc.GetDown (3, 0), gfc.GetUpIpv4Address (4, 0), port + 1, 14, 1, MakeCallback (&CwndChangeE2));
  InstallBulkSend (gfc.GetDown (3, 0), gfc.GetUpIpv4Address (4, 0), port + 2, 14, 2, MakeCallback (&CwndChangeE3));
  InstallBulkSend (gfc.GetDown (3, 0), gfc.GetUpIpv4Address (4, 0), port + 3, 14, 3, MakeCallback (&CwndChangeE4));
  InstallBulkSend (gfc.GetDown (3, 0), gfc.GetUpIpv4Address (4, 0), port + 4, 14, 4, MakeCallback (&CwndChangeE5));
  InstallBulkSend (gfc.GetDown (3, 0), gfc.GetUpIpv4Address (4, 0), port + 5, 14, 5, MakeCallback (&CwndChangeE6));

  // Application F
  InstallBulkSend (gfc.GetDown (1, 1), gfc.GetUpIpv4Address (2, 0), port + 0, 9, 0, MakeCallback (&CwndChangeF1));
  InstallBulkSend (gfc.GetDown (1, 1), gfc.GetUpIpv4Address (2, 0), port + 1, 9, 1, MakeCallback (&CwndChangeF2));

  dir = "results/" + currentTime + "/";
  std::string dirToSave = "mkdir -p " + dir;
  system (dirToSave.c_str ());
  system ((dirToSave + "/pcap/").c_str ());
  system ((dirToSave + "/cwndTraces/").c_str ());
  system ((dirToSave + "/queueTraces/").c_str ());
  system (("cp -R PlotScripts-1/* " + dir + "/pcap/").c_str ());

  TrafficControlHelper tch;
  tch.SetRootQueueDisc (queue_disc_type);

  QueueDiscContainer qd;
  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> streamWrapper;
  uint8_t x = 0;
  for (uint32_t i = 0; i < gfc.GetSwitchCount () - 1; i++)
    {
      tch.Uninstall (gfc.GetSwitch (i)->GetDevice (x));
      qd.Add (tch.Install (gfc.GetSwitch (i)->GetDevice (x)).Get (0));
      streamWrapper = asciiTraceHelper.CreateFileStream (dir + "/queueTraces/drop-" + std::to_string (i) + ".plotme");
      qd.Get (i)->TraceConnectWithoutContext ("Drop", MakeBoundCallback (&DropAtQueue, streamWrapper));
      streamWrapper = asciiTraceHelper.CreateFileStream (dir + "/queueTraces/mark-" + std::to_string (i) + ".plotme");
      qd.Get (i)->TraceConnectWithoutContext ("Mark", MakeBoundCallback (&MarkAtQueue, streamWrapper));
      filePlotQueue.push_back (std::stringstream (dir + "/queue-" + std::to_string (i) + ".plotme"));
      remove (filePlotQueue [i].str ().c_str ());
      filePlotPacketSojourn.push_back (std::stringstream (dir + "/delay-" + std::to_string (i) + ".plotme"));
      remove (filePlotPacketSojourn [i].str ().c_str ());
      Simulator::ScheduleNow (&CheckQueueSize, qd.Get (i), i);
      Simulator::ScheduleNow (&CheckPacketSojourn, qd.Get (i), i);
      x = 1;
    }
  pointToPointRouter.EnablePcapAll (dir + "/pcap/N", false);

  Config::Set ("/$ns3::NodeListPriv/NodeList/0/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/0/$" + queue_disc_type + "/MaxSize", QueueSizeValue (QueueSize ("38p")));
  Config::Set ("/$ns3::NodeListPriv/NodeList/1/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$" + queue_disc_type + "/MaxSize", QueueSizeValue (QueueSize ("38p")));
  Config::Set ("/$ns3::NodeListPriv/NodeList/2/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$" + queue_disc_type + "/MaxSize", QueueSizeValue (QueueSize ("38p")));
  Config::Set ("/$ns3::NodeListPriv/NodeList/3/$ns3::Node/$ns3::TrafficControlLayer/RootQueueDiscList/1/$" + queue_disc_type + "/MaxSize", QueueSizeValue (QueueSize ("38p")));

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
  myfile << "delAckCount " << delAckCount << "\n";
  myfile.close ();

  Simulator::Stop (Seconds (stopTime));
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
  myfile.close ();

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

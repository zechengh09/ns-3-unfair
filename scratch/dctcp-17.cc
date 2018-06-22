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
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 */

// The network topology used in this example is based on the Fig. 17 described in
// Mohammad Alizadeh, Albert Greenberg, David A. Maltz, Jitendra Padhye,
// Parveen Patel, Balaji Prabhakar, Sudipta Sengupta, and Murari Sridharan.
// "Data Center TCP (DCTCP)." In ACM SIGCOMM Computer Communication Review,
// Vol. 40, No. 4, pp. 63-74. ACM, 2010.

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
//std::vector<std::stringstream> filePlotPacketSojourn;
Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
std::string dir = "results/gfc-dctcp/";
double stopTime = 10;

void
CheckQueueSize (Ptr<QueueDisc> queue)
{
  uint32_t qSize = queue->GetCurrentSize ().GetValue ();

  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.001), &CheckQueueSize, queue);

  std::ofstream fPlotQueue (std::stringstream (dir + "queue-0.plotme").str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << qSize << std::endl;
  fPlotQueue.close ();
}

void
CheckQueueSize1 (Ptr<QueueDisc> queue)
{
  uint32_t qSize = queue->GetCurrentSize ().GetValue ();

  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.001), &CheckQueueSize1, queue);

  std::ofstream fPlotQueue (std::stringstream (dir + "queue-1.plotme").str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << qSize << std::endl;
  fPlotQueue.close ();
}

void
CheckQueueSize2 (Ptr<QueueDisc> queue)
{
  uint32_t qSize = queue->GetCurrentSize ().GetValue ();

  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.001), &CheckQueueSize2, queue);

  std::ofstream fPlotQueue (std::stringstream (dir + "queue-2.plotme").str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << qSize << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS10 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S10.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS11 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S11.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS12 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S12.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS13 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S13.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS14 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S14.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS15 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S15.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS16 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S16.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS17 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S17.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS18 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S18.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS19 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S19.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS20 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S20.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS21 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S21.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS22 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S22.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS23 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S23.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS24 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S24.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS25 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S25.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS26 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S26.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS27 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S27.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS28 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S28.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS29 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S29.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS210 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S210.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS211 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S211.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS212 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S212.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS213 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S213.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS214 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S214.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS215 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S215.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS216 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S216.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS217 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S217.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS218 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S218.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS219 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S219.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS30 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S30.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS31 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S31.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS32 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S32.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS33 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S33.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS34 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S34.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeS35 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S35.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS36 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S36.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS37 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S37.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS38 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S38.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}
static void
CwndChangeS39 (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/S39.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
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
  sinkApps.Start (Seconds (uv->GetValue (0, 1)));
  sinkApps.Stop (Seconds (stopTime));
}

void InstallBulkSend (Ptr<Node> node, Ipv4Address address, uint16_t port, 
                      uint32_t nodeId, uint32_t cwndWindow,
                      Callback <void, uint32_t, uint32_t> CwndTrace)
{

  BulkSendHelper source ("ns3::TcpSocketFactory", 
                         InetSocketAddress (address, port));

  source.SetAttribute ("MaxBytes", UintegerValue (0));
  ApplicationContainer sourceApps = source.Install (node);
  Time timeToStart = Seconds (0);
  sourceApps.Start (timeToStart);
  Simulator::Schedule (timeToStart + Seconds (0.001), &TraceCwnd, nodeId, cwndWindow, CwndTrace);
  sourceApps.Stop (Seconds (stopTime));
}



int main (int argc, char *argv[])
{
  uint32_t stream = 1;
  std::string transport_prot = "TcpNewReno";
  std::string queue_disc_type = "FifoQueueDisc";
  bool useEcn = true;
  uint32_t dataSize = 1446;
  uint32_t delAckCount = 2;

  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%d-%m-%Y-%I-%M-%S",timeinfo);
  std::string currentTime (buffer);

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
  cmd.AddValue ("stopTime", "Stop time for applications / simulation time will be stopTime", stopTime);
  cmd.Parse (argc,argv);

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

  NodeContainer S1, S2, S3, R1, R2, T, Scorp;
  T.Create (2);
  Scorp.Create (1);

  S1.Create (10);
  S2.Create (20);
  S3.Create (10);

  R2.Create (20);
  R1.Create (1);


  PointToPointHelper pointToPointSR;
  pointToPointSR.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPointSR.SetChannelAttribute ("Delay", StringValue ("0.01ms"));

  PointToPointHelper pointToPointT;
  pointToPointT.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  pointToPointT.SetChannelAttribute ("Delay", StringValue ("0.01ms"));

  PointToPointHelper pointToPointT1;
  pointToPointT1.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  pointToPointT1.SetChannelAttribute ("Delay", StringValue ("0.01ms"));

  NetDeviceContainer T1ScorpDev, T2ScorpDev, S1T1Dev, S2T1Dev, S3T2Dev, R1T2Dev, R2T2Dev;
  T1ScorpDev = pointToPointT.Install (T.Get (0), Scorp.Get (0));
  T2ScorpDev = pointToPointT1.Install (Scorp.Get (0), T.Get (1));
  R1T2Dev = pointToPointSR.Install (R1.Get (0), T.Get (1));

  for (uint32_t i = 0; i < S1.GetN (); i++)
    {
      S1T1Dev.Add (pointToPointSR.Install (S1.Get (i), T.Get (0)));
      S3T2Dev.Add (pointToPointSR.Install (S3.Get (i), T.Get (1)));
    }

  for (uint32_t i = 0; i < S2.GetN (); i++)
    {
      S2T1Dev.Add (pointToPointSR.Install (S2.Get (i), T.Get (0)));
     
    }

  for (uint32_t i = 0; i < R2.GetN (); i++)
    {
      R2T2Dev.Add (pointToPointSR.Install (R2.Get (i), T.Get (1)));
    }

  InternetStackHelper stack;
  stack.Install (S1);
  stack.Install (S2);
  stack.Install (S3);
  stack.Install (R1);
  stack.Install (R2);
  stack.Install (T);
  stack.Install (Scorp);

  //Assign IP address. We keep each sender-switch link in a different network
  Ipv4AddressHelper address;
  Ipv4InterfaceContainer S1Int, S2Int, S3Int, R1Int, R2Int, T1Int,T2Int;

  address.SetBase ("10.1.0.0", "255.255.255.0");



  for(uint32_t i=0; i<S1T1Dev.GetN (); i = i+2)
  {
     address.NewNetwork ();
     NetDeviceContainer S1iT1i;
     S1iT1i.Add(S1T1Dev.Get (i));
     S1iT1i.Add(S1T1Dev.Get (i+1));
     S1Int.Add (address.Assign (S1iT1i));
  }
  address.SetBase ("10.2.0.0", "255.255.255.0");


  for(uint32_t i=0; i<S2T1Dev.GetN (); i = i+2)
  {
     address.NewNetwork ();
     NetDeviceContainer S2iT1i;
     S2iT1i.Add(S2T1Dev.Get (i));
     S2iT1i.Add(S2T1Dev.Get (i+1));
     S2Int.Add (address.Assign (S2iT1i));
  }

  address.SetBase ("10.3.0.0", "255.255.255.0");

  for(uint32_t i=0; i<S3T2Dev.GetN (); i = i+2)
  {
     address.NewNetwork ();
     NetDeviceContainer S3iT2i;
     S3iT2i.Add(S3T2Dev.Get (i));
     S3iT2i.Add(S3T2Dev.Get (i+1));
     S3Int.Add (address.Assign (S3iT2i));
  }

  address.SetBase ("10.4.0.0", "255.255.255.0");
  R1Int = address.Assign (R1T2Dev);
  address.SetBase ("10.5.0.0", "255.255.255.0");


  for(uint32_t i=0; i<R2T2Dev.GetN (); i = i+2)
  {
     address.NewNetwork ();
     NetDeviceContainer R2iT2i;
     R2iT2i.Add(R2T2Dev.Get (i));
     R2iT2i.Add(R2T2Dev.Get (i+1));
     R2Int.Add (address.Assign (R2iT2i));
  }

  address.SetBase ("10.6.0.0", "255.255.255.0");
  T1Int = address.Assign (T1ScorpDev);

  address.SetBase ("10.7.0.0", "255.255.255.0");
  T2Int = address.Assign (T2ScorpDev);

  dir += (currentTime + "/");
  std::string dirToSave = "mkdir -p " + dir;
  system (dirToSave.c_str ());
  system ((dirToSave + "/pcap/").c_str ());
  system ((dirToSave + "/cwndTraces/").c_str ());
  system ((dirToSave + "/markTraces/").c_str ());
  system ((dirToSave + "/queueTraces/").c_str ());
  system (("cp -R PlotScripts-gfc-dctcp/* " + dir + "/pcap/").c_str ());

  Config::SetDefault ("ns3::TcpSocket::SndBufSize", UintegerValue (1 << 20));
  Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue (1 << 20));
  Config::SetDefault ("ns3::TcpSocket::InitialCwnd", UintegerValue (10));
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (delAckCount));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (dataSize));
  Config::SetDefault ("ns3::TcpSocketBase::UseEcn", BooleanValue (useEcn));
  Config::SetDefault ("ns3::PiQueueDisc::UseEcn", BooleanValue (useEcn));
  Config::SetDefault ("ns3::PiQueueDisc::MeanPktSize", UintegerValue (1500));
  Config::SetDefault ("ns3::PiQueueDisc::A", DoubleValue (0.002013602268));
  Config::SetDefault ("ns3::PiQueueDisc::B", DoubleValue (0.001342401512));
  Config::SetDefault ("ns3::PiQueueDisc::W", DoubleValue (400));
  Config::SetDefault ("ns3::PiQueueDisc::QueueRef", DoubleValue (10));
  Config::SetDefault (queue_disc_type + "::MaxSize", QueueSizeValue (QueueSize ("38p")));

  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> streamWrapper;

  TrafficControlHelper tch;
  tch.SetRootQueueDisc (queue_disc_type);

  QueueDiscContainer qd, qd1;
/*  tch.Uninstall (T.Get (0)->GetDevice (0));
  qd.Add (tch.Install (T.Get (0)->GetDevice (0)).Get (0));*/
  tch.Uninstall (T1ScorpDev);
  qd = tch.Install (T1ScorpDev);
  Simulator::ScheduleNow (&CheckQueueSize, qd.Get (0));
  streamWrapper = asciiTraceHelper.CreateFileStream (dir + "/queueTraces/drop-0.plotme");
  qd.Get (0)->TraceConnectWithoutContext ("Drop", MakeBoundCallback (&DropAtQueue, streamWrapper));
  streamWrapper = asciiTraceHelper.CreateFileStream (dir + "/queueTraces/mark-0.plotme");
  qd.Get (0)->TraceConnectWithoutContext ("Mark", MakeBoundCallback (&MarkAtQueue, streamWrapper));

/*  tch.Uninstall (T.Get (1)->GetDevice (1));
  qd1.Add (tch.Install (T.Get (1)->GetDevice (1)).Get (0));*/
  tch.Uninstall (T2ScorpDev);
  qd1 = tch.Install (T2ScorpDev);
  Simulator::ScheduleNow (&CheckQueueSize1, qd1.Get (0));
  streamWrapper = asciiTraceHelper.CreateFileStream (dir + "/queueTraces/drop-1.plotme");
  qd1.Get (0)->TraceConnectWithoutContext ("Drop", MakeBoundCallback (&DropAtQueue, streamWrapper));
  streamWrapper = asciiTraceHelper.CreateFileStream (dir + "/queueTraces/mark-1.plotme");
  qd1.Get (0)->TraceConnectWithoutContext ("Mark", MakeBoundCallback (&MarkAtQueue, streamWrapper));

  Simulator::ScheduleNow (&CheckQueueSize2, qd1.Get (1));
  streamWrapper = asciiTraceHelper.CreateFileStream (dir + "/queueTraces/drop-2.plotme");
  qd1.Get (1)->TraceConnectWithoutContext ("Drop", MakeBoundCallback (&DropAtQueue, streamWrapper));
  streamWrapper = asciiTraceHelper.CreateFileStream (dir + "/queueTraces/mark-2.plotme");
  qd1.Get (1)->TraceConnectWithoutContext ("Mark", MakeBoundCallback (&MarkAtQueue, streamWrapper));


  uint16_t port = 50000;
  //Install Sink applications on R1
  for(int i=0; i<20; i++)
    {
      InstallPacketSink (R1.Get (0), port+i);
    }

  //Install BulkSend applications on S1 and S3
  InstallBulkSend (S1.Get (0), R1Int.GetAddress (0), port, 3, 0, MakeCallback (&CwndChangeS10));
  InstallBulkSend (S1.Get (1), R1Int.GetAddress (0), port+1, 4, 0, MakeCallback (&CwndChangeS11));
  InstallBulkSend (S1.Get (2), R1Int.GetAddress (0), port+2, 5, 0, MakeCallback (&CwndChangeS12));
  InstallBulkSend (S1.Get (3), R1Int.GetAddress (0), port+3, 6, 0, MakeCallback (&CwndChangeS13));
  InstallBulkSend (S1.Get (4), R1Int.GetAddress (0), port+4, 7, 0, MakeCallback (&CwndChangeS14));
  InstallBulkSend (S1.Get (5), R1Int.GetAddress (0), port+5, 8, 0, MakeCallback (&CwndChangeS15));
  InstallBulkSend (S1.Get (6), R1Int.GetAddress (0), port+6, 9, 0, MakeCallback (&CwndChangeS16));
  InstallBulkSend (S1.Get (7), R1Int.GetAddress (0), port+7, 10, 0, MakeCallback (&CwndChangeS17));
  InstallBulkSend (S1.Get (8), R1Int.GetAddress (0), port+8, 11, 0, MakeCallback (&CwndChangeS18));
  InstallBulkSend (S1.Get (9), R1Int.GetAddress (0), port+9, 12, 0, MakeCallback (&CwndChangeS19));
  InstallBulkSend (S3.Get (0), R1Int.GetAddress (0), port+10, 33, 0, MakeCallback (&CwndChangeS30));
  InstallBulkSend (S3.Get (1), R1Int.GetAddress (0), port+11, 34, 0, MakeCallback (&CwndChangeS31));
  InstallBulkSend (S3.Get (2), R1Int.GetAddress (0), port+12, 35, 0, MakeCallback (&CwndChangeS32));
  InstallBulkSend (S3.Get (3), R1Int.GetAddress (0), port+13, 36, 0, MakeCallback (&CwndChangeS33));
  InstallBulkSend (S3.Get (4), R1Int.GetAddress (0), port+14, 37, 0, MakeCallback (&CwndChangeS34));
  InstallBulkSend (S3.Get (5), R1Int.GetAddress (0), port+15, 38, 0, MakeCallback (&CwndChangeS35));
  InstallBulkSend (S3.Get (6), R1Int.GetAddress (0), port+16, 39, 0, MakeCallback (&CwndChangeS36));
  InstallBulkSend (S3.Get (7), R1Int.GetAddress (0), port+17, 40, 0, MakeCallback (&CwndChangeS37));
  InstallBulkSend (S3.Get (8), R1Int.GetAddress (0), port+18, 41, 0, MakeCallback (&CwndChangeS38));
  InstallBulkSend (S3.Get (9), R1Int.GetAddress (0), port+19, 42, 0, MakeCallback (&CwndChangeS39));

  //Install Sink applications on R2
  for (uint32_t i = 0; i < R2.GetN (); i++)
    {
      InstallPacketSink (R2.Get (i), port);
    }

  //Install BulkSend applications on S2
  InstallBulkSend (S2.Get (0), R2Int.GetAddress (0), port, 13, 0, MakeCallback (&CwndChangeS20));
  InstallBulkSend (S2.Get (1), R2Int.GetAddress (2), port, 14, 0, MakeCallback (&CwndChangeS21));
  InstallBulkSend (S2.Get (2), R2Int.GetAddress (4), port, 15, 0, MakeCallback (&CwndChangeS22));
  InstallBulkSend (S2.Get (3), R2Int.GetAddress (6), port, 16, 0, MakeCallback (&CwndChangeS23));
  InstallBulkSend (S2.Get (4), R2Int.GetAddress (8), port, 17, 0, MakeCallback (&CwndChangeS24));
  InstallBulkSend (S2.Get (5), R2Int.GetAddress (10), port, 18, 0, MakeCallback (&CwndChangeS25));
  InstallBulkSend (S2.Get (6), R2Int.GetAddress (12), port, 19, 0, MakeCallback (&CwndChangeS26));
  InstallBulkSend (S2.Get (7), R2Int.GetAddress (14), port, 20, 0, MakeCallback (&CwndChangeS27));
  InstallBulkSend (S2.Get (8), R2Int.GetAddress (16), port, 21, 0, MakeCallback (&CwndChangeS28));
  InstallBulkSend (S2.Get (9), R2Int.GetAddress (18), port, 22, 0, MakeCallback (&CwndChangeS29));
  InstallBulkSend (S2.Get (10), R2Int.GetAddress (20), port, 23, 0, MakeCallback (&CwndChangeS210));
  InstallBulkSend (S2.Get (11), R2Int.GetAddress (22), port, 24, 0, MakeCallback (&CwndChangeS211));
  InstallBulkSend (S2.Get (12), R2Int.GetAddress (24), port, 25, 0, MakeCallback (&CwndChangeS212));
  InstallBulkSend (S2.Get (13), R2Int.GetAddress (26), port, 26, 0, MakeCallback (&CwndChangeS213));
  InstallBulkSend (S2.Get (14), R2Int.GetAddress (28), port, 27, 0, MakeCallback (&CwndChangeS214));
  InstallBulkSend (S2.Get (15), R2Int.GetAddress (30), port, 28, 0, MakeCallback (&CwndChangeS215));
  InstallBulkSend (S2.Get (16), R2Int.GetAddress (32), port, 29, 0, MakeCallback (&CwndChangeS216));
  InstallBulkSend (S2.Get (17), R2Int.GetAddress (34), port, 30, 0, MakeCallback (&CwndChangeS217));
  InstallBulkSend (S2.Get (18), R2Int.GetAddress (36), port, 31, 0, MakeCallback (&CwndChangeS218));
  InstallBulkSend (S2.Get (19), R2Int.GetAddress (38), port, 32, 0, MakeCallback (&CwndChangeS219));


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  pointToPointSR.EnablePcapAll (dir + "pcap/N", true);

  Simulator::Stop (Seconds (stopTime));
  Simulator::Run ();
  std::ofstream myfile;
  myfile.open (dir + "queueStats.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  myfile << std::endl;
  myfile << "Stat for Queue 1";
  myfile << qd.Get (0)->GetStats ();
  myfile << "Stat for Queue 2";
  myfile << qd1.Get (0)->GetStats ();
  myfile.close ();

  myfile.open (dir + "config.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  myfile << "useEcn " << useEcn << "\n";
  myfile << "queue_disc_type " << queue_disc_type << "\n";
  myfile << "stream  " << stream << "\n";
  myfile << "transport_prot " << transport_prot << "\n";
  myfile << "dataSize " << dataSize << "\n";
  myfile << "delAckCount " << delAckCount << "\n";
  myfile << "stopTime " << stopTime << "\n";
  myfile.close ();

  Simulator::Destroy ();
  return 0;

}

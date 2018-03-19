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
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/applications-module.h"
#include "ns3/packet-sink.h"
#include "ns3/traffic-control-module.h"
#include "ns3/log.h"

using namespace ns3;

void InstallBulkSend (Ptr<Node> node, Ipv4Address address, uint16_t port)
{
  BulkSendHelper source ("ns3::TcpSocketFactory", 
                         InetSocketAddress (address, port));

  source.SetAttribute ("MaxBytes", UintegerValue (0));
  ApplicationContainer sourceApps = source.Install (node);
  sourceApps.Start (Seconds (0.0));
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
  std::string transport_prot = "TcpNewReno";
  std::string queue_disc_type = "FifoQueueDisc";

  CommandLine cmd;
  cmd.AddValue ("transport_prot", "Transport protocol to use: TcpNewReno, "
                "TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, "
                "TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus, TcpLedbat, "
                "TcpLp", transport_prot);
  cmd.AddValue ("queue_disc_type", "Queue disc type for gateway (e.g. ns3::CoDelQueueDisc)", queue_disc_type);
  cmd.Parse (argc, argv);

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

  Config::SetDefault ("ns3::TcpSocketBase::UseEcn", BooleanValue (true));
  Config::SetDefault ("ns3::PiQueueDisc::UseEcn", BooleanValue (true));
  Config::SetDefault ("ns3::PiQueueDisc::QueueLimit", DoubleValue (1000));

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
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("13.32ms"));
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
  pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("150Mbps"));
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("3.33ms"));

  PointToPointGfcHelper gfc (7, nodes, trunkLink, pointToPointLeaf);


  // Install Stack
  InternetStackHelper stack;
  gfc.InstallStack (stack);

  gfc.AssignIpv4Address ();

  // Set up the acutal simulation
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  uint16_t port = 45000;

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

  TrafficControlHelper tch;
  tch.SetRootQueueDisc (queue_disc_type);
  tch.Uninstall (gfc.GetSwitchNetDevice (2));
  tch.Install (gfc.GetSwitchNetDevice (2));
  tch.Uninstall (gfc.GetSwitchNetDevice (4));
  tch.Install (gfc.GetSwitchNetDevice (4));
  tch.Uninstall (gfc.GetSwitchNetDevice (5));
  QueueDiscContainer qd = tch.Install (gfc.GetSwitchNetDevice (5));


  pointToPointRouter.EnablePcapAll ("pcap/N", false);

  Simulator::Stop (Seconds (10));
  Simulator::Run ();
  QueueDisc::Stats st = qd.Get (0)->GetStats ();
  std::cout << st;
  Simulator::Destroy ();
  return 0;
}

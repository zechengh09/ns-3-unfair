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

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  std::vector <std::pair <uint32_t, uint32_t>> nodes;
  nodes.push_back (std::make_pair (1,1));
  nodes.push_back (std::make_pair (1,2));
  nodes.push_back (std::make_pair (1,1));
  nodes.push_back (std::make_pair (2,1));
  nodes.push_back (std::make_pair (1,1));

  std::vector <PointToPointHelper> trunkLink;
  // Create the point-to-point link helpers
  PointToPointHelper pointToPointRouter;
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("50Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("1ms"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("150Mbps"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("150Mbps"));
  trunkLink.push_back (pointToPointRouter);
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("100Mbps"));
  trunkLink.push_back (pointToPointRouter);

  PointToPointHelper pointToPointLeaf;
  pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("150Mbps"));
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("1ms"));

  PointToPointGfcHelper gfc (5, nodes, trunkLink, pointToPointLeaf);

  // Install Stack
  InternetStackHelper stack;
  gfc.InstallStack (stack);

  gfc.AssignIpv4Address ();

  // Set up the acutal simulation
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

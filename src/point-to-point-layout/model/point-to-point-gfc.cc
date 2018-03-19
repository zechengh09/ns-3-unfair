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
#include <cmath>
#include <iostream>
#include <sstream>

// ns3 includes
#include "ns3/log.h"
#include "ns3/point-to-point-gfc.h"
#include "ns3/constant-position-mobility-model.h"

#include "ns3/node-list.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/vector.h"
#include "ns3/ipv6-address-generator.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PointToPointGfcHelper");

PointToPointGfcHelper::PointToPointGfcHelper (uint32_t nSwitch,
                                              std::vector <std::pair <uint32_t, uint32_t>> nSwitchLeafNodes,
                                              std::vector <PointToPointHelper> trunkLink,
                                              PointToPointHelper accessLink)
{
  NS_ASSERT (nSwitchLeafNodes.size () == nSwitch);
  NS_ASSERT (trunkLink.size () == nSwitch - 1);
  m_switches.Create (nSwitch);

  for (uint32_t i = 0; i < nSwitch - 1; ++i)
    {
      NetDeviceContainer c = trunkLink [i].Install (m_switches.Get (i), m_switches.Get (i + 1));
      m_switchDevices.push_back (c);
    }

  for (uint32_t i = 0; i < nSwitchLeafNodes.size (); ++i)
    {
      NodeContainer ncUp;
      ncUp.Create (nSwitchLeafNodes [i].first);

      NodeContainer ncDown;
      ncDown.Create (nSwitchLeafNodes [i].second);

      m_nodes.push_back (std::make_pair (ncUp, ncDown));

      NetDeviceContainer c1, c2;
      for (uint32_t j = 0; j < nSwitchLeafNodes [i].first; ++j)
        {
          c1.Add (accessLink.Install (m_switches.Get (i), m_nodes [i].first.Get (j)));
        }

      for (uint32_t j = 0; j < nSwitchLeafNodes [i].second; ++j)
        {
          c2.Add (accessLink.Install (m_switches.Get (i), m_nodes [i].second.Get (j)));
        }
      m_nodesDevices.push_back (std::make_pair (c1, c2));
    }

}

PointToPointGfcHelper::~PointToPointGfcHelper ()
{
}

void PointToPointGfcHelper::InstallStack (InternetStackHelper stack)
{
  stack.Install (m_switches);
  for (uint32_t i = 0; i < m_nodes.size (); ++i)
    {
      stack.Install (m_nodes [i].first);
      stack.Install (m_nodes [i].second);
    }
}

void PointToPointGfcHelper::AssignIpv4Address ()
{
  Ipv4AddressHelper ipAddresses ("10.0.0.0", "255.255.255.0");
  for (uint32_t i = 0; i < m_switchDevices.size (); ++i)
    {
      NetDeviceContainer ndc;
      ndc.Add (m_switchDevices [i].Get (0));
      ndc.Add (m_switchDevices [i].Get (1));
      Ipv4InterfaceContainer ifc = ipAddresses.Assign (ndc);
      m_switchInterfaces.push_back (ifc);
      ipAddresses.NewNetwork ();
    }

  for (uint32_t i = 0; i < m_nodesDevices.size (); ++i)
    {
      std::vector<Ipv4InterfaceContainer> ifc1, ifc2;
      for (uint32_t j = 0; j < m_nodesDevices [i].first.GetN (); ++j, ++j)
        {
          NetDeviceContainer ndc;
          ndc.Add (m_nodesDevices [i].first.Get (j));
          ndc.Add (m_nodesDevices [i].first.Get (j + 1));
          ifc1.push_back(ipAddresses.Assign (ndc));
          ipAddresses.NewNetwork ();
        }
      m_upperIpv4Interfaces.push_back (ifc1);

      for (uint32_t j = 0; j < m_nodesDevices [i].second.GetN (); ++j, ++j)
        {
          NetDeviceContainer ndc;
          ndc.Add (m_nodesDevices [i].second.Get (j));
          ndc.Add (m_nodesDevices [i].second.Get (j + 1));
          ifc2.push_back(ipAddresses.Assign (ndc));
          ipAddresses.NewNetwork ();
        }
      m_lowerIpv4Interfaces.push_back (ifc2);
    }
}

uint32_t
PointToPointGfcHelper::GetSwitchCount () const
{
  return m_switches.GetN ();
}

Ptr<Node>
PointToPointGfcHelper::GetSwitch (uint32_t i) const
{
  NS_ASSERT (i < m_switches.GetN ());
  return m_switches.Get (i);
}

uint32_t
PointToPointGfcHelper::GetUpCount (uint32_t i) const
{
  NS_ASSERT (i < m_switches.GetN ());
  return m_nodes [i].first.GetN ();
}

Ptr<Node>
PointToPointGfcHelper::GetUp (uint32_t i, uint32_t j) const
{
  NS_ASSERT (i < m_switches.GetN ());
  NS_ASSERT (j < m_nodes [i].first.GetN ());
  return m_nodes [i].first.Get (j);
}

uint32_t
PointToPointGfcHelper::GetDownCount (uint32_t i) const
{
  NS_ASSERT (i < m_switches.GetN ());
  return m_nodes [i].second.GetN ();
}

Ptr<Node>
PointToPointGfcHelper::GetDown (uint32_t i, uint32_t j) const
{
  NS_ASSERT (i < m_switches.GetN ());
  NS_ASSERT (j < m_nodes [i].second.GetN ());
  return m_nodes [i].second.Get (j);
}

Ipv4Address
PointToPointGfcHelper::GetUpIpv4Address (uint32_t i, uint32_t j) const
{
  NS_ASSERT (i < m_switches.GetN ());
  NS_ASSERT (j < m_nodes [i].first.GetN ());
  return m_upperIpv4Interfaces [i][j].GetAddress (1, 0);
}

Ipv4Address
PointToPointGfcHelper::GetDownIpv4Address (uint32_t i, uint32_t j) const
{
  return m_lowerIpv4Interfaces [i][j].GetAddress (1, 0);
}

NetDeviceContainer
PointToPointGfcHelper::GetSwitchNetDevice (uint32_t i) const
{
  NS_ASSERT (i < m_switchDevices.size ());
  return m_switchDevices [i];
}

} // namespace ns3

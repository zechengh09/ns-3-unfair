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

#ifndef POINT_TO_POINT_GFC_HELPER_H
#define POINT_TO_POINT_GFC_HELPER_H

#include <string>

#include "point-to-point-helper.h"
#include "ipv4-address-helper.h"
#include "ipv6-address-helper.h"
#include "internet-stack-helper.h"
#include "ipv4-interface-container.h"
#include "ipv6-interface-container.h"

namespace ns3 {

/**
 * \ingroup point-to-point-layout
 *
 * \brief A helper to make it easier to create a Gfc topology
 * with p2p links
 */
class PointToPointGfcHelper
{
public:
  PointToPointGfcHelper (uint32_t nSwitch,
                         std::vector <std::pair <uint32_t, uint32_t>> nSwitchLeafNodes,
                         std::vector <PointToPointHelper> trunkLink,
                         PointToPointHelper accessLink);

  ~PointToPointGfcHelper ();

  void      InstallStack (InternetStackHelper stack);
  void      AssignIpv4Address ();

  /**
   * \returns number of switch node
   */
  uint32_t GetSwitchCount () const;

  /**
   * \returns pointer to the i'th switch node
   * \param i node number
   */
  Ptr<Node> GetSwitch (uint32_t i) const;

  /**
   * \returns number of upper node at i'th switch node
   */
  uint32_t GetUpCount (uint32_t i) const;

  /**
   * \returns pointer to the j'th up node of i'th switch node
   * \param i switch number
   * \param j node number
   */
  Ptr<Node> GetUp (uint32_t i, uint32_t j) const;

  /**
   * \returns number of lower node at i'th switch node
   */
  uint32_t GetDownCount (uint32_t i) const;

  /**
   * \returns pointer to the j'th lower node of i'th switch node
   * \param i switch number
   * \param j node number
   */
  Ptr<Node> GetDown (uint32_t i, uint32_t j) const;

  /**
   * \returns Ipv4Address to the j'th upper node of i'th switch node
   * \param i switch number
   * \param j node number
   */
  Ipv4Address GetUpIpv4Address (uint32_t i, uint32_t j) const;

  /**
   * \returns Ipv4Address to the j'th lower node of i'th switch node
   * \param i switch number
   * \param j node number
   */
  Ipv4Address GetDownIpv4Address (uint32_t i, uint32_t j) const;

  NetDeviceContainer GetSwitchNetDevice (uint32_t i) const;

private:
  NodeContainer          m_switches;
  std::vector <NetDeviceContainer>     m_switchDevices;
  std::vector <Ipv4InterfaceContainer> m_switchInterfaces;

  std::vector <std::vector<Ipv4InterfaceContainer>> m_upperIpv4Interfaces;
  std::vector <std::vector<Ipv4InterfaceContainer>> m_lowerIpv4Interfaces;

  std::vector <std::pair <NodeContainer, NodeContainer>> m_nodes;
  std::vector <std::pair <NetDeviceContainer, NetDeviceContainer>> m_nodesDevices;
};

} // namespace ns3

#endif /* POINT_TO_POINT_GFC_HELPER_H */

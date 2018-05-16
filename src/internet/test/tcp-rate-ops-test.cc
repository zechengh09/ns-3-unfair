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

#include "ns3/test.h"
#include "ns3/tcp-tx-item.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/tcp-rate-ops.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpRateOpsTestSuite");

/**
 * \ingroup internet-tests
 * \ingroup tests
 *
 * \brief The TcpRateLinux Test
 */
class TcpRateLinuxBasicTest : public TestCase
{
public:
  TcpRateLinuxBasicTest (uint32_t cWnd, SequenceNumber32 tailSeq, SequenceNumber32 nextTx,
                    uint32_t testCase, std::string testName);

private:
  virtual void DoRun (void);

  void SendSkb (TcpTxItem * skb);
  void SkbDelivered (TcpTxItem * skb);

  TcpRateLinux              m_rateOps;
  uint32_t                  m_cWnd;
  uint32_t                  m_inFlight;
  uint32_t                  m_segmentSize;
  uint32_t                  m_delivered;
  Time                      m_deliveredTime;
  SequenceNumber32          m_tailSeq;
  SequenceNumber32          m_nextTx;
  uint32_t                  m_testCase;
  std::vector <TcpTxItem *> m_skbs;
};

TcpRateLinuxBasicTest::TcpRateLinuxBasicTest (uint32_t cWnd, SequenceNumber32 tailSeq,
                                    SequenceNumber32 nextTx, uint32_t testCase,
                                    std::string testName)
  : TestCase (testName),
    m_cWnd (cWnd),
    m_inFlight (0),
    m_segmentSize (1),
    m_delivered (0),
    m_deliveredTime (Seconds (0)),
    m_tailSeq (tailSeq),
    m_nextTx (nextTx),
    m_testCase (testCase)
{
}

void
TcpRateLinuxBasicTest::DoRun ()
{
  for (uint8_t i = 0; i < 100; ++i)
    {
      m_skbs.push_back (new TcpTxItem ());
      Simulator::Schedule (Time (Seconds (i * 0.01)), &TcpRateLinuxBasicTest::SendSkb, this, m_skbs [i]);
    }

  for (uint8_t i = 0; i < 100; ++i)
    {
      Simulator::Schedule (Time (Seconds ((i + 1) * 0.1)), &TcpRateLinuxBasicTest::SkbDelivered, this, m_skbs [i]);
    }

  Simulator::Run ();
  Simulator::Destroy ();

  for (uint8_t i = 0; i < 100; ++i)
    {
      delete m_skbs[i];
    }
}

void
TcpRateLinuxBasicTest::SendSkb (TcpTxItem * skb)
{
  bool isStartOfTransmission = m_inFlight == 0;
  m_rateOps.CalculateAppLimited (m_cWnd, m_inFlight, m_segmentSize, m_tailSeq, m_nextTx);
  m_rateOps.SkbSent (skb, isStartOfTransmission);
  m_inFlight += skb->GetSeqSize ();

  NS_TEST_ASSERT_MSG_EQ (skb->GetRateInformation ().m_delivered, m_delivered, "SKB should have delivered equal to current value of total delivered");

  if (isStartOfTransmission)
    {
      NS_TEST_ASSERT_MSG_EQ (skb->GetRateInformation ().m_deliveredTime, Simulator::Now (), "SKB should have updated the delivered time to current value");
    }
  else
    {
      NS_TEST_ASSERT_MSG_EQ (skb->GetRateInformation ().m_deliveredTime, m_deliveredTime, "SKB should have updated the delivered time to current value");
    }
}

void
TcpRateLinuxBasicTest::SkbDelivered (TcpTxItem * skb)
{
  m_rateOps.SkbDelivered (skb);
  m_inFlight  -= skb->GetSeqSize ();
  m_delivered += skb->GetSeqSize ();
  m_deliveredTime = Simulator::Now ();

  NS_TEST_ASSERT_MSG_EQ (skb->GetRateInformation ().m_deliveredTime, Time::Max (), "SKB should have delivered time as Time::Max ()");

  if (m_testCase == 1)
    {
      NS_TEST_ASSERT_MSG_EQ (skb->GetRateInformation ().m_isAppLimited, false, "Socket should not be applimited");
    }
  else if (m_testCase == 2)
    {
      NS_TEST_ASSERT_MSG_EQ (skb->GetRateInformation ().m_isAppLimited, true, "Socket should be applimited");
    }
}

/**
 * \ingroup internet-test
 * \ingroup tests
 *
 * \brief the TestSuite for the TcpRateLinux test case
 */
class TcpRateOpsTestSuite : public TestSuite
{
public:
  TcpRateOpsTestSuite ()
    : TestSuite ("tcp-rate-ops", UNIT)
  {
    AddTestCase (new TcpRateLinuxBasicTest (1000, SequenceNumber32 (20), SequenceNumber32 (11), 1, "Testing SkbDelivered and SkbSent"), TestCase::QUICK);
    AddTestCase (new TcpRateLinuxBasicTest (1000, SequenceNumber32 (11), SequenceNumber32 (11), 2, "Testing SkbDelivered and SkbSent with app limited data"), TestCase::QUICK);
  }
};

static TcpRateOpsTestSuite  g_TcpRateOpsTestSuite; //!< Static variable for test initialization

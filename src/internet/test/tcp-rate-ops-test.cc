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
#include "ns3/log.h"
#include "ns3/tcp-rate-ops.h"
#include "tcp-general-test.h"
#include "ns3/config.h"
#include "tcp-error-model.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpRateOpsTestSuite");

class MimicCongControl;

/**
 * \ingroup internet-tests
 * \ingroup tests
 *
 * \brief The TcpRateLinux Basic Test
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
 * \brief Behaves as NewReno except HasCongControl returns true
 */
class MimicCongControl : public TcpNewReno
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  MimicCongControl ()
  {
  }

  virtual bool HasCongControl () const
  {
    return true;
  }

};

TypeId
MimicCongControl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MimicCongControl")
    .SetParent<TcpNewReno> ()
    .AddConstructor<MimicCongControl> ()
    .SetGroupName ("Internet")
  ;
  return tid;
}

/**
 * \ingroup internet-tests
 * \ingroup tests
 *
 * \brief The TcpRateLinux Test uses sender-receiver model to test its functionality.
 *        This test case uses the bytes inflight trace to check whether rate sample
 *        correctly sets the value of m_deliveredTime and m_firstSentTime. This is
 *        done using rate trace. Further, Using Rx trace, m_isDupAck is maintained to
 *        track duplicate acknowledgments. This, in turn, is used to see whether rate
 *        sample is updated properly (in case of SACK) or not (in case of non SACK).
 */
class TcpRateLinuxWithSocketsTest : public TcpGeneralTest
{
public:
  /**
   * \brief Constructor.
   * \param desc Description.
   * \param sackEnabled To use SACK or not
   * \param toDrop Packets to drop.
   */
  TcpRateLinuxWithSocketsTest (const std::string &desc, bool sackEnabled,
                               std::vector<uint32_t> &toDrop);

protected:
  /**
   * \brief Create and install the socket to install on the sender
   * \param node sender node pointer
   * \return the socket to be installed in the sender
   */
  virtual Ptr<TcpSocketMsgBase> CreateSenderSocket (Ptr<Node> node);

  /**
   * \brief Create a receiver error model.
   * \returns The receiver error model.
   */
  virtual Ptr<ErrorModel> CreateReceiverErrorModel ();

  /**
   * \brief Receive a packet.
   * \param p The packet.
   * \param h The TCP header.
   * \param who Who the socket belongs to (sender or receiver).
   */
  virtual void Rx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);

  /**
   * \brief Track the bytes in flight.
   * \param oldValue previous value.
   * \param newValue actual value.
   */
  virtual void BytesInFlightTrace (uint32_t oldValue, uint32_t newValue);

  /**
   * \brief Called when a packet is dropped.
   * \param ipH The IP header.
   * \param tcpH The TCP header.
   * \param p The packet.
   */
  void PktDropped (const Ipv4Header &ipH, const TcpHeader& tcpH, Ptr<const Packet> p);

  /**
   * \brief Configure the test.
   */
  void ConfigureEnvironment ();

  /**
   * \brief Do the final checks.
   */
  void FinalChecks ();

  /**
   * \brief Track the rate value of TcpRateLinux.
   * \param rate updated value of TcpRate.
   */
  virtual void RateUpdatedTrace (const TcpRateLinux::TcpRate &rate);

  /**
   * \brief Track the rate sample value of TcpRateLinux.
   * \param sample updated value of TcpRateSample.
   */
  virtual void RateSampleUpdatedTrace (const TcpRateLinux::TcpRateSample &sample);

private:
  Ptr<MimicCongControl>        m_congCtl;     //!< Dummy congestion control.
  bool                         m_sackEnabled; //!< Sack Variable
  std::vector<uint32_t>        m_toDrop;      //!< List of SequenceNumber to drop
  uint32_t                     m_bytesInFlight; //!< Bytes inflight
  SequenceNumber32             m_lastAckRecv {SequenceNumber32 (1)};   //!< Last ACK received.
  bool                         m_isDupAck;    //!< Whether ACK is DupAck
  TcpRateLinux::TcpRate        m_prevRate;    //!< Previous rate
  TcpRateLinux::TcpRateSample  m_prevRateSample; //!< Previous rate sample
};

TcpRateLinuxWithSocketsTest::TcpRateLinuxWithSocketsTest (const std::string &desc, bool sackEnabled,
                                                          std::vector<uint32_t> &toDrop)
  : TcpGeneralTest (desc),
    m_sackEnabled (sackEnabled),
    m_toDrop (toDrop)
{
}

Ptr<TcpSocketMsgBase>
TcpRateLinuxWithSocketsTest::CreateSenderSocket (Ptr<Node> node)
{
  Ptr<TcpSocketMsgBase> s = TcpGeneralTest::CreateSenderSocket (node);
  m_congCtl = CreateObject<MimicCongControl> ();
  s->SetCongestionControlAlgorithm (m_congCtl);
  return s;
}

void
TcpRateLinuxWithSocketsTest::ConfigureEnvironment ()
{
  TcpGeneralTest::ConfigureEnvironment ();
  SetAppPktCount (300);
  SetPropagationDelay (MilliSeconds (50));
  SetTransmitStart (Seconds (2.0));

  Config::SetDefault ("ns3::TcpSocketBase::Sack", BooleanValue (m_sackEnabled));
}

Ptr<ErrorModel>
TcpRateLinuxWithSocketsTest::CreateReceiverErrorModel ()
{
  Ptr<TcpSeqErrorModel> m_errorModel = CreateObject<TcpSeqErrorModel> ();
  for (std::vector<uint32_t>::iterator it = m_toDrop.begin (); it != m_toDrop.end (); ++it)
    {
      m_errorModel->AddSeqToKill (SequenceNumber32 (*it));
    }

  m_errorModel->SetDropCallback (MakeCallback (&TcpRateLinuxWithSocketsTest::PktDropped, this));

  return m_errorModel;
}

void
TcpRateLinuxWithSocketsTest::PktDropped (const Ipv4Header &ipH, const TcpHeader &tcpH,
                                  Ptr<const Packet> p)
{
  NS_LOG_DEBUG ("Drop seq= " << tcpH.GetSequenceNumber () << " size " << p->GetSize ());
}

void
TcpRateLinuxWithSocketsTest::Rx (const Ptr<const Packet> p, const TcpHeader &h, SocketWho who)
{
  if (who == SENDER)
    {
      if (h.GetAckNumber () == m_lastAckRecv
         && m_lastAckRecv != SequenceNumber32 (1)
         && (h.GetFlags () & TcpHeader::FIN) == 0)
        {
          m_isDupAck = true;
        }
      else
        {
          m_isDupAck = false;
          m_lastAckRecv = h.GetAckNumber ();
        }
    }
}

void
TcpRateLinuxWithSocketsTest::BytesInFlightTrace (uint32_t oldValue, uint32_t newValue)
{
  NS_UNUSED (oldValue);
  m_bytesInFlight = newValue;
}

void
TcpRateLinuxWithSocketsTest::RateUpdatedTrace (const TcpRateLinux::TcpRate &rate)
{
  NS_LOG_DEBUG ("Rate updated " << rate);
  if (m_bytesInFlight == 0)
    {
      NS_TEST_ASSERT_MSG_EQ (rate.m_firstSentTime, Simulator::Now (), "FirstSentTime should be current time when bytes inflight is zero");
      NS_TEST_ASSERT_MSG_EQ (rate.m_deliveredTime, Simulator::Now (), "Delivered time should be current time when bytes inflight is zero");
    }
  NS_TEST_ASSERT_MSG_GT_OR_EQ (rate.m_delivered, m_prevRate.m_delivered, "Total delivered should not be lesser than previous values");
  m_prevRate = rate;
}

void
TcpRateLinuxWithSocketsTest::RateSampleUpdatedTrace (const TcpRateLinux::TcpRateSample &sample)
{
  NS_LOG_DEBUG ("Rate sample updated " << sample);
  if (m_isDupAck)
    {
      if (!m_sackEnabled)
        {
          NS_TEST_ASSERT_MSG_EQ (m_prevRateSample, sample, "RateSample should not update due to DupAcks");
        }
      else
        {
          if (sample.m_ackedSacked == 0)
            {
              NS_TEST_ASSERT_MSG_EQ (m_prevRateSample, sample, "RateSample should not update as nothing is acked or sacked");
            }
        }
    }
  m_prevRateSample = sample;
}

void
TcpRateLinuxWithSocketsTest::FinalChecks ()
{
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

    std::vector<uint32_t> toDrop;
    toDrop.push_back (4001);
    AddTestCase (new TcpRateLinuxWithSocketsTest ("Checking Rate sample value without SACK, one drop", false, toDrop),
                 TestCase::QUICK);

    AddTestCase (new TcpRateLinuxWithSocketsTest ("Checking Rate sample value with SACK, one drop", true, toDrop),
                 TestCase::QUICK);
  }
};

static TcpRateOpsTestSuite  g_TcpRateOpsTestSuite; //!< Static variable for test initialization

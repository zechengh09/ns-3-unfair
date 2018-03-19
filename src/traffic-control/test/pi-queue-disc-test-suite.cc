/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 NITK Surathkal
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
 * Authors: Viyom Mittal <viyommittal@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"

using namespace ns3;

/**
 * \ingroup traffic-control-test
 * \defgroup traffic-control-test traffic-control module tests
 */


/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Pi Queue Disc Test Item
 */
class PiQueueDiscTestItem : public QueueDiscItem
{
public:
  /**
   * Constructor
   *
   * \param p packet
   * \param addr address
   */
  PiQueueDiscTestItem (Ptr<Packet> p, const Address & addr);
  virtual ~PiQueueDiscTestItem ();
  virtual void AddHeader (void);
  virtual bool Mark (void);

private:
  PiQueueDiscTestItem ();
  /**
   * \brief Copy constructor
   * Disable default implementation to avoid misuse
   */
  PiQueueDiscTestItem (const PiQueueDiscTestItem &);
  /**
   * \brief Assignment operator
   * \return this object
   * Disable default implementation to avoid misuse
   */
  PiQueueDiscTestItem &operator = (const PiQueueDiscTestItem &);
};

PiQueueDiscTestItem::PiQueueDiscTestItem (Ptr<Packet> p, const Address & addr)
  : QueueDiscItem (p, addr, 0)
{
}

PiQueueDiscTestItem::~PiQueueDiscTestItem ()
{
}

void
PiQueueDiscTestItem::AddHeader (void)
{
}

bool
PiQueueDiscTestItem::Mark (void)
{
  return false;
}

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Pi Queue Disc Test Case
 */
class PiQueueDiscTestCase : public TestCase
{
public:
  PiQueueDiscTestCase ();
  virtual void DoRun (void);
private:
  /**
   * Enqueue function
   * \param queue the queue disc
   * \param size the size
   * \param nPkt the number of packets
   */
  void Enqueue (Ptr<PiQueueDisc> queue, uint32_t size, uint32_t nPkt);
  /**
   * Enqueue with delay function
   * \param queue the queue disc
   * \param size the size
   * \param nPkt the number of packets
   */
  void EnqueueWithDelay (Ptr<PiQueueDisc> queue, uint32_t size, uint32_t nPkt);
  /**
   * Run Pi queue disc test function
   * \param mode the test mode
   */
  void RunPiTest (QueueSizeUnit mode);
};

PiQueueDiscTestCase::PiQueueDiscTestCase ()
  : TestCase ("Sanity check on the PI queue disc implementation")
{
}

void
PiQueueDiscTestCase::RunPiTest (QueueSizeUnit mode)
{
  uint32_t pktSize = 0;
  // 1 for packets; pktSize for bytes
  uint32_t modeSize = 1;
  uint32_t qSize = 8;
  Ptr<PiQueueDisc> queue = CreateObject<PiQueueDisc> ();

  // test 1: simple enqueue/dequeue with no drops
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxSize", QueueSizeValue (QueueSize (mode, qSize))),
                         true, "Verify that we can actually set the attribute MaxSize");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("A", DoubleValue (0.125)), true,
                         "Verify that we can actually set the attribute A");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("B", DoubleValue (1.25)), true,
                         "Verify that we can actually set the attribute B");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MeanPktSize", UintegerValue (1000)), true,
                         "Verify that we can actually set the attribute MeanPktSize");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueRef", DoubleValue (4)), true,
                         "Verify that we can actually set the attribute QueueRef");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("W", DoubleValue (125)), true,
                         "Verify that we can actually set the attribute W");

  Address dest;

  if (mode == QueueSizeUnit::BYTES)
    {
      pktSize = 500;
      modeSize = pktSize;
      queue->SetAttribute ("MaxSize", QueueSizeValue (QueueSize (mode, qSize * modeSize)));
    }

  Ptr<Packet> p1, p2, p3, p4, p5, p6, p7, p8;
  p1 = Create<Packet> (pktSize);
  p2 = Create<Packet> (pktSize);
  p3 = Create<Packet> (pktSize);
  p4 = Create<Packet> (pktSize);
  p5 = Create<Packet> (pktSize);
  p6 = Create<Packet> (pktSize);
  p7 = Create<Packet> (pktSize);
  p8 = Create<Packet> (pktSize);

  queue->Initialize ();
  NS_TEST_EXPECT_MSG_EQ (queue->GetCurrentSize ().GetValue (), 0 * modeSize, "There should be no packets in there");
  queue->Enqueue (Create<PiQueueDiscTestItem> (p1, dest));
  NS_TEST_EXPECT_MSG_EQ (queue->GetCurrentSize ().GetValue (), 1 * modeSize, "There should be one packet in there");
  queue->Enqueue (Create<PiQueueDiscTestItem> (p2, dest));
  NS_TEST_EXPECT_MSG_EQ (queue->GetCurrentSize ().GetValue (), 2 * modeSize, "There should be two packets in there");
  queue->Enqueue (Create<PiQueueDiscTestItem> (p3, dest));
  queue->Enqueue (Create<PiQueueDiscTestItem> (p4, dest));
  queue->Enqueue (Create<PiQueueDiscTestItem> (p5, dest));
  queue->Enqueue (Create<PiQueueDiscTestItem> (p6, dest));
  queue->Enqueue (Create<PiQueueDiscTestItem> (p7, dest));
  queue->Enqueue (Create<PiQueueDiscTestItem> (p8, dest));
  NS_TEST_EXPECT_MSG_EQ (queue->GetCurrentSize ().GetValue (), 8 * modeSize, "There should be eight packets in there");

  Ptr<QueueDiscItem> item;

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the first packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetCurrentSize ().GetValue (), 7 * modeSize, "There should be seven packets in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p1->GetUid (), "was this the first packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the second packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetCurrentSize ().GetValue (), 6 * modeSize, "There should be six packet in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p2->GetUid (), "Was this the second packet ?");

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item != 0), true, "I want to remove the third packet");
  NS_TEST_EXPECT_MSG_EQ (queue->GetCurrentSize ().GetValue (), 5 * modeSize, "There should be five packets in there");
  NS_TEST_EXPECT_MSG_EQ (item->GetPacket ()->GetUid (), p3->GetUid (), "Was this the third packet ?");

  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();
  item = queue->Dequeue ();

  item = queue->Dequeue ();
  NS_TEST_EXPECT_MSG_EQ ((item == 0), true, "There are really no packets in there");

  struct d
  {
    uint32_t test2;
    uint32_t test3;
  } drop;


  // test 2: default values for PI parameters
  queue = CreateObject<PiQueueDisc> ();
  qSize = 300 * modeSize;
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxSize", QueueSizeValue (QueueSize (mode, qSize))),
                         true, "Verify that we can actually set the attribute MaxSize");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueRef", DoubleValue (50)), true,
                         "Verify that we can actually set the attribute QueueRef");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("W", DoubleValue (170)), true,
                         "Verify that we can actually set the attribute W");
  queue->Initialize ();
  EnqueueWithDelay (queue, pktSize, 300);
  Simulator::Stop (Seconds (40));
  Simulator::Run ();
  QueueDisc::Stats st = queue->GetStats ();
  drop.test2 = st.GetNDroppedPackets (PiQueueDisc::UNFORCED_DROP);
  NS_TEST_EXPECT_MSG_NE (drop.test2, 0, "There should be some unforced drops");


  // test 3: high value of W
  queue = CreateObject<PiQueueDisc> ();
  qSize = 300 * modeSize;
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MeanPktSize", UintegerValue (pktSize)), true,
                         "Verify that we can actually set the attribute MeanPktSize");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("QueueRef", DoubleValue (50)), true,
                         "Verify that we can actually set the attribute QueueRef");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("MaxSize", QueueSizeValue (QueueSize (mode, qSize))),
                         true, "Verify that we can actually set the attribute MaxSize");
  NS_TEST_EXPECT_MSG_EQ (queue->SetAttributeFailSafe ("W", DoubleValue (500)), true,
                         "Verify that we can actually set the attribute W");
  queue->Initialize ();
  EnqueueWithDelay (queue, pktSize, 300);
  Simulator::Stop (Seconds (40));
  Simulator::Run ();
  st = queue->GetStats ();
  drop.test3 = st.GetNDroppedPackets (PiQueueDisc::UNFORCED_DROP);
  NS_TEST_EXPECT_MSG_GT (drop.test3, drop.test2, "Test 3 should have more unforced drops than Test 2");
}

void
PiQueueDiscTestCase::Enqueue (Ptr<PiQueueDisc> queue, uint32_t size, uint32_t nPkt)
{
  Address dest;
  for (uint32_t i = 0; i < nPkt; i++)
    {
      queue->Enqueue (Create<PiQueueDiscTestItem> (Create<Packet> (size), dest));
    }
}

void
PiQueueDiscTestCase::EnqueueWithDelay (Ptr<PiQueueDisc> queue, uint32_t size, uint32_t nPkt)
{
  Address dest;
  double delay = 0.1;
  for (uint32_t i = 0; i < nPkt; i++)
    {
      Simulator::Schedule (Time (Seconds ((i + 1) * delay)), &PiQueueDiscTestCase::Enqueue,this,queue,size, 1);
    }

}

void
PiQueueDiscTestCase::DoRun (void)
{
  RunPiTest (QueueSizeUnit::PACKETS);
  RunPiTest (QueueSizeUnit::BYTES);
  Simulator::Destroy ();
}

static class PiQueueDiscTestSuite : public TestSuite
{
public:
  PiQueueDiscTestSuite ()
    : TestSuite ("pi-queue-disc", UNIT)
  {
    AddTestCase (new PiQueueDiscTestCase (), TestCase::QUICK);
  }
} g_piQueueTestSuite;

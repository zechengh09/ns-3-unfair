
#pragma once

#include "ns3/type-id.h"
#include "ns3/tcp-header.h"
#include "ns3/uinteger.h"

namespace ns3 {

class BbrTag : public Tag {
 public:
  BbrTag () = default;
  BbrTag (Time time, bool _isBbr) : sndTime (time), isBbr (_isBbr) {}

  static TypeId
    GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::BbrTag")
      .SetParent<Tag> ()
      .AddConstructor<BbrTag> ()
      ;
    return tid;
  }

  TypeId
    GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }

  uint32_t
    GetSerializedSize (void) const
  {
    return sizeof (Time);
  }

  void
    Serialize (TagBuffer i) const
  {
    i.WriteU64 (*(uint64_t*)&sndTime);
  }

  void
    Deserialize (TagBuffer i)
  {
    auto tmp = i.ReadU64 ();
    sndTime = *(Time*)&tmp;
  }

  void
    Print (std::ostream &os) const
  {
    os << "v=" << sndTime;
  }

  Time sndTime;
  bool isBbr;
};

} // namespace ns3

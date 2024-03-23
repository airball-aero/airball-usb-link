#ifndef AIRBALL_TELEMETRY_TELEMETRY_H
#define AIRBALL_TELEMETRY_TELEMETRY_H

#include <cstdint>

#include "AbstractSerialLink.h"

namespace airball {

class Telemetry {
public:
  static const uint16_t kMessageDomainLocal = 0;
  static const uint16_t kMessageDomainCanBus = 1;

  static const uint16_t kLocalMessageIdButtonPress = 0;
  static const uint16_t kLocalMessageIdButtonRelease = 1;
  static const uint16_t kLocalMessageIdKnobIncrement = 2;
  static const uint16_t kLocalMessageIdKnobDecrement = 3;
  static const uint16_t kLocalMessageIdIdentifyLeaderFollower = 4;
  static const uint16_t kLocalMessageIdThisNodeIsLeader = 5;
  static const uint16_t kLocalMessageIdThisNodeIsFollower = 6;

#pragma pack(push, 1)
  struct Message {
    uint16_t domain;
    uint16_t id;
    uint8_t data[8];
  };
#pragma pack(pop)

  Telemetry(AbstractSerialLink<sizeof(Message)>* link);

  // Receive the next Message. Returns false if no message is available.
  bool recv(Message* m);

  // Send a Message. This is broadcast to everyone on the network.
  void send(Message s);

private:
  AbstractSerialLink<sizeof(Message)>* link_;
};

}  // namespace airball

#endif // AIRBALL_TELEMETRY_TELEMETRY_H

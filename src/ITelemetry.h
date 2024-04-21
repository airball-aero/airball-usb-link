#ifndef AIRBALL_TELEMETRY_I_TELEMETRY_H
#define AIRBALL_TELEMETRY_I_TELEMETRY_H

#include <cstdint>

namespace airball {

class ITelemetry {
public:
  static const uint16_t kMessageDomainLocal = 1;
  static const uint16_t kMessageDomainCanBus = 2;
  static const uint16_t kMessageDomainSync = 3;

  static const uint16_t kLocalMessageIdButtonPress = 1;
  static const uint16_t kLocalMessageIdButtonRelease = 2;
  static const uint16_t kLocalMessageIdKnobIncrement = 3;
  static const uint16_t kLocalMessageIdKnobDecrement = 4;
  static const uint16_t kLocalMessageIdIdentifyLeaderFollower = 5;
  static const uint16_t kLocalMessageIdThisNodeIsLeader = 6;
  static const uint16_t kLocalMessageIdThisNodeIsFollower = 7;

#pragma pack(push, 1)
  struct Message {
    uint16_t domain;
    uint16_t id;
    uint8_t data[8];
  };
#pragma pack(pop)

   // Receive the next Message. Returns false if no message is available.
  virtual bool recv(Message* m) = 0;

  // Send a Message. This is broadcast to everyone on the network.
  virtual void send(Message s) = 0;
};

}  // namespace airball

#endif // AIRBALL_TELEMETRY_I_TELEMETRY_H

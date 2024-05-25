#ifndef AIRBALL_TELEMETRY_I_TELEMETRY_H
#define AIRBALL_TELEMETRY_I_TELEMETRY_H

#include <cstdint>

namespace airball {

class ITelemetry {
public:
  // A Message is basically the "Good Parts" of a CANBus frame with an extended
  // ID. Only the least-significant 29 bits of the ID are effective. We use this
  // structure in all the messages passed around by Airball componens, so that
  // any message may or may not easily be carried over a CANBus link, depending
  // on the physical network topology.
#pragma pack(push, 1)
  struct Message {
    uint32_t id;
    uint8_t data[8];
  };
#pragma pack(pop)

  // Message IDs are organized into "pages". Each page contains at most
  // 1024 codes, each of which is at most 10 bits long. The page number is
  // at most 19 bits long. Putting these two together gives us a 29-bit
  // CANBus extended ID.
  //
  // The idea behind this separation is that each page represents the set
  // of messages of one subsystem. As long as the subsystem reserves a
  // page here, portions of that subsystem can independently manage their
  // codes within that page.

  // Given a full 29-bit ID, return the page number.
  static constexpr uint32_t page(uint32_t id) {
    return id >> 10;
  }

  // Given a full 29-bit ID, return the code within the page.
  static constexpr uint32_t code(uint32_t id) {
    return id & 0x3ff;
  }
  
  // Given a page number and a code, return a full 29-bit ID.
  static constexpr uint32_t id(uint32_t page, uint32_t code) {
    return (page << 10) + code;
  }

  // Page zero is reserved.
  static const uint32_t kPageReserved = 0;

  static const uint32_t kPageAirdata = 1;  // Airdata from sensors, and commands to airdata sensors
  static const uint32_t kPageHumanInterface = 2;  // Messages to and from human interface devices
  static const uint32_t kPageDisplaySettings = 3;  // Messages to transfer display system settings

  // Codes in kPageHumanInterface. These are kind of a special case, because they are
  // transferred between the same entities that share the airball-usb-link Git repo. Strictly
  // speaking, these codes should be defined in a separate "human interface" Git repo.
  static const uint32_t kHumanInterfaceButtonPress = 1;
  static const uint32_t kHumanInterfaceButtonRelease = 2;
  static const uint32_t kHumanInterfaceKnobIncrement = 3;
  static const uint32_t kHumanInterfaceKnobDecrement = 4;
  static const uint32_t kHumanInterfaceIdentifyLeaderFollower = 5;
  static const uint32_t kHumanInterfaceThisNodeIsLeader = 6;
  static const uint32_t kHumanInterfaceThisNodeIsFollower = 7;

  // Receive the next Message. Returns false if no message is available.
  virtual bool recv(Message* m) = 0;

  // Send a Message. This is broadcast to everyone on the network.
  virtual void send(Message s) = 0;
};

}  // namespace airball

#endif // AIRBALL_TELEMETRY_I_TELEMETRY_H

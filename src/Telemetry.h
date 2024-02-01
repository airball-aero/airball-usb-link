#ifndef AIRBALL_TELEMETRY_TELEMETRY_H
#define AIRBALL_TELEMETRY_TELEMETRY_H

#include <cstdint>

#include "AbstractSerialLink.h"

namespace airball {

class Telemetry {
public:
  struct Message {
    uint16_t id;
    uint8_t data[8];
  };

  Telemetry(AbstractSerialLink<sizeof(Message)>* link);

  // Receive the next Message. Blocks until a Message is received.
  Message recv();

  // Send a Message. This is broadcast to everyone on the network.
  void send(Message s);

private:
  AbstractSerialLink<sizeof(Message)>* link_;
};

}  // namespace airball

#endif // AIRBALL_TELEMETRY_TELEMETRY_H

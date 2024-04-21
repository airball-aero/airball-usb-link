#ifndef AIRBALL_TELEMETRY_SERIAL_TELEMETRY_H
#define AIRBALL_TELEMETRY_SERIAL_TELEMETRY_H

#include <cstdint>

#include "AbstractSerialLink.h"
#include "ITelemetry.h"

namespace airball {

class SerialTelemetry : public ITelemetry {
public:
  SerialTelemetry(AbstractSerialLink<sizeof(Message)>* link);

  // Receive the next Message. Returns false if no message is available.
  bool recv(Message* m) override;

  // Send a Message. This is broadcast to everyone on the network.
  void send(Message s) override;

private:
  AbstractSerialLink<sizeof(Message)>* link_;
};

}  // namespace airball

#endif // AIRBALL_TELEMETRY_SERIAL_TELEMETRY_H

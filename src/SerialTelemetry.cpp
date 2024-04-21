#include "SerialTelemetry.h"

namespace airball {

SerialTelemetry::SerialTelemetry(AbstractSerialLink<sizeof(Message)>* link)
    : link_(link) {}

bool SerialTelemetry::recv(Message* m) {
  return link_->recv((uint8_t*) m);
}

void SerialTelemetry::send(Message s) {
  link_->send((const uint8_t*) &s);
}

} // namespace airball


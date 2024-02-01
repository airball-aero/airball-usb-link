#include "Telemetry.h"

namespace airball {

Telemetry::Telemetry(AbstractSerialLink<sizeof(Message)>* link)
    : link_(link) {}

Telemetry::Message Telemetry::recv() {
  Message m { 0 };
  link_->recv((uint8_t*) &m);
  return m;
}

void Telemetry::send(Message s) {
  link_->send((const uint8_t*) &s);
}

} // namespace airball


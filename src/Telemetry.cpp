#include "Telemetry.h"

namespace airball {

Telemetry::Telemetry(AbstractSerialLink<sizeof(Message)>* link)
    : link_(link) {}

bool Telemetry::recv(Message* m) {
  return link_->recv((uint8_t*) m);
}

void Telemetry::send(Message s) {
  link_->send((const uint8_t*) &s);
}

} // namespace airball


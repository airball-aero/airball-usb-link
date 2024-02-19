#include "Telemetry.h"

namespace airball {

Telemetry::Telemetry(AbstractSerialLink<sizeof(Message)>* link)
    : link_(link) {}

Telemetry::Message Telemetry::recv() {
  std::unique_lock lock(recv_mu_);
  Message m { 0 };
  link_->recv((uint8_t*) &m);
  return m;
}

void Telemetry::send(Message s) {
  std::unique_lock lock(send_mu_);
  link_->send((const uint8_t*) &s);
}

} // namespace airball


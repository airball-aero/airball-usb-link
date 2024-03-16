#ifndef SRC_ABSTRACTSERIALLINK_H
#define SRC_ABSTRACTSERIALLINK_H

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <optional>

namespace airball {

template <size_t PDU_LEN>
class AbstractSerialLink {
public:
  AbstractSerialLink()
      : recv_next_pdu_(PDU_LEN)
  {
    state_ = IN_PDU;
    recv_next_pdu_.clear();
  }

  void send(const uint8_t *buf) {
    for (int i = 0; i < PDU_LEN; i++) {
      if (buf[i] == kEndMarker) {
        // Escape sequence
        push(kEndMarker);
      }
      push(buf[i]);
    }
    push(kEndMarker);
  }

  bool recv(uint8_t *buf) {
    std::optional<uint8_t> unget;
    while (true) {
      uint8_t c;
      if (unget.has_value()) {
        c = unget.value();
        unget.reset();
      } else {
        if (!pull(&c)) {
          return false;
        }
      }
      switch (state_) {
        case IN_PDU:
          switch (c) {
            case kEndMarker:
              state_ = IN_PDU_GOT_END;
              break;
            default:
              recv_next_pdu_.push_back(c);
              state_ = recv_next_pdu_.size() == PDU_LEN ?
                       DONE_PDU_AWAITING_END : IN_PDU;
              break;
          }
          break;
        case IN_PDU_GOT_END:
          switch (c) {
            case kEndMarker:
              recv_next_pdu_.push_back(c);
              state_ = recv_next_pdu_.size() == PDU_LEN ?
                       DONE_PDU_AWAITING_END : IN_PDU;
              break;
            default:
              // Error
              recv_next_pdu_.clear();
              state_ = IN_PDU;
              unget = c;
              break;
          }
          break;
        case DONE_PDU_AWAITING_END:
          switch (c) {
            case kEndMarker:
              memcpy(buf, recv_next_pdu_.data(), PDU_LEN);
              recv_next_pdu_.clear();
              state_ = IN_PDU;
              return true;
            default:
              // Error
              recv_next_pdu_.clear();
              state_ = IN_PDU;
              break;
          }
          break;
      }
    }
  }

protected:
  virtual void push(uint8_t c) = 0;

  virtual bool pull(uint8_t* c) = 0;

private:
  static const uint8_t kEndMarker = 0xff;

  enum State {
    IN_PDU = 0,
    IN_PDU_GOT_END = 1,
    DONE_PDU_AWAITING_END = 2,
  };

  std::vector<uint8_t> recv_next_pdu_;
  State state_;
};

} // namespace airball

#endif //SRC_ABSTRACTSERIALLINK_H

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
    push(sumPdu(buf));
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
          if (c == kEndMarker) {
            state_ = IN_PDU_GOT_END;
          } else {
            recv_next_pdu_.push_back(c);
            state_ = recv_next_pdu_.size() == PDU_LEN ?
                     DONE_PDU_AWAITING_SUM : IN_PDU;
          }
          break;
        case IN_PDU_GOT_END:
          if (c == kEndMarker) {
            recv_next_pdu_.push_back(c);
            state_ = recv_next_pdu_.size() == PDU_LEN ?
                     DONE_PDU_AWAITING_SUM : IN_PDU;
          } else {
            // Error
            recv_next_pdu_.clear();
            state_ = IN_PDU;
            unget = c;
          }
          break;
        case DONE_PDU_AWAITING_SUM:
          if (c == sumPdu(recv_next_pdu_.data())) {
            state_ = DONE_PDU_AWAITING_END;
          } else {
            // Error
            recv_next_pdu_.clear();
            state_ = WRONG_SUM_AWAITING_END;
          }
          break;
        case WRONG_SUM_AWAITING_END:
          if (c == kEndMarker) {
            state_ = IN_PDU;
          } else {
            // Something else -- let's try to start afresh
            unget = c;
            state_ = IN_PDU;
          }
          break;
        case DONE_PDU_AWAITING_END:
          if (c == kEndMarker) {
            memcpy(buf, recv_next_pdu_.data(), PDU_LEN);
            recv_next_pdu_.clear();
            state_ = IN_PDU;
            return true;
          } else {
            // Error
            recv_next_pdu_.clear();
            state_ = IN_PDU;
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
    DONE_PDU_AWAITING_SUM = 2,
    WRONG_SUM_AWAITING_END = 3,
    DONE_PDU_AWAITING_END = 4,
  };

  uint8_t sumPdu(const uint8_t* v) {
    uint8_t s = 0;
    for (int i = 0; i < PDU_LEN; i++) {
      s ^= v[i];
    }
    return s;
  }

  std::vector<uint8_t> recv_next_pdu_;
  State state_;
};

} // namespace airball

#endif //SRC_ABSTRACTSERIALLINK_H

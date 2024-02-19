#ifndef SRC_ABSTRACTSERIALLINK_H
#define SRC_ABSTRACTSERIALLINK_H

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>

#include "Base16Encoding.h"

namespace airball {

template <size_t PDU_LEN>
class AbstractSerialLink {
public:
  AbstractSerialLink()
      : recv_next_byte_encoded_(2),
        recv_next_line_(PDU_LEN)
  {
    recv_next_byte_encoded_.clear();
    recv_next_line_.clear();
  }

  void send(const uint8_t *buf) {
    for (int i = 0; i < PDU_LEN; i++) {
      push2(buf[i]);
    }
    push(kNewline);
  }

  bool recv(uint8_t *buf) {
    while (true) {
      uint8_t c;
      if (!pull(&c)) {
        return false;
      }
      if (c == kNewline) {
        bool good_line = recv_next_line_.size() == PDU_LEN;
        if (good_line) {
          memcpy(buf, recv_next_line_.data(), PDU_LEN);
        }
        recv_next_byte_encoded_.clear();
        recv_next_line_.clear();
        if (good_line) {
          return true;
        }
        continue;
      }
      if (recv_next_line_.size() == PDU_LEN) {
        recv_next_line_.clear();
        recv_next_byte_encoded_.clear();
        continue;
      }
      recv_next_byte_encoded_.push_back(c);
      if (recv_next_byte_encoded_.size() != 2) {
        continue;
      }
      uint8_t new_byte;
      if (!Base16Encoding::decode(recv_next_byte_encoded_.data(), 2, &new_byte)) {
        recv_next_line_.clear();
      } else {
        recv_next_line_.push_back(new_byte);
      }
      recv_next_byte_encoded_.clear();

    }
  }

protected:
  virtual void push(uint8_t c) = 0;

  virtual bool pull(uint8_t* c) = 0;

private:
  static const size_t kNewline = '\n';

  std::vector<uint8_t> recv_next_byte_encoded_;
  std::vector<uint8_t> recv_next_line_;

  void push2(uint8_t c) {
    uint8_t encoded[2];
    Base16Encoding::encode(&c, 1, encoded);
    push(encoded[0]);
    push(encoded[1]);
  }
};

} // namespace airball

#endif //SRC_ABSTRACTSERIALLINK_H

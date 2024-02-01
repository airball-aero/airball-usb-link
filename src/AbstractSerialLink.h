#ifndef SRC_ABSTRACTSERIALLINK_H
#define SRC_ABSTRACTSERIALLINK_H

#include <cstdint>
#include <cstddef>
#include <cstring>

#include "Base16Encoding.h"

namespace airball {

template <size_t PDU_LEN>
class AbstractSerialLink {
public:
  void send(const uint8_t *buf) {
    for (int i = 0; i < PDU_LEN; i++) {
      push2(buf[i]);
    }
    push(kNewline);
  }

  void recv(uint8_t *buf) {
    uint8_t tmp[PDU_LEN];
    uint8_t chars[2];
    while (true) {
      for (int i = 0;; i++) {
        if (i == PDU_LEN) {
          if (pull() == kNewline) {
            memcpy(buf, tmp, PDU_LEN);
            return;
          }
          break;
        }
        if ((chars[0] = pull()) == kNewline) {
          break;
        }
        if ((chars[1] = pull()) == kNewline) {
          break;
        }
        Base16Encoding::decode(chars, 2, &tmp[i]);
      }
    }
  }

protected:
  virtual void push(uint8_t c) = 0;

  virtual uint8_t pull() = 0;

private:
  static const size_t kNewline = '\n';

  void push2(uint8_t c) {
    uint8_t encoded[2];
    Base16Encoding::encode(&c, 1, encoded);
    push(encoded[0]);
    push(encoded[1]);
  }
};

} // namespace airball

#endif //SRC_ABSTRACTSERIALLINK_H

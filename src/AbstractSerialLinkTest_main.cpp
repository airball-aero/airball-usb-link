#include <iostream>
#include <string>

#include "AbstractSerialLink.h"

constexpr size_t kLen = 8;
constexpr char kEndMarker = (char) 0xff;

char sumStr(const std::string& str) {
  uint8_t s = 0;
  for (char c : str) {
    s ^= ((uint8_t) c);
  }
  return (char) s;
}

class TestSerialLink : public airball::AbstractSerialLink<kLen> {
public:
  std::string pushed;
  std::string toPull;

protected:
  void push(uint8_t c) override {
    pushed.append((const char*) &c, 1);
  }

  bool pull(uint8_t* c) override {
    if (toPull.empty()) {
      return false;
    }
    uint8_t p = toPull[0];
    toPull.erase(0, 1);
    if (p == 'X') {
      return false;
    }
    *c = p;
    return true;
  }
};

void testSendSimple() {
  TestSerialLink tsl;
  const char* buf = "abcd0123";
  tsl.send((uint8_t*) buf);
  std::string want = {
      'a', 'b', 'c', 'd', '0', '1', '2', '3',
      sumStr("abcd0123"), kEndMarker,
  };
  if (tsl.pushed != want) {
    std::cerr << "testSendSimple failed; got " << tsl.pushed << " want " << want << std::endl;
  }
}

void testRecvSimple() {
  TestSerialLink tsl;
  tsl.toPull = {
    'a', 'b', 'c', 'd', '0', '1', '2', '3',
    sumStr("abcd0123"), kEndMarker,
  };
  uint8_t* buf[kLen];
  while (true) {
    if (tsl.recv((uint8_t *) buf)) {
      break;
    }
  }
  std::string recvd((char*) buf, kLen);
  std::string want = "abcd0123";
  if (recvd != want) {
    std::cerr << "testRecvSimple failed; got " << recvd << " want " << want << std::endl;
  }
}

void testRecvError() {
  TestSerialLink tsl;
  tsl.toPull = std::string({

      // Truncated PDU
      'a', 'b', 'c', kEndMarker,

      // Run-on PDU
      'a', 'b', 'c', 'd', '0', '1', '2', '3', '4', kEndMarker,

      // Valid PDU
      'a', 'b', 'c', 'd', '0', '1', '2', '3', sumStr("abcd0123"), kEndMarker,

      // Invalid checksum but otherwise well-formed
      'a', 'b', 'c', 'd', '0', '1', '2', '3', 0, kEndMarker,

      // PDU with nothing but end markers
      kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker,
      kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker,
      sumStr({
        kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker,
        kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker, kEndMarker,
      }),
      kEndMarker,

      // Valid PDU with end marker at end
      'a', 'b', 'c', 'd', '0', '1', '2', kEndMarker, kEndMarker,
      sumStr({
        'a', 'b', 'c', 'd', '0', '1', '2', kEndMarker,
      }),
      kEndMarker,

      // Valid PDU with end marker in middle
      'a', 'b', 'c', 'd', '0', '1', kEndMarker, kEndMarker, '3',
      sumStr({
        'a', 'b', 'c', 'd', '0', '1', kEndMarker, '3',
      }),
      kEndMarker,
  });

  auto fetch = [&tsl]() -> std::string {
    uint8_t* buf[kLen];
    while (true) {
      if (tsl.recv((uint8_t *) buf)) {
        break;
      }
    }
    return {(char*) buf, kLen};
  };

  std::string s;

  s = fetch();
  if (s != "abcd0123") {
    std::cerr << "testRecvError failed; got " << s << std::endl;
  }

  s = fetch();
  if (s != std::string({ kEndMarker, kEndMarker, kEndMarker, kEndMarker,
                         kEndMarker, kEndMarker, kEndMarker, kEndMarker })) {
    std::cerr << "testRecvError failed; got " << s << std::endl;
  }

  s = fetch();
  if (s != std::string({ 'a', 'b', 'c', 'd', '0', '1', '2', kEndMarker })) {
    std::cerr << "testRecvError failed; got " << s << std::endl;
  }

  s = fetch();
  if (s != std::string({ 'a', 'b', 'c', 'd', '0', '1', kEndMarker, '3' })) {
    std::cerr << "testRecvError failed; got " << s << std::endl;
  }
}

 int main(int arg, char** argv) {
  testSendSimple();
  testRecvSimple();
  testRecvError();
}
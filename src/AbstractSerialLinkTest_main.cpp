#include <iostream>
#include <string>

#include "AbstractSerialLink.h"

constexpr size_t kLen = 8;

class TestSerialLink : public airball::AbstractSerialLink<kLen> {
public:
  std::string pushed;
  std::string toPull;

protected:
  void push(uint8_t c) override {
    pushed.append((const char*) &c, 1);
  }

  uint8_t pull() override {
    uint8_t c = toPull[0];
    toPull.erase(0, 1);
    return c;
  }
};

void testSendSimple() {
  TestSerialLink tsl;
  const char* buf = "abcd0123";
  tsl.send((uint8_t*) buf);
  if (tsl.pushed != "6162636430313233\n") {
    std::cerr << "testSendSimple failed; pushed was " << tsl.pushed << std::endl;
  }
}

void testRecvSimple() {
  TestSerialLink tsl;
  tsl.toPull = "6162636430313233\n";
  uint8_t* buf[kLen];
  tsl.recv((uint8_t*) buf);
  std::string recvd((char*) buf, kLen);
  if (recvd != "abcd0123") {
    std::cerr << "testRecvSimple failed; received was " << recvd << std::endl;
  }
}

void testRecvError() {
  TestSerialLink tsl;
  tsl.toPull = "abc\na\nx\nabcdefghijklmnopqrstuvwxyz\n\n\n\n6162636430313233\n";
  uint8_t* buf[kLen];
  tsl.recv((uint8_t*) buf);
  std::string recvd((char*) buf, kLen);
  if (recvd != "abcd0123") {
    std::cerr << "testRecvError failed; received was " << recvd << std::endl;
  }
}

int main(int arg, char** argv) {
  testSendSimple();
  testRecvSimple();
  testRecvError();
}
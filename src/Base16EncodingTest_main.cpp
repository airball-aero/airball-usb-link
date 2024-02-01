#include <iostream>

#include "Base16Encoding.h"

void testRoundTrip() {
  std::string decoded;
  for (int i = 0; i < 0xff; i++) {
    decoded.push_back(i);
  }
  std::string encoded(decoded.size() * 2, 0);
  airball::Base16Encoding::encode((const uint8_t*) decoded.data(), decoded.size(), (uint8_t*) encoded.data());
  std::string newly_decoded(decoded.size(), 0);
  airball::Base16Encoding::decode((const uint8_t*) encoded.data(), encoded.size(), (uint8_t*) newly_decoded.data());
  if (decoded != newly_decoded) {
    std::cerr << "FAIL, decode round trip" << std::endl;
  }
}

void testKnownEncoding() {
  std::string decoded("abcdef");
  std::string encoded(decoded.size() * 2, '0');
  airball::Base16Encoding::encode((const uint8_t*) decoded.data(), decoded.size(), (uint8_t*) encoded.data());
  if (encoded != "616263646566") {
    std::cerr << "FAIL, encode known " << encoded << std::endl;
  }
}

void testKnownDecoding() {
  std::string encoded("616263646566");
  std::string decoded(encoded.size() / 2, '0');
  airball::Base16Encoding::decode((const uint8_t*) encoded.data(), encoded.size(), (uint8_t*) decoded.data());
  if (decoded != "abcdef") {
    std::cerr << "FAIL, decode known " << decoded << std::endl;
  }
}

int main(int arg, char** argv) {
  testRoundTrip();
  testKnownEncoding();
  testKnownDecoding();
}
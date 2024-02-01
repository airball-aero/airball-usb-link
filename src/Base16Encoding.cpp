#include "Base16Encoding.h"

#include <unordered_map>

namespace airball {

typedef std::unordered_map<uint8_t, uint8_t> CharToCharMap;

uint8_t kAsciiChar_0 = 0x30;
uint8_t kAsciiChar_a = 0x61;

std::pair<CharToCharMap, CharToCharMap> makeMaps() {
  CharToCharMap nyblToChar;
  CharToCharMap charToNybl;
  for (int i = 0; i < 0x0a; i++) {
    nyblToChar.insert({i, kAsciiChar_0 + i});
    charToNybl.insert({kAsciiChar_0 + i, i});
  }
  for (int i = 0x0a; i < 0x10; i++) {
    nyblToChar.insert({i, kAsciiChar_a + i - 0x0a});
    charToNybl.insert({ kAsciiChar_a + i - 0x0a, i});
  }
  return { nyblToChar, charToNybl };
}

std::pair<CharToCharMap, CharToCharMap> maps = makeMaps();

const CharToCharMap kNyblToChar = maps.first;
const CharToCharMap kCharToNybl = maps.second;

uint8_t leastSignificantNybl(uint8_t byte) {
  return byte & 0x0f;
}

uint8_t mostSignificantNybl(uint8_t byte) {
  return (byte >> 4) & 0x0f;
}

uint8_t byte(uint8_t msNybl, uint8_t lsNybl) {
  return (msNybl << 4) + lsNybl;
}

uint8_t safeAt(const CharToCharMap& map, uint8_t c) {
  try {
    return map.at(c);
  } catch (std::exception& e) {
    return 0;
  }
}

void Base16Encoding::encode(const uint8_t* decoded, size_t len, uint8_t* encoded) {
  for (int i = 0; i < len; i++) {
    encoded[2 * i] = safeAt(kNyblToChar, mostSignificantNybl(decoded[i]));
    encoded[2 * i + 1] = safeAt(kNyblToChar, leastSignificantNybl(decoded[i]));
  }
}

void Base16Encoding::decode(const uint8_t* encoded, size_t len, uint8_t* decoded) {
  for (int i = 0; i < len / 2; i++) {
    decoded[i] = byte(
        safeAt(kCharToNybl, encoded[2 * i]),
        safeAt(kCharToNybl, encoded[2 * i + 1]));
  }
}

} // namespace airball
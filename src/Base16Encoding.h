#ifndef SRC_AIRBALL_UTIL_BASE16ENCODING_H
#define SRC_AIRBALL_UTIL_BASE16ENCODING_H

#include <cstdint>
#include <cstddef>

namespace airball {

class Base16Encoding {
public:
  // Encode the string "decoded". "encoded" must point to a buffer containing at least
  // (2 * len) characters.
  static void encode(const uint8_t* decoded, size_t len, uint8_t* encoded);

  // Decode the string "encoded". "decoded" must point to a buffer containing at least
  // (len / 2) characters. "len" must be an even number.
  static void decode(const uint8_t* encoded, size_t len, uint8_t* decoded);
};

} // namespace airball

#endif //SRC_AIRBALL_UTIL_BASE16ENCODING_H

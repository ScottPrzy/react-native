/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bitset>
#include <cstdio>
#include <cstdint>

#include <yoga/YGEnums.h>

namespace facebook::yoga::details {

constexpr size_t log2ceilFn(size_t n) {
  return n < 1 ? 0 : (1 + log2ceilFn(n / 2));
}

constexpr int mask(size_t bitWidth, size_t index) {
  return ((1 << bitWidth) - 1) << index;
}

} // namespace facebook::yoga::details

namespace facebook::yoga {

// The number of bits necessary to represent enums defined with YG_ENUM_SEQ_DECL
template <typename Enum>
constexpr size_t minimumBitCount() {
  static_assert(
      enums::count<Enum>() > 0, "Enums must have at least one entries");
  return details::log2ceilFn(enums::count<Enum>() - 1);
}

template <typename Enum>
constexpr Enum getEnumData(int flags, size_t index) {
  return static_cast<Enum>(
      (flags & details::mask(minimumBitCount<Enum>(), index)) >> index);
}

template <typename Enum>
void setEnumData(uint32_t& flags, size_t index, int newValue) {
  flags = (flags & ~details::mask(minimumBitCount<Enum>(), index)) |
      ((newValue << index) & (details::mask(minimumBitCount<Enum>(), index)));
}

template <typename Enum>
void setEnumData(uint8_t& flags, size_t index, int newValue) {
  flags =
      (flags &
       ~static_cast<uint8_t>(details::mask(minimumBitCount<Enum>(), index))) |
      ((newValue << index) &
       (static_cast<uint8_t>(details::mask(minimumBitCount<Enum>(), index))));
}

constexpr bool getBooleanData(int flags, size_t index) {
  return (flags >> index) & 1;
}

inline void setBooleanData(uint8_t& flags, size_t index, bool value) {
  if (value) {
    flags |= 1 << index;
  } else {
    flags &= ~(1 << index);
  }
}

} // namespace facebook::yoga

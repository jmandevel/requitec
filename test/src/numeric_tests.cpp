// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include "catch2_ext.hpp"
#include <cstdint>
#include <requite/numeric.hpp>

TEST_CASE("requite::getNumericValue(llvm::StringRef)") {
  SECTION("unsigned integrals") {
    std::uint8_t uint8_value;
    CHECK(requite::getNumericValue<std::uint8_t>("2", uint8_value) ==
          requite::NumericResult::OK);
    CHECK(uint8_value == 2);
    CHECK(requite::getNumericValue<std::uint8_t>("2x110", uint8_value) ==
          requite::NumericResult::OK);
    CHECK(uint8_value == 6);
    CHECK(requite::getNumericValue<std::uint8_t>("16xFF", uint8_value) ==
          requite::NumericResult::OK);
    CHECK(uint8_value == 255);
    CHECK(requite::getNumericValue<std::uint8_t>("16xff", uint8_value) ==
          requite::NumericResult::OK);
    CHECK(uint8_value == 255);
    CHECK(requite::getNumericValue<std::uint8_t>("64x0", uint8_value) ==
          requite::NumericResult::OK);
    CHECK(uint8_value == 0);

    std::uint16_t uint16_value;
    CHECK(requite::getNumericValue<std::uint16_t>("1000", uint16_value) ==
          requite::NumericResult::OK);
    CHECK(uint16_value == 1000);
    CHECK(requite::getNumericValue<std::uint16_t>("2x101110", uint16_value) ==
          requite::NumericResult::OK);
    CHECK(uint16_value == 46);
    CHECK(requite::getNumericValue<std::uint16_t>("16x7FFF", uint16_value) ==
          requite::NumericResult::OK);
    CHECK(uint16_value == 32767);
    CHECK(requite::getNumericValue<std::uint16_t>("16xFFFF", uint16_value) ==
          requite::NumericResult::OK);
    CHECK(uint16_value == 65535);
    CHECK(requite::getNumericValue<std::uint16_t>("64x0", uint16_value) ==
          requite::NumericResult::OK);
    CHECK(uint16_value == 0);

    std::uint32_t uint32_value;
    CHECK(requite::getNumericValue<std::uint32_t>("100000", uint32_value) ==
          requite::NumericResult::OK);
    CHECK(uint32_value == 100000);
    CHECK(requite::getNumericValue<std::uint32_t>(
              "2x110110101", uint32_value) == requite::NumericResult::OK);
    CHECK(uint32_value == 437);
    CHECK(requite::getNumericValue<std::uint32_t>(
              "16x7FFFFFFF", uint32_value) == requite::NumericResult::OK);
    CHECK(uint32_value == 2147483647);
    CHECK(requite::getNumericValue<std::uint32_t>(
              "16xFFFFFFFF", uint32_value) == requite::NumericResult::OK);
    CHECK(uint32_value == 4294967295);
    CHECK(requite::getNumericValue<std::uint32_t>("64x0", uint32_value) ==
          requite::NumericResult::OK);
    CHECK(uint32_value == 0);

    std::uint64_t uint64_value;
    CHECK(requite::getNumericValue<std::uint64_t>("1000000000", uint64_value) ==
          requite::NumericResult::OK);
    CHECK(uint64_value == 1000000000);
    CHECK(requite::getNumericValue<std::uint64_t>("2x110110101001011010",
                                                  uint64_value) ==
          requite::NumericResult::OK);
    CHECK(uint64_value == 223834);
    CHECK(requite::getNumericValue<std::uint64_t>("16x7FFFFFFFFFFFFFFF",
                                                  uint64_value) ==
          requite::NumericResult::OK);
    CHECK(uint64_value == 9223372036854775807);
    CHECK(requite::getNumericValue<std::uint64_t>("16xFFFFFFFFFFFFFFFF",
                                                  uint64_value) ==
          requite::NumericResult::OK);
    CHECK(uint64_value == 18446744073709551615u);
    CHECK(requite::getNumericValue<std::uint64_t>("64x0", uint64_value) ==
          requite::NumericResult::OK);
    CHECK(uint64_value == 0);
  }

  SECTION("signed integrals") {
    std::int8_t int8_value;
    CHECK(requite::getNumericValue<std::int8_t>("2", int8_value) ==
          requite::NumericResult::OK);
    CHECK(int8_value == 2);
    CHECK(requite::getNumericValue<std::int8_t>("2x110", int8_value) ==
          requite::NumericResult::OK);
    CHECK(int8_value == 6);
    CHECK(requite::getNumericValue<std::int8_t>("16xFF", int8_value) ==
          requite::NumericResult::OK);
    CHECK(int8_value == -1);
    CHECK(requite::getNumericValue<std::int8_t>("16xff", int8_value) ==
          requite::NumericResult::OK);
    CHECK(int8_value == -1);
    CHECK(requite::getNumericValue<std::int8_t>("64x0", int8_value) ==
          requite::NumericResult::OK);
    CHECK(int8_value == 0);

    std::int16_t int16_value;
    CHECK(requite::getNumericValue<std::int16_t>("1000", int16_value) ==
          requite::NumericResult::OK);
    CHECK(int16_value == 1000);
    CHECK(requite::getNumericValue<std::int16_t>("2x101110", int16_value) ==
          requite::NumericResult::OK);
    CHECK(int16_value == 46);
    CHECK(requite::getNumericValue<std::int16_t>("16x7FFF", int16_value) ==
          requite::NumericResult::OK);
    CHECK(int16_value == 32767);
    CHECK(requite::getNumericValue<std::int16_t>("16x8000", int16_value) ==
          requite::NumericResult::OK);
    CHECK(int16_value == std::numeric_limits<std::int16_t>::min());
    CHECK(requite::getNumericValue<std::int16_t>("16xFFFF", int16_value) ==
          requite::NumericResult::OK);
    CHECK(int16_value == -1);
    CHECK(requite::getNumericValue<std::int16_t>("64x0", int16_value) ==
          requite::NumericResult::OK);
    CHECK(int16_value == 0);

    std::int32_t int32_value;
    CHECK(requite::getNumericValue<std::int32_t>("100000", int32_value) ==
          requite::NumericResult::OK);
    CHECK(int32_value == 100000);
    CHECK(requite::getNumericValue<std::int32_t>("2x110110101", int32_value) ==
          requite::NumericResult::OK);
    CHECK(int32_value == 437);
    CHECK(requite::getNumericValue<std::int32_t>("16x7FFFFFFF", int32_value) ==
          requite::NumericResult::OK);
    CHECK(int32_value == 2147483647);
    CHECK(requite::getNumericValue<std::int32_t>("16x80000000", int32_value) ==
          requite::NumericResult::OK);
    CHECK(int32_value == std::numeric_limits<std::int32_t>::min());
    CHECK(requite::getNumericValue<std::int32_t>("16xFFFFFFFF", int32_value) ==
          requite::NumericResult::OK);
    CHECK(int32_value == -1);
    CHECK(requite::getNumericValue<std::int32_t>("64x0", int32_value) ==
          requite::NumericResult::OK);
    CHECK(int32_value == 0);

    std::int64_t int64_value;
    CHECK(requite::getNumericValue<std::int64_t>("1000000000", int64_value) ==
          requite::NumericResult::OK);
    CHECK(int64_value == 1000000000);
    CHECK(requite::getNumericValue<std::int64_t>("2x110110101001011010",
                                                 int64_value) ==
          requite::NumericResult::OK);
    CHECK(int64_value == 223834);
    CHECK(requite::getNumericValue<std::int64_t>("16x7FFFFFFFFFFFFFFF",
                                                 int64_value) ==
          requite::NumericResult::OK);
    CHECK(int64_value == 9223372036854775807);
    CHECK(requite::getNumericValue<std::int64_t>("16x8000000000000000",
                                                 int64_value) ==
          requite::NumericResult::OK);
    CHECK(int64_value == std::numeric_limits<std::int64_t>::min());
    CHECK(requite::getNumericValue<std::int64_t>("16xFFFFFFFFFFFFFFFF",
                                                 int64_value) ==
          requite::NumericResult::OK);
    CHECK(int64_value == -1);
    CHECK(requite::getNumericValue<std::int64_t>("64x0", int64_value) ==
          requite::NumericResult::OK);
    CHECK(int64_value == 0);
  }
}
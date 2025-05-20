// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include "catch2_ext.hpp"

#include <requite/codeunits.hpp>

#include <bitset>
#include <string>
#include <utility>

TEST_CASE("requite::getUtf8Name(unsigned char)") {
  SECTION("ASCII control characters and spaces") {
    CHECK(requite::getUtf8Name(0x00) == "NUL");
    CHECK(requite::getUtf8Name(0x01) == "SOH");
    CHECK(requite::getUtf8Name(0x02) == "STX");
    CHECK(requite::getUtf8Name(0x03) == "ETX");
    CHECK(requite::getUtf8Name(0x04) == "EOT");
    CHECK(requite::getUtf8Name(0x05) == "ENQ");
    CHECK(requite::getUtf8Name(0x06) == "ACK");
    CHECK(requite::getUtf8Name(0x07) == "BEL");
    CHECK(requite::getUtf8Name(0x08) == "BS");
    CHECK(requite::getUtf8Name(0x09) == "HT");
    CHECK(requite::getUtf8Name(0x0A) == "LF");
    CHECK(requite::getUtf8Name(0x0B) == "VT");
    CHECK(requite::getUtf8Name(0x0C) == "FF");
    CHECK(requite::getUtf8Name(0x0D) == "CR");
    CHECK(requite::getUtf8Name(0x0E) == "SO");
    CHECK(requite::getUtf8Name(0x0F) == "SI");
    CHECK(requite::getUtf8Name(0x10) == "DLE");
    CHECK(requite::getUtf8Name(0x11) == "DC1");
    CHECK(requite::getUtf8Name(0x12) == "DC2");
    CHECK(requite::getUtf8Name(0x13) == "DC3");
    CHECK(requite::getUtf8Name(0x14) == "DC4");
    CHECK(requite::getUtf8Name(0x15) == "NAK");
    CHECK(requite::getUtf8Name(0x16) == "SYN");
    CHECK(requite::getUtf8Name(0x17) == "ETB");
    CHECK(requite::getUtf8Name(0x18) == "CAN");
    CHECK(requite::getUtf8Name(0x19) == "EM");
    CHECK(requite::getUtf8Name(0x1A) == "SUB");
    CHECK(requite::getUtf8Name(0x1B) == "ESC");
    CHECK(requite::getUtf8Name(0x1C) == "FS");
    CHECK(requite::getUtf8Name(0x1D) == "GS");
    CHECK(requite::getUtf8Name(0x1E) == "RS");
    CHECK(requite::getUtf8Name(0x1F) == "US");
    CHECK(requite::getUtf8Name(0x20) == "SP");
  }

  SECTION("ASCII symbols") {
    CHECK(requite::getUtf8Name(0x21) == "!");
    CHECK(requite::getUtf8Name(0x22) == "\"");
    CHECK(requite::getUtf8Name(0x23) == "#");
    CHECK(requite::getUtf8Name(0x24) == "$");
    CHECK(requite::getUtf8Name(0x25) == "%");
    CHECK(requite::getUtf8Name(0x26) == "&");
    CHECK(requite::getUtf8Name(0x27) == "'");
    CHECK(requite::getUtf8Name(0x28) == "(");
    CHECK(requite::getUtf8Name(0x29) == ")");
    CHECK(requite::getUtf8Name(0x2A) == "*");
    CHECK(requite::getUtf8Name(0x2B) == "+");
    CHECK(requite::getUtf8Name(0x2C) == ",");
    CHECK(requite::getUtf8Name(0x2D) == "-");
    CHECK(requite::getUtf8Name(0x2E) == ".");
    CHECK(requite::getUtf8Name(0x2F) == "/");
    CHECK(requite::getUtf8Name(0x3A) == ":");
    CHECK(requite::getUtf8Name(0x3B) == ";");
    CHECK(requite::getUtf8Name(0x3C) == "<");
    CHECK(requite::getUtf8Name(0x3D) == "=");
    CHECK(requite::getUtf8Name(0x3E) == ">");
    CHECK(requite::getUtf8Name(0x3F) == "?");
    CHECK(requite::getUtf8Name(0x40) == "@");
  }

  SECTION("ASCII digits") {
    CHECK(requite::getUtf8Name(0x30) == "0");
    CHECK(requite::getUtf8Name(0x31) == "1");
    CHECK(requite::getUtf8Name(0x32) == "2");
    CHECK(requite::getUtf8Name(0x33) == "3");
    CHECK(requite::getUtf8Name(0x34) == "4");
    CHECK(requite::getUtf8Name(0x35) == "5");
    CHECK(requite::getUtf8Name(0x36) == "6");
    CHECK(requite::getUtf8Name(0x37) == "7");
    CHECK(requite::getUtf8Name(0x38) == "8");
    CHECK(requite::getUtf8Name(0x39) == "9");
  }

  SECTION("ASCII letters") {
    for (char c = 'A'; c <= 'Z'; ++c) {
      CHECK(requite::getUtf8Name(c) == std::string(1, c));
    }
    for (char c = 'a'; c <= 'z'; ++c) {
      CHECK(requite::getUtf8Name(c) == std::string(1, c));
    }
    CHECK(requite::getUtf8Name(0x7F) == "DEL");
  }

  SECTION("Extended characters") {
    for (unsigned char codeunit = 0x80; codeunit <= 0xBF; ++codeunit) {
      std::string expected_name =
          "CONTINUE_" + std::bitset<6>(codeunit & 0x3F).to_string();
      CHECK(requite::getUtf8Name(codeunit) == expected_name);
    }

    for (unsigned char codeunit = 0xC0; codeunit <= 0xDF; ++codeunit) {
      std::string expected_name =
          "TWO_START_" + std::bitset<5>(codeunit & 0x1F).to_string();
      CHECK(requite::getUtf8Name(codeunit) == expected_name);
    }

    for (unsigned char codeunit = 0xE0; codeunit <= 0xEF; ++codeunit) {
      std::string expected_name =
          "THREE_START_" + std::bitset<4>(codeunit & 0x0F).to_string();
      CHECK(requite::getUtf8Name(codeunit) == expected_name);
    }

    for (unsigned char codeunit = 0xF0; codeunit <= 0xF7; ++codeunit) {
      std::string expected_name =
          "FOUR_START_" + std::bitset<3>(codeunit & 0x07).to_string();
      CHECK(requite::getUtf8Name(codeunit) == expected_name);
    }

    for (unsigned char codeunit = 0xF8; codeunit >= 0xF8; ++codeunit) {
      std::string expected_name = "INVALID_" + std::to_string(codeunit & 0x07);
      CHECK(requite::getUtf8Name(codeunit) == expected_name);
    }
  }
}

TEST_CASE("requite::_getFlags(unsignec char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK(requite::_getFlags(0x00) == requite::_CHAR_FLAGS_NUL);
    CHECK(requite::_getFlags(0x01) == requite::_CHAR_FLAGS_SOH);
    CHECK(requite::_getFlags(0x02) == requite::_CHAR_FLAGS_STX);
    CHECK(requite::_getFlags(0x03) == requite::_CHAR_FLAGS_ETX);
    CHECK(requite::_getFlags(0x04) == requite::_CHAR_FLAGS_EOT);
    CHECK(requite::_getFlags(0x05) == requite::_CHAR_FLAGS_ENQ);
    CHECK(requite::_getFlags(0x06) == requite::_CHAR_FLAGS_ACK);
    CHECK(requite::_getFlags(0x07) == requite::_CHAR_FLAGS_BEL);
    CHECK(requite::_getFlags(0x08) == requite::_CHAR_FLAGS_BS);
    CHECK(requite::_getFlags(0x09) == requite::_CHAR_FLAGS_HT);
    CHECK(requite::_getFlags(0x0A) == requite::_CHAR_FLAGS_LF);
    CHECK(requite::_getFlags(0x0B) == requite::_CHAR_FLAGS_VT);
    CHECK(requite::_getFlags(0x0C) == requite::_CHAR_FLAGS_FF);
    CHECK(requite::_getFlags(0x0D) == requite::_CHAR_FLAGS_CR);
    CHECK(requite::_getFlags(0x0E) == requite::_CHAR_FLAGS_SO);
    CHECK(requite::_getFlags(0x0F) == requite::_CHAR_FLAGS_SI);
    CHECK(requite::_getFlags(0x10) == requite::_CHAR_FLAGS_DLE);
    CHECK(requite::_getFlags(0x11) == requite::_CHAR_FLAGS_DC1);
    CHECK(requite::_getFlags(0x12) == requite::_CHAR_FLAGS_DC2);
    CHECK(requite::_getFlags(0x13) == requite::_CHAR_FLAGS_DC3);
    CHECK(requite::_getFlags(0x14) == requite::_CHAR_FLAGS_DC4);
    CHECK(requite::_getFlags(0x15) == requite::_CHAR_FLAGS_NAK);
    CHECK(requite::_getFlags(0x16) == requite::_CHAR_FLAGS_SYN);
    CHECK(requite::_getFlags(0x17) == requite::_CHAR_FLAGS_ETB);
    CHECK(requite::_getFlags(0x18) == requite::_CHAR_FLAGS_CAN);
    CHECK(requite::_getFlags(0x19) == requite::_CHAR_FLAGS_EM);
    CHECK(requite::_getFlags(0x1A) == requite::_CHAR_FLAGS_SUB);
    CHECK(requite::_getFlags(0x1B) == requite::_CHAR_FLAGS_ESC);
    CHECK(requite::_getFlags(0x1C) == requite::_CHAR_FLAGS_FS);
    CHECK(requite::_getFlags(0x1D) == requite::_CHAR_FLAGS_GS);
    CHECK(requite::_getFlags(0x1E) == requite::_CHAR_FLAGS_RS);
    CHECK(requite::_getFlags(0x1F) == requite::_CHAR_FLAGS_US);
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK(requite::_getFlags(0x20) == requite::_CHAR_FLAGS_SPACE);
    CHECK(requite::_getFlags(0x21) == requite::_CHAR_FLAGS_BANG);
    CHECK(requite::_getFlags(0x22) == requite::_CHAR_FLAGS_DOUBLE_QUOTE);
    CHECK(requite::_getFlags(0x23) == requite::_CHAR_FLAGS_HASH);
    CHECK(requite::_getFlags(0x24) == requite::_CHAR_FLAGS_DOLLAR);
    CHECK(requite::_getFlags(0x25) == requite::_CHAR_FLAGS_MODULO);
    CHECK(requite::_getFlags(0x26) == requite::_CHAR_FLAGS_AMPERPAND);
    CHECK(requite::_getFlags(0x27) == requite::_CHAR_FLAGS_SINGLE_QUOTE);
    CHECK(requite::_getFlags(0x28) == requite::_CHAR_FLAGS_OPEN_PARENTHESIS);
    CHECK(requite::_getFlags(0x29) == requite::_CHAR_FLAGS_CLOSE_PARENTHESIS);
    CHECK(requite::_getFlags(0x2A) == requite::_CHAR_FLAGS_ASTERISK);
    CHECK(requite::_getFlags(0x2B) == requite::_CHAR_FLAGS_PLUS);
    CHECK(requite::_getFlags(0x2C) == requite::_CHAR_FLAGS_COMMA);
    CHECK(requite::_getFlags(0x2D) == requite::_CHAR_FLAGS_MINUS);
    CHECK(requite::_getFlags(0x2E) == requite::_CHAR_FLAGS_DOT);
    CHECK(requite::_getFlags(0x2F) == requite::_CHAR_FLAGS_FORWARD_SLASH);
    CHECK(requite::_getFlags(0x30) == requite::_CHAR_FLAGS_DIGIT_0);
    CHECK(requite::_getFlags(0x31) == requite::_CHAR_FLAGS_DIGIT_1);
    CHECK(requite::_getFlags(0x32) == requite::_CHAR_FLAGS_DIGIT_2);
    CHECK(requite::_getFlags(0x33) == requite::_CHAR_FLAGS_DIGIT_3);
    CHECK(requite::_getFlags(0x34) == requite::_CHAR_FLAGS_DIGIT_4);
    CHECK(requite::_getFlags(0x35) == requite::_CHAR_FLAGS_DIGIT_5);
    CHECK(requite::_getFlags(0x36) == requite::_CHAR_FLAGS_DIGIT_6);
    CHECK(requite::_getFlags(0x37) == requite::_CHAR_FLAGS_DIGIT_7);
    CHECK(requite::_getFlags(0x38) == requite::_CHAR_FLAGS_DIGIT_8);
    CHECK(requite::_getFlags(0x39) == requite::_CHAR_FLAGS_DIGIT_9);
    CHECK(requite::_getFlags(0x3A) == requite::_CHAR_FLAGS_COLON);
    CHECK(requite::_getFlags(0x3B) == requite::_CHAR_FLAGS_SEMICOLON);
    CHECK(requite::_getFlags(0x3C) == requite::_CHAR_FLAGS_OPEN_ANGLE);
    CHECK(requite::_getFlags(0x3D) == requite::_CHAR_FLAGS_EQUAL);
    CHECK(requite::_getFlags(0x3E) == requite::_CHAR_FLAGS_CLOSE_ANGLE);
    CHECK(requite::_getFlags(0x3F) == requite::_CHAR_FLAGS_QUESTION);
    CHECK(requite::_getFlags(0x40) == requite::_CHAR_FLAGS_AT);
    CHECK(requite::_getFlags(0x41) == requite::_CHAR_FLAGS_UPPER_A);
    CHECK(requite::_getFlags(0x42) == requite::_CHAR_FLAGS_UPPER_B);
    CHECK(requite::_getFlags(0x43) == requite::_CHAR_FLAGS_UPPER_C);
    CHECK(requite::_getFlags(0x44) == requite::_CHAR_FLAGS_UPPER_D);
    CHECK(requite::_getFlags(0x45) == requite::_CHAR_FLAGS_UPPER_E);
    CHECK(requite::_getFlags(0x46) == requite::_CHAR_FLAGS_UPPER_F);
    CHECK(requite::_getFlags(0x47) == requite::_CHAR_FLAGS_UPPER_G);
    CHECK(requite::_getFlags(0x48) == requite::_CHAR_FLAGS_UPPER_H);
    CHECK(requite::_getFlags(0x49) == requite::_CHAR_FLAGS_UPPER_I);
    CHECK(requite::_getFlags(0x4A) == requite::_CHAR_FLAGS_UPPER_J);
    CHECK(requite::_getFlags(0x4B) == requite::_CHAR_FLAGS_UPPER_K);
    CHECK(requite::_getFlags(0x4C) == requite::_CHAR_FLAGS_UPPER_L);
    CHECK(requite::_getFlags(0x4D) == requite::_CHAR_FLAGS_UPPER_M);
    CHECK(requite::_getFlags(0x4E) == requite::_CHAR_FLAGS_UPPER_N);
    CHECK(requite::_getFlags(0x4F) == requite::_CHAR_FLAGS_UPPER_O);
    CHECK(requite::_getFlags(0x50) == requite::_CHAR_FLAGS_UPPER_P);
    CHECK(requite::_getFlags(0x51) == requite::_CHAR_FLAGS_UPPER_Q);
    CHECK(requite::_getFlags(0x52) == requite::_CHAR_FLAGS_UPPER_R);
    CHECK(requite::_getFlags(0x53) == requite::_CHAR_FLAGS_UPPER_S);
    CHECK(requite::_getFlags(0x54) == requite::_CHAR_FLAGS_UPPER_T);
    CHECK(requite::_getFlags(0x55) == requite::_CHAR_FLAGS_UPPER_U);
    CHECK(requite::_getFlags(0x56) == requite::_CHAR_FLAGS_UPPER_V);
    CHECK(requite::_getFlags(0x57) == requite::_CHAR_FLAGS_UPPER_W);
    CHECK(requite::_getFlags(0x58) == requite::_CHAR_FLAGS_UPPER_X);
    CHECK(requite::_getFlags(0x59) == requite::_CHAR_FLAGS_UPPER_Y);
    CHECK(requite::_getFlags(0x5A) == requite::_CHAR_FLAGS_UPPER_Z);
    CHECK(requite::_getFlags(0x5B) == requite::_CHAR_FLAGS_OPEN_BRACKET);
    CHECK(requite::_getFlags(0x5C) == requite::_CHAR_FLAGS_BACK_SLASH);
    CHECK(requite::_getFlags(0x5D) == requite::_CHAR_FLAGS_CLOSE_BRACKET);
    CHECK(requite::_getFlags(0x5E) == requite::_CHAR_FLAGS_CARET);
    CHECK(requite::_getFlags(0x5F) == requite::_CHAR_FLAGS_UNDERSCORE);
    CHECK(requite::_getFlags(0x60) == requite::_CHAR_FLAGS_GRAVE);
    CHECK(requite::_getFlags(0x61) == requite::_CHAR_FLAGS_LOWER_A);
    CHECK(requite::_getFlags(0x62) == requite::_CHAR_FLAGS_LOWER_B);
    CHECK(requite::_getFlags(0x63) == requite::_CHAR_FLAGS_LOWER_C);
    CHECK(requite::_getFlags(0x64) == requite::_CHAR_FLAGS_LOWER_D);
    CHECK(requite::_getFlags(0x65) == requite::_CHAR_FLAGS_LOWER_E);
    CHECK(requite::_getFlags(0x66) == requite::_CHAR_FLAGS_LOWER_F);
    CHECK(requite::_getFlags(0x67) == requite::_CHAR_FLAGS_LOWER_G);
    CHECK(requite::_getFlags(0x68) == requite::_CHAR_FLAGS_LOWER_H);
    CHECK(requite::_getFlags(0x69) == requite::_CHAR_FLAGS_LOWER_I);
    CHECK(requite::_getFlags(0x6A) == requite::_CHAR_FLAGS_LOWER_J);
    CHECK(requite::_getFlags(0x6B) == requite::_CHAR_FLAGS_LOWER_K);
    CHECK(requite::_getFlags(0x6C) == requite::_CHAR_FLAGS_LOWER_L);
    CHECK(requite::_getFlags(0x6D) == requite::_CHAR_FLAGS_LOWER_M);
    CHECK(requite::_getFlags(0x6E) == requite::_CHAR_FLAGS_LOWER_N);
    CHECK(requite::_getFlags(0x6F) == requite::_CHAR_FLAGS_LOWER_O);
    CHECK(requite::_getFlags(0x70) == requite::_CHAR_FLAGS_LOWER_P);
    CHECK(requite::_getFlags(0x71) == requite::_CHAR_FLAGS_LOWER_Q);
    CHECK(requite::_getFlags(0x72) == requite::_CHAR_FLAGS_LOWER_R);
    CHECK(requite::_getFlags(0x73) == requite::_CHAR_FLAGS_LOWER_S);
    CHECK(requite::_getFlags(0x74) == requite::_CHAR_FLAGS_LOWER_T);
    CHECK(requite::_getFlags(0x75) == requite::_CHAR_FLAGS_LOWER_U);
    CHECK(requite::_getFlags(0x76) == requite::_CHAR_FLAGS_LOWER_V);
    CHECK(requite::_getFlags(0x77) == requite::_CHAR_FLAGS_LOWER_W);
    CHECK(requite::_getFlags(0x78) == requite::_CHAR_FLAGS_LOWER_X);
    CHECK(requite::_getFlags(0x79) == requite::_CHAR_FLAGS_LOWER_Y);
    CHECK(requite::_getFlags(0x7A) == requite::_CHAR_FLAGS_LOWER_Z);
    CHECK(requite::_getFlags(0x7B) == requite::_CHAR_FLAGS_OPEN_BRACE);
    CHECK(requite::_getFlags(0x7C) == requite::_CHAR_FLAGS_PIPE);
    CHECK(requite::_getFlags(0x7D) == requite::_CHAR_FLAGS_CLOSE_BRACE);
    CHECK(requite::_getFlags(0x7E) == requite::_CHAR_FLAGS_TILDE);
  }

  SECTION("DEL Character") {
    CHECK(requite::_getFlags(0x7F) == requite::_CHAR_FLAGS_DEL);
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    // Loop through extended characters (0x80 - 0xBF)
    for (unsigned char c = 0x80; c <= 0xBF; ++c) {
      CHECK(requite::_getFlags(c) == requite::_CHAR_COMMON_FLAGS_CONTINUE);
    }

    // Loop through extended characters (0xC0 - 0xDF)
    for (unsigned char c = 0xC0; c <= 0xDF; ++c) {
      CHECK(requite::_getFlags(c) == requite::_CHAR_COMMON_FLAGS_TWO_START);
    }

    // Loop through extended characters (0xE0 - 0xEF)
    for (unsigned char c = 0xE0; c <= 0xEF; ++c) {
      CHECK(requite::_getFlags(c) == requite::_CHAR_COMMON_FLAGS_THREE_START);
    }

    // Loop through extended characters (0xF0 - 0xF7)
    for (unsigned char c = 0xF0; c <= 0xF7; ++c) {
      CHECK(requite::_getFlags(c) == requite::_CHAR_COMMON_FLAGS_FOUR_START);
    }

    // Loop through invalid extended characters (0xF8 - 0xFF)
    for (unsigned char c = 0xF8; c >= 0xF8; ++c) {
      CHECK(requite::_getFlags(c) == requite::_CHAR_COMMON_FLAGS_INVALID);
    }
  }
}

TEST_CASE("_getHasFlags(char, requite::_CharFlags) ") {
  CHECK(requite::_getHasFlags(' ', requite::_CHAR_FLAG_SPACE));
  CHECK_FALSE(requite::_getHasFlags(' ', requite::_CHAR_FLAG_VERTICAL_SPACE));
}

TEST_CASE("_getMaskValue(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK(+requite::_getMaskValue(0x00) == 0); // NUL
    CHECK(+requite::_getMaskValue(0x01) == 0); // SOH
    CHECK(+requite::_getMaskValue(0x02) == 0); // STX
    CHECK(+requite::_getMaskValue(0x03) == 0); // ETX
    CHECK(+requite::_getMaskValue(0x04) == 0); // EOT
    CHECK(+requite::_getMaskValue(0x05) == 0); // ENQ
    CHECK(+requite::_getMaskValue(0x06) == 0); // ACK
    CHECK(+requite::_getMaskValue(0x07) == 0); // BEL
    CHECK(+requite::_getMaskValue(0x08) == 0); // BS
    CHECK(+requite::_getMaskValue(0x09) == 0); // HT
    CHECK(+requite::_getMaskValue(0x0A) == 0); // LF
    CHECK(+requite::_getMaskValue(0x0B) == 0); // VT
    CHECK(+requite::_getMaskValue(0x0C) == 0); // FF
    CHECK(+requite::_getMaskValue(0x0D) == 0); // CR
    CHECK(+requite::_getMaskValue(0x0E) == 0); // SO
    CHECK(+requite::_getMaskValue(0x0F) == 0); // SI
    CHECK(+requite::_getMaskValue(0x10) == 0); // DLE
    CHECK(+requite::_getMaskValue(0x11) == 0); // DC1
    CHECK(+requite::_getMaskValue(0x12) == 0); // DC2
    CHECK(+requite::_getMaskValue(0x13) == 0); // DC3
    CHECK(+requite::_getMaskValue(0x14) == 0); // DC4
    CHECK(+requite::_getMaskValue(0x15) == 0); // NAK
    CHECK(+requite::_getMaskValue(0x16) == 0); // SYN
    CHECK(+requite::_getMaskValue(0x17) == 0); // ETB
    CHECK(+requite::_getMaskValue(0x18) == 0); // CAN
    CHECK(+requite::_getMaskValue(0x19) == 0); // EM
    CHECK(+requite::_getMaskValue(0x1A) == 0); // SUB
    CHECK(+requite::_getMaskValue(0x1B) == 0); // ESC
    CHECK(+requite::_getMaskValue(0x1C) == 0); // FS
    CHECK(+requite::_getMaskValue(0x1D) == 0); // GS
    CHECK(+requite::_getMaskValue(0x1E) == 0); // RS
    CHECK(+requite::_getMaskValue(0x1F) == 0); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK(+requite::_getMaskValue(0x20) == 0);  // SP
    CHECK(+requite::_getMaskValue(0x21) == 0);  // '!'
    CHECK(+requite::_getMaskValue(0x22) == 0);  // '"'
    CHECK(+requite::_getMaskValue(0x23) == 0);  // '#'
    CHECK(+requite::_getMaskValue(0x24) == 0);  // '$'
    CHECK(+requite::_getMaskValue(0x25) == 0);  // '%'
    CHECK(+requite::_getMaskValue(0x26) == 0);  // '&'
    CHECK(+requite::_getMaskValue(0x27) == 0);  // '''
    CHECK(+requite::_getMaskValue(0x28) == 0);  // '('
    CHECK(+requite::_getMaskValue(0x29) == 0);  // ')'
    CHECK(+requite::_getMaskValue(0x2A) == 0);  // '*'
    CHECK(+requite::_getMaskValue(0x2B) == 0);  // '+'
    CHECK(+requite::_getMaskValue(0x2C) == 0);  // ','
    CHECK(+requite::_getMaskValue(0x2D) == 0);  // '-'
    CHECK(+requite::_getMaskValue(0x2E) == 62); // '.'
    CHECK(+requite::_getMaskValue(0x2F) == 0);  // '/'
    CHECK(+requite::_getMaskValue(0x30) == 0);  // '0'
    CHECK(+requite::_getMaskValue(0x31) == 1);  // '1'
    CHECK(+requite::_getMaskValue(0x32) == 2);  // '2'
    CHECK(+requite::_getMaskValue(0x33) == 3);  // '3'
    CHECK(+requite::_getMaskValue(0x34) == 4);  // '4'
    CHECK(+requite::_getMaskValue(0x35) == 5);  // '5'
    CHECK(+requite::_getMaskValue(0x36) == 6);  // '6'
    CHECK(+requite::_getMaskValue(0x37) == 7);  // '7'
    CHECK(+requite::_getMaskValue(0x38) == 8);  // '8'
    CHECK(+requite::_getMaskValue(0x39) == 9);  // '9'
    CHECK(+requite::_getMaskValue(0x3A) == 63); // ':'
    CHECK(+requite::_getMaskValue(0x3B) == 0);  // ';'
    CHECK(+requite::_getMaskValue(0x3C) == 0);  // '<'
    CHECK(+requite::_getMaskValue(0x3D) == 0);  // '='
    CHECK(+requite::_getMaskValue(0x3E) == 0);  // '>'
    CHECK(+requite::_getMaskValue(0x3F) == 0);  // '?'
    CHECK(+requite::_getMaskValue(0x40) == 0);  // '@'
    CHECK(+requite::_getMaskValue(0x41) == 36); // 'A'
    CHECK(+requite::_getMaskValue(0x42) == 37); // 'B'
    CHECK(+requite::_getMaskValue(0x43) == 38); // 'C'
    CHECK(+requite::_getMaskValue(0x44) == 39); // 'D'
    CHECK(+requite::_getMaskValue(0x45) == 40); // 'E'
    CHECK(+requite::_getMaskValue(0x46) == 41); // 'F'
    CHECK(+requite::_getMaskValue(0x47) == 42); // 'G'
    CHECK(+requite::_getMaskValue(0x48) == 43); // 'H'
    CHECK(+requite::_getMaskValue(0x49) == 44); // 'I'
    CHECK(+requite::_getMaskValue(0x4A) == 45); // 'J'
    CHECK(+requite::_getMaskValue(0x4B) == 46); // 'K'
    CHECK(+requite::_getMaskValue(0x4C) == 47); // 'L'
    CHECK(+requite::_getMaskValue(0x4D) == 48); // 'M'
    CHECK(+requite::_getMaskValue(0x4E) == 49); // 'N'
    CHECK(+requite::_getMaskValue(0x4F) == 50); // 'O'
    CHECK(+requite::_getMaskValue(0x50) == 51); // 'P'
    CHECK(+requite::_getMaskValue(0x51) == 52); // 'Q'
    CHECK(+requite::_getMaskValue(0x52) == 53); // 'R'
    CHECK(+requite::_getMaskValue(0x53) == 54); // 'S'
    CHECK(+requite::_getMaskValue(0x54) == 55); // 'T'
    CHECK(+requite::_getMaskValue(0x55) == 56); // 'U'
    CHECK(+requite::_getMaskValue(0x56) == 57); // 'V'
    CHECK(+requite::_getMaskValue(0x57) == 58); // 'W'
    CHECK(+requite::_getMaskValue(0x58) == 59); // 'X'
    CHECK(+requite::_getMaskValue(0x59) == 60); // 'Y'
    CHECK(+requite::_getMaskValue(0x5A) == 61); // 'Z'
    CHECK(+requite::_getMaskValue(0x5B) == 0);  // '['
    CHECK(+requite::_getMaskValue(0x5C) == 0);  // '\'
    CHECK(+requite::_getMaskValue(0x5D) == 0);  // ']'
    CHECK(+requite::_getMaskValue(0x5E) == 0);  // '^'
    CHECK(+requite::_getMaskValue(0x5F) == 0);  // '_'
    CHECK(+requite::_getMaskValue(0x60) == 0);  // '`'
    CHECK(+requite::_getMaskValue(0x61) == 10); // 'a'
    CHECK(+requite::_getMaskValue(0x62) == 11); // 'b'
    CHECK(+requite::_getMaskValue(0x63) == 12); // 'c'
    CHECK(+requite::_getMaskValue(0x64) == 13); // 'd'
    CHECK(+requite::_getMaskValue(0x65) == 14); // 'e'
    CHECK(+requite::_getMaskValue(0x66) == 15); // 'f'
    CHECK(+requite::_getMaskValue(0x67) == 16); // 'g'
    CHECK(+requite::_getMaskValue(0x68) == 17); // 'h'
    CHECK(+requite::_getMaskValue(0x69) == 18); // 'i'
    CHECK(+requite::_getMaskValue(0x6A) == 19); // 'j'
    CHECK(+requite::_getMaskValue(0x6B) == 20); // 'k'
    CHECK(+requite::_getMaskValue(0x6C) == 21); // 'l'
    CHECK(+requite::_getMaskValue(0x6D) == 22); // 'm'
    CHECK(+requite::_getMaskValue(0x6E) == 23); // 'n'
    CHECK(+requite::_getMaskValue(0x6F) == 24); // 'o'
    CHECK(+requite::_getMaskValue(0x70) == 25); // 'p'
    CHECK(+requite::_getMaskValue(0x71) == 26); // 'q'
    CHECK(+requite::_getMaskValue(0x72) == 27); // 'r'
    CHECK(+requite::_getMaskValue(0x73) == 28); // 's'
    CHECK(+requite::_getMaskValue(0x74) == 29); // 't'
    CHECK(+requite::_getMaskValue(0x75) == 30); // 'u'
    CHECK(+requite::_getMaskValue(0x76) == 31); // 'v'
    CHECK(+requite::_getMaskValue(0x77) == 32); // 'w'
    CHECK(+requite::_getMaskValue(0x78) == 33); // 'x'
    CHECK(+requite::_getMaskValue(0x79) == 34); // 'y'
    CHECK(+requite::_getMaskValue(0x7A) == 35); // 'z'
    CHECK(+requite::_getMaskValue(0x7B) == 0);  // '{'
    CHECK(+requite::_getMaskValue(0x7C) == 0);  // '|'
    CHECK(+requite::_getMaskValue(0x7D) == 0);  // '}'
    CHECK(+requite::_getMaskValue(0x7E) == 0);  // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK(+requite::_getMaskValue(0x7F) == 0); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    // Extended continue characters (0x80 - 0x8F)
    for (unsigned char codeunit = 0x80; codeunit <= 0x8F; ++codeunit) {
      CHECK(+requite::_getMaskValue(codeunit) == 0);
    }

    // Extended 2 byte start characters (0xC0 - 0xDF)
    for (unsigned char codeunit = 0xC0; codeunit <= 0xDF; ++codeunit) {
      CHECK(+requite::_getMaskValue(codeunit) == 1);
    }

    // Extended 3 byte start characters (0xE0 - 0xEF)
    for (unsigned char codeunit = 0xE0; codeunit <= 0xEF; ++codeunit) {
      CHECK(+requite::_getMaskValue(codeunit) == 2);
    }

    // Extended four byte start characters (0xF0 - 0xF7)
    for (unsigned char codeunit = 0xF0; codeunit <= 0xF7; ++codeunit) {
      CHECK(+requite::_getMaskValue(codeunit) == 3);
    }

    // Invalid Extended Characters (0xF8 - 0xFF)
    for (unsigned char codeunit = 0xF8; codeunit >= 0xF8; ++codeunit) {
      CHECK(+requite::_getMaskValue(codeunit) == 0);
    }
  }
}

TEST_CASE("requite::getIsWhitespace(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsWhitespace(0x00)); // NUL
    CHECK_FALSE(requite::getIsWhitespace(0x01)); // SOH
    CHECK_FALSE(requite::getIsWhitespace(0x02)); // STX
    CHECK_FALSE(requite::getIsWhitespace(0x03)); // ETX
    CHECK_FALSE(requite::getIsWhitespace(0x04)); // EOT
    CHECK_FALSE(requite::getIsWhitespace(0x05)); // ENQ
    CHECK_FALSE(requite::getIsWhitespace(0x06)); // ACK
    CHECK_FALSE(requite::getIsWhitespace(0x07)); // BEL
    CHECK_FALSE(requite::getIsWhitespace(0x08)); // BS
    CHECK(requite::getIsWhitespace(0x09));       // HT
    CHECK(requite::getIsWhitespace(0x0A));       // LF
    CHECK(requite::getIsWhitespace(0x0B));       // VT
    CHECK_FALSE(requite::getIsWhitespace(0x0C)); // FF
    CHECK_FALSE(requite::getIsWhitespace(0x0D)); // CR
    CHECK_FALSE(requite::getIsWhitespace(0x0E)); // SO
    CHECK_FALSE(requite::getIsWhitespace(0x0F)); // SI
    CHECK_FALSE(requite::getIsWhitespace(0x10)); // DLE
    CHECK_FALSE(requite::getIsWhitespace(0x11)); // DC1
    CHECK_FALSE(requite::getIsWhitespace(0x12)); // DC2
    CHECK_FALSE(requite::getIsWhitespace(0x13)); // DC3
    CHECK_FALSE(requite::getIsWhitespace(0x14)); // DC4
    CHECK_FALSE(requite::getIsWhitespace(0x15)); // NAK
    CHECK_FALSE(requite::getIsWhitespace(0x16)); // SYN
    CHECK_FALSE(requite::getIsWhitespace(0x17)); // ETB
    CHECK_FALSE(requite::getIsWhitespace(0x18)); // CAN
    CHECK_FALSE(requite::getIsWhitespace(0x19)); // EM
    CHECK_FALSE(requite::getIsWhitespace(0x1A)); // SUB
    CHECK_FALSE(requite::getIsWhitespace(0x1B)); // ESC
    CHECK_FALSE(requite::getIsWhitespace(0x1C)); // FS
    CHECK_FALSE(requite::getIsWhitespace(0x1D)); // GS
    CHECK_FALSE(requite::getIsWhitespace(0x1E)); // RS
    CHECK_FALSE(requite::getIsWhitespace(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK(requite::getIsWhitespace(0x20));       // SP
    CHECK_FALSE(requite::getIsWhitespace(0x21)); // '!'
    CHECK_FALSE(requite::getIsWhitespace(0x22)); // '"'
    CHECK_FALSE(requite::getIsWhitespace(0x23)); // '#'
    CHECK_FALSE(requite::getIsWhitespace(0x24)); // '$'
    CHECK_FALSE(requite::getIsWhitespace(0x25)); // '%'
    CHECK_FALSE(requite::getIsWhitespace(0x26)); // '&'
    CHECK_FALSE(requite::getIsWhitespace(0x27)); // '''
    CHECK_FALSE(requite::getIsWhitespace(0x28)); // '('
    CHECK_FALSE(requite::getIsWhitespace(0x29)); // ')'
    CHECK_FALSE(requite::getIsWhitespace(0x2A)); // '*'
    CHECK_FALSE(requite::getIsWhitespace(0x2B)); // '+'
    CHECK_FALSE(requite::getIsWhitespace(0x2C)); // ','
    CHECK_FALSE(requite::getIsWhitespace(0x2D)); // '-'
    CHECK_FALSE(requite::getIsWhitespace(0x2E)); // '.'
    CHECK_FALSE(requite::getIsWhitespace(0x2F)); // '/'
    CHECK_FALSE(requite::getIsWhitespace(0x30)); // '0'
    CHECK_FALSE(requite::getIsWhitespace(0x31)); // '1'
    CHECK_FALSE(requite::getIsWhitespace(0x32)); // '2'
    CHECK_FALSE(requite::getIsWhitespace(0x33)); // '3'
    CHECK_FALSE(requite::getIsWhitespace(0x34)); // '4'
    CHECK_FALSE(requite::getIsWhitespace(0x35)); // '5'
    CHECK_FALSE(requite::getIsWhitespace(0x36)); // '6'
    CHECK_FALSE(requite::getIsWhitespace(0x37)); // '7'
    CHECK_FALSE(requite::getIsWhitespace(0x38)); // '8'
    CHECK_FALSE(requite::getIsWhitespace(0x39)); // '9'
    CHECK_FALSE(requite::getIsWhitespace(0x3A)); // ':'
    CHECK_FALSE(requite::getIsWhitespace(0x3B)); // ';'
    CHECK_FALSE(requite::getIsWhitespace(0x3C)); // '<'
    CHECK_FALSE(requite::getIsWhitespace(0x3D)); // '='
    CHECK_FALSE(requite::getIsWhitespace(0x3E)); // '>'
    CHECK_FALSE(requite::getIsWhitespace(0x3F)); // '?'
    CHECK_FALSE(requite::getIsWhitespace(0x40)); // '@'
    CHECK_FALSE(requite::getIsWhitespace(0x41)); // 'A'
    CHECK_FALSE(requite::getIsWhitespace(0x42)); // 'B'
    CHECK_FALSE(requite::getIsWhitespace(0x43)); // 'C'
    CHECK_FALSE(requite::getIsWhitespace(0x44)); // 'D'
    CHECK_FALSE(requite::getIsWhitespace(0x45)); // 'E'
    CHECK_FALSE(requite::getIsWhitespace(0x46)); // 'F'
    CHECK_FALSE(requite::getIsWhitespace(0x47)); // 'G'
    CHECK_FALSE(requite::getIsWhitespace(0x48)); // 'H'
    CHECK_FALSE(requite::getIsWhitespace(0x49)); // 'I'
    CHECK_FALSE(requite::getIsWhitespace(0x4A)); // 'J'
    CHECK_FALSE(requite::getIsWhitespace(0x4B)); // 'K'
    CHECK_FALSE(requite::getIsWhitespace(0x4C)); // 'L'
    CHECK_FALSE(requite::getIsWhitespace(0x4D)); // 'M'
    CHECK_FALSE(requite::getIsWhitespace(0x4E)); // 'N'
    CHECK_FALSE(requite::getIsWhitespace(0x4F)); // 'O'
    CHECK_FALSE(requite::getIsWhitespace(0x50)); // 'P'
    CHECK_FALSE(requite::getIsWhitespace(0x51)); // 'Q'
    CHECK_FALSE(requite::getIsWhitespace(0x52)); // 'R'
    CHECK_FALSE(requite::getIsWhitespace(0x53)); // 'S'
    CHECK_FALSE(requite::getIsWhitespace(0x54)); // 'T'
    CHECK_FALSE(requite::getIsWhitespace(0x55)); // 'U'
    CHECK_FALSE(requite::getIsWhitespace(0x56)); // 'V'
    CHECK_FALSE(requite::getIsWhitespace(0x57)); // 'W'
    CHECK_FALSE(requite::getIsWhitespace(0x58)); // 'X'
    CHECK_FALSE(requite::getIsWhitespace(0x59)); // 'Y'
    CHECK_FALSE(requite::getIsWhitespace(0x5A)); // 'Z'
    CHECK_FALSE(requite::getIsWhitespace(0x5B)); // '['
    CHECK_FALSE(requite::getIsWhitespace(0x5C)); // '\'
    CHECK_FALSE(requite::getIsWhitespace(0x5D)); // ']'
    CHECK_FALSE(requite::getIsWhitespace(0x5E)); // '^'
    CHECK_FALSE(requite::getIsWhitespace(0x5F)); // '_'
    CHECK_FALSE(requite::getIsWhitespace(0x60)); // '`'
    CHECK_FALSE(requite::getIsWhitespace(0x61)); // 'a'
    CHECK_FALSE(requite::getIsWhitespace(0x62)); // 'b'
    CHECK_FALSE(requite::getIsWhitespace(0x63)); // 'c'
    CHECK_FALSE(requite::getIsWhitespace(0x64)); // 'd'
    CHECK_FALSE(requite::getIsWhitespace(0x65)); // 'e'
    CHECK_FALSE(requite::getIsWhitespace(0x66)); // 'f'
    CHECK_FALSE(requite::getIsWhitespace(0x67)); // 'g'
    CHECK_FALSE(requite::getIsWhitespace(0x68)); // 'h'
    CHECK_FALSE(requite::getIsWhitespace(0x69)); // 'i'
    CHECK_FALSE(requite::getIsWhitespace(0x6A)); // 'j'
    CHECK_FALSE(requite::getIsWhitespace(0x6B)); // 'k'
    CHECK_FALSE(requite::getIsWhitespace(0x6C)); // 'l'
    CHECK_FALSE(requite::getIsWhitespace(0x6D)); // 'm'
    CHECK_FALSE(requite::getIsWhitespace(0x6E)); // 'n'
    CHECK_FALSE(requite::getIsWhitespace(0x6F)); // 'o'
    CHECK_FALSE(requite::getIsWhitespace(0x70)); // 'p'
    CHECK_FALSE(requite::getIsWhitespace(0x71)); // 'q'
    CHECK_FALSE(requite::getIsWhitespace(0x72)); // 'r'
    CHECK_FALSE(requite::getIsWhitespace(0x73)); // 's'
    CHECK_FALSE(requite::getIsWhitespace(0x74)); // 't'
    CHECK_FALSE(requite::getIsWhitespace(0x75)); // 'u'
    CHECK_FALSE(requite::getIsWhitespace(0x76)); // 'v'
    CHECK_FALSE(requite::getIsWhitespace(0x77)); // 'w'
    CHECK_FALSE(requite::getIsWhitespace(0x78)); // 'x'
    CHECK_FALSE(requite::getIsWhitespace(0x79)); // 'y'
    CHECK_FALSE(requite::getIsWhitespace(0x7A)); // 'z'
    CHECK_FALSE(requite::getIsWhitespace(0x7B)); // '{'
    CHECK_FALSE(requite::getIsWhitespace(0x7C)); // '|'
    CHECK_FALSE(requite::getIsWhitespace(0x7D)); // '}'
    CHECK_FALSE(requite::getIsWhitespace(0x7E)); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsWhitespace(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    for (unsigned char codeunit = 0x80; codeunit >= 0x80; ++codeunit) {
      CHECK_FALSE(requite::getIsWhitespace(codeunit));
    }
  }
}

TEST_CASE("requite::getIsVerticalSpace(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsVerticalSpace(0x00)); // NUL
    CHECK_FALSE(requite::getIsVerticalSpace(0x01)); // SOH
    CHECK_FALSE(requite::getIsVerticalSpace(0x02)); // STX
    CHECK_FALSE(requite::getIsVerticalSpace(0x03)); // ETX
    CHECK_FALSE(requite::getIsVerticalSpace(0x04)); // EOT
    CHECK_FALSE(requite::getIsVerticalSpace(0x05)); // ENQ
    CHECK_FALSE(requite::getIsVerticalSpace(0x06)); // ACK
    CHECK_FALSE(requite::getIsVerticalSpace(0x07)); // BEL
    CHECK_FALSE(requite::getIsVerticalSpace(0x08)); // BS
    CHECK_FALSE(requite::getIsVerticalSpace(0x09)); // HT
    CHECK(requite::getIsVerticalSpace(0x0A));       // LF
    CHECK(requite::getIsVerticalSpace(0x0B));       // VT
    CHECK_FALSE(requite::getIsVerticalSpace(0x0C)); // FF
    CHECK_FALSE(requite::getIsVerticalSpace(0x0D)); // CR
    CHECK_FALSE(requite::getIsVerticalSpace(0x0E)); // SO
    CHECK_FALSE(requite::getIsVerticalSpace(0x0F)); // SI
    CHECK_FALSE(requite::getIsVerticalSpace(0x10)); // DLE
    CHECK_FALSE(requite::getIsVerticalSpace(0x11)); // DC1
    CHECK_FALSE(requite::getIsVerticalSpace(0x12)); // DC2
    CHECK_FALSE(requite::getIsVerticalSpace(0x13)); // DC3
    CHECK_FALSE(requite::getIsVerticalSpace(0x14)); // DC4
    CHECK_FALSE(requite::getIsVerticalSpace(0x15)); // NAK
    CHECK_FALSE(requite::getIsVerticalSpace(0x16)); // SYN
    CHECK_FALSE(requite::getIsVerticalSpace(0x17)); // ETB
    CHECK_FALSE(requite::getIsVerticalSpace(0x18)); // CAN
    CHECK_FALSE(requite::getIsVerticalSpace(0x19)); // EM
    CHECK_FALSE(requite::getIsVerticalSpace(0x1A)); // SUB
    CHECK_FALSE(requite::getIsVerticalSpace(0x1B)); // ESC
    CHECK_FALSE(requite::getIsVerticalSpace(0x1C)); // FS
    CHECK_FALSE(requite::getIsVerticalSpace(0x1D)); // GS
    CHECK_FALSE(requite::getIsVerticalSpace(0x1E)); // RS
    CHECK_FALSE(requite::getIsVerticalSpace(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_FALSE(requite::getIsVerticalSpace(0x20)); // SP
    CHECK_FALSE(requite::getIsVerticalSpace(0x21)); // '!'
    CHECK_FALSE(requite::getIsVerticalSpace(0x22)); // '"'
    CHECK_FALSE(requite::getIsVerticalSpace(0x23)); // '#'
    CHECK_FALSE(requite::getIsVerticalSpace(0x24)); // '$'
    CHECK_FALSE(requite::getIsVerticalSpace(0x25)); // '%'
    CHECK_FALSE(requite::getIsVerticalSpace(0x26)); // '&'
    CHECK_FALSE(requite::getIsVerticalSpace(0x27)); // '''
    CHECK_FALSE(requite::getIsVerticalSpace(0x28)); // '('
    CHECK_FALSE(requite::getIsVerticalSpace(0x29)); // ')'
    CHECK_FALSE(requite::getIsVerticalSpace(0x2A)); // '*'
    CHECK_FALSE(requite::getIsVerticalSpace(0x2B)); // '+'
    CHECK_FALSE(requite::getIsVerticalSpace(0x2C)); // ','
    CHECK_FALSE(requite::getIsVerticalSpace(0x2D)); // '-'
    CHECK_FALSE(requite::getIsVerticalSpace(0x2E)); // '.'
    CHECK_FALSE(requite::getIsVerticalSpace(0x2F)); // '/'
    CHECK_FALSE(requite::getIsVerticalSpace(0x30)); // '0'
    CHECK_FALSE(requite::getIsVerticalSpace(0x31)); // '1'
    CHECK_FALSE(requite::getIsVerticalSpace(0x32)); // '2'
    CHECK_FALSE(requite::getIsVerticalSpace(0x33)); // '3'
    CHECK_FALSE(requite::getIsVerticalSpace(0x34)); // '4'
    CHECK_FALSE(requite::getIsVerticalSpace(0x35)); // '5'
    CHECK_FALSE(requite::getIsVerticalSpace(0x36)); // '6'
    CHECK_FALSE(requite::getIsVerticalSpace(0x37)); // '7'
    CHECK_FALSE(requite::getIsVerticalSpace(0x38)); // '8'
    CHECK_FALSE(requite::getIsVerticalSpace(0x39)); // '9'
    CHECK_FALSE(requite::getIsVerticalSpace(0x3A)); // ':'
    CHECK_FALSE(requite::getIsVerticalSpace(0x3B)); // ';'
    CHECK_FALSE(requite::getIsVerticalSpace(0x3C)); // '<'
    CHECK_FALSE(requite::getIsVerticalSpace(0x3D)); // '='
    CHECK_FALSE(requite::getIsVerticalSpace(0x3E)); // '>'
    CHECK_FALSE(requite::getIsVerticalSpace(0x3F)); // '?'
    CHECK_FALSE(requite::getIsVerticalSpace(0x40)); // '@'
    CHECK_FALSE(requite::getIsVerticalSpace(0x41)); // 'A'
    CHECK_FALSE(requite::getIsVerticalSpace(0x42)); // 'B'
    CHECK_FALSE(requite::getIsVerticalSpace(0x43)); // 'C'
    CHECK_FALSE(requite::getIsVerticalSpace(0x44)); // 'D'
    CHECK_FALSE(requite::getIsVerticalSpace(0x45)); // 'E'
    CHECK_FALSE(requite::getIsVerticalSpace(0x46)); // 'F'
    CHECK_FALSE(requite::getIsVerticalSpace(0x47)); // 'G'
    CHECK_FALSE(requite::getIsVerticalSpace(0x48)); // 'H'
    CHECK_FALSE(requite::getIsVerticalSpace(0x49)); // 'I'
    CHECK_FALSE(requite::getIsVerticalSpace(0x4A)); // 'J'
    CHECK_FALSE(requite::getIsVerticalSpace(0x4B)); // 'K'
    CHECK_FALSE(requite::getIsVerticalSpace(0x4C)); // 'L'
    CHECK_FALSE(requite::getIsVerticalSpace(0x4D)); // 'M'
    CHECK_FALSE(requite::getIsVerticalSpace(0x4E)); // 'N'
    CHECK_FALSE(requite::getIsVerticalSpace(0x4F)); // 'O'
    CHECK_FALSE(requite::getIsVerticalSpace(0x50)); // 'P'
    CHECK_FALSE(requite::getIsVerticalSpace(0x51)); // 'Q'
    CHECK_FALSE(requite::getIsVerticalSpace(0x52)); // 'R'
    CHECK_FALSE(requite::getIsVerticalSpace(0x53)); // 'S'
    CHECK_FALSE(requite::getIsVerticalSpace(0x54)); // 'T'
    CHECK_FALSE(requite::getIsVerticalSpace(0x55)); // 'U'
    CHECK_FALSE(requite::getIsVerticalSpace(0x56)); // 'V'
    CHECK_FALSE(requite::getIsVerticalSpace(0x57)); // 'W'
    CHECK_FALSE(requite::getIsVerticalSpace(0x58)); // 'X'
    CHECK_FALSE(requite::getIsVerticalSpace(0x59)); // 'Y'
    CHECK_FALSE(requite::getIsVerticalSpace(0x5A)); // 'Z'
    CHECK_FALSE(requite::getIsVerticalSpace(0x5B)); // '['
    CHECK_FALSE(requite::getIsVerticalSpace(0x5C)); // '\'
    CHECK_FALSE(requite::getIsVerticalSpace(0x5D)); // ']'
    CHECK_FALSE(requite::getIsVerticalSpace(0x5E)); // '^'
    CHECK_FALSE(requite::getIsVerticalSpace(0x5F)); // '_'
    CHECK_FALSE(requite::getIsVerticalSpace(0x60)); // '`'
    CHECK_FALSE(requite::getIsVerticalSpace(0x61)); // 'a'
    CHECK_FALSE(requite::getIsVerticalSpace(0x62)); // 'b'
    CHECK_FALSE(requite::getIsVerticalSpace(0x63)); // 'c'
    CHECK_FALSE(requite::getIsVerticalSpace(0x64)); // 'd'
    CHECK_FALSE(requite::getIsVerticalSpace(0x65)); // 'e'
    CHECK_FALSE(requite::getIsVerticalSpace(0x66)); // 'f'
    CHECK_FALSE(requite::getIsVerticalSpace(0x67)); // 'g'
    CHECK_FALSE(requite::getIsVerticalSpace(0x68)); // 'h'
    CHECK_FALSE(requite::getIsVerticalSpace(0x69)); // 'i'
    CHECK_FALSE(requite::getIsVerticalSpace(0x6A)); // 'j'
    CHECK_FALSE(requite::getIsVerticalSpace(0x6B)); // 'k'
    CHECK_FALSE(requite::getIsVerticalSpace(0x6C)); // 'l'
    CHECK_FALSE(requite::getIsVerticalSpace(0x6D)); // 'm'
    CHECK_FALSE(requite::getIsVerticalSpace(0x6E)); // 'n'
    CHECK_FALSE(requite::getIsVerticalSpace(0x6F)); // 'o'
    CHECK_FALSE(requite::getIsVerticalSpace(0x70)); // 'p'
    CHECK_FALSE(requite::getIsVerticalSpace(0x71)); // 'q'
    CHECK_FALSE(requite::getIsVerticalSpace(0x72)); // 'r'
    CHECK_FALSE(requite::getIsVerticalSpace(0x73)); // 's'
    CHECK_FALSE(requite::getIsVerticalSpace(0x74)); // 't'
    CHECK_FALSE(requite::getIsVerticalSpace(0x75)); // 'u'
    CHECK_FALSE(requite::getIsVerticalSpace(0x76)); // 'v'
    CHECK_FALSE(requite::getIsVerticalSpace(0x77)); // 'w'
    CHECK_FALSE(requite::getIsVerticalSpace(0x78)); // 'x'
    CHECK_FALSE(requite::getIsVerticalSpace(0x79)); // 'y'
    CHECK_FALSE(requite::getIsVerticalSpace(0x7A)); // 'z'
    CHECK_FALSE(requite::getIsVerticalSpace(0x7B)); // '{'
    CHECK_FALSE(requite::getIsVerticalSpace(0x7C)); // '|'
    CHECK_FALSE(requite::getIsVerticalSpace(0x7D)); // '}'
    CHECK_FALSE(requite::getIsVerticalSpace(0x7E)); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsVerticalSpace(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    for (unsigned char codeunit = 0x80; codeunit >= 0x80; ++codeunit) {
      CHECK_FALSE(requite::getIsVerticalSpace(codeunit));
    }
  }
}

TEST_CASE("requite::getIsDecimalDigit(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsDecimalDigit(0x00)); // NUL
    CHECK_FALSE(requite::getIsDecimalDigit(0x01)); // SOH
    CHECK_FALSE(requite::getIsDecimalDigit(0x02)); // STX
    CHECK_FALSE(requite::getIsDecimalDigit(0x03)); // ETX
    CHECK_FALSE(requite::getIsDecimalDigit(0x04)); // EOT
    CHECK_FALSE(requite::getIsDecimalDigit(0x05)); // ENQ
    CHECK_FALSE(requite::getIsDecimalDigit(0x06)); // ACK
    CHECK_FALSE(requite::getIsDecimalDigit(0x07)); // BEL
    CHECK_FALSE(requite::getIsDecimalDigit(0x08)); // BS
    CHECK_FALSE(requite::getIsDecimalDigit(0x09)); // HT
    CHECK_FALSE(requite::getIsDecimalDigit(0x0A)); // LF
    CHECK_FALSE(requite::getIsDecimalDigit(0x0B)); // VT
    CHECK_FALSE(requite::getIsDecimalDigit(0x0C)); // FF
    CHECK_FALSE(requite::getIsDecimalDigit(0x0D)); // CR
    CHECK_FALSE(requite::getIsDecimalDigit(0x0E)); // SO
    CHECK_FALSE(requite::getIsDecimalDigit(0x0F)); // SI
    CHECK_FALSE(requite::getIsDecimalDigit(0x10)); // DLE
    CHECK_FALSE(requite::getIsDecimalDigit(0x11)); // DC1
    CHECK_FALSE(requite::getIsDecimalDigit(0x12)); // DC2
    CHECK_FALSE(requite::getIsDecimalDigit(0x13)); // DC3
    CHECK_FALSE(requite::getIsDecimalDigit(0x14)); // DC4
    CHECK_FALSE(requite::getIsDecimalDigit(0x15)); // NAK
    CHECK_FALSE(requite::getIsDecimalDigit(0x16)); // SYN
    CHECK_FALSE(requite::getIsDecimalDigit(0x17)); // ETB
    CHECK_FALSE(requite::getIsDecimalDigit(0x18)); // CAN
    CHECK_FALSE(requite::getIsDecimalDigit(0x19)); // EM
    CHECK_FALSE(requite::getIsDecimalDigit(0x1A)); // SUB
    CHECK_FALSE(requite::getIsDecimalDigit(0x1B)); // ESC
    CHECK_FALSE(requite::getIsDecimalDigit(0x1C)); // FS
    CHECK_FALSE(requite::getIsDecimalDigit(0x1D)); // GS
    CHECK_FALSE(requite::getIsDecimalDigit(0x1E)); // RS
    CHECK_FALSE(requite::getIsDecimalDigit(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_FALSE(requite::getIsDecimalDigit(0x20)); // SP
    CHECK_FALSE(requite::getIsDecimalDigit(0x21)); // '!'
    CHECK_FALSE(requite::getIsDecimalDigit(0x22)); // '"'
    CHECK_FALSE(requite::getIsDecimalDigit(0x23)); // '#'
    CHECK_FALSE(requite::getIsDecimalDigit(0x24)); // '$'
    CHECK_FALSE(requite::getIsDecimalDigit(0x25)); // '%'
    CHECK_FALSE(requite::getIsDecimalDigit(0x26)); // '&'
    CHECK_FALSE(requite::getIsDecimalDigit(0x27)); // '''
    CHECK_FALSE(requite::getIsDecimalDigit(0x28)); // '('
    CHECK_FALSE(requite::getIsDecimalDigit(0x29)); // ')'
    CHECK_FALSE(requite::getIsDecimalDigit(0x2A)); // '*'
    CHECK_FALSE(requite::getIsDecimalDigit(0x2B)); // '+'
    CHECK_FALSE(requite::getIsDecimalDigit(0x2C)); // ','
    CHECK_FALSE(requite::getIsDecimalDigit(0x2D)); // '-'
    CHECK_FALSE(requite::getIsDecimalDigit(0x2E)); // '.'
    CHECK_FALSE(requite::getIsDecimalDigit(0x2F)); // '/'
    CHECK(requite::getIsDecimalDigit(0x30));       // '0'
    CHECK(requite::getIsDecimalDigit(0x31));       // '1'
    CHECK(requite::getIsDecimalDigit(0x32));       // '2'
    CHECK(requite::getIsDecimalDigit(0x33));       // '3'
    CHECK(requite::getIsDecimalDigit(0x34));       // '4'
    CHECK(requite::getIsDecimalDigit(0x35));       // '5'
    CHECK(requite::getIsDecimalDigit(0x36));       // '6'
    CHECK(requite::getIsDecimalDigit(0x37));       // '7'
    CHECK(requite::getIsDecimalDigit(0x38));       // '8'
    CHECK(requite::getIsDecimalDigit(0x39));       // '9'
    CHECK_FALSE(requite::getIsDecimalDigit(0x3A)); // ':'
    CHECK_FALSE(requite::getIsDecimalDigit(0x3B)); // ';'
    CHECK_FALSE(requite::getIsDecimalDigit(0x3C)); // '<'
    CHECK_FALSE(requite::getIsDecimalDigit(0x3D)); // '='
    CHECK_FALSE(requite::getIsDecimalDigit(0x3E)); // '>'
    CHECK_FALSE(requite::getIsDecimalDigit(0x3F)); // '?'
    CHECK_FALSE(requite::getIsDecimalDigit(0x40)); // '@'
    CHECK_FALSE(requite::getIsDecimalDigit(0x41)); // 'A'
    CHECK_FALSE(requite::getIsDecimalDigit(0x42)); // 'B'
    CHECK_FALSE(requite::getIsDecimalDigit(0x43)); // 'C'
    CHECK_FALSE(requite::getIsDecimalDigit(0x44)); // 'D'
    CHECK_FALSE(requite::getIsDecimalDigit(0x45)); // 'E'
    CHECK_FALSE(requite::getIsDecimalDigit(0x46)); // 'F'
    CHECK_FALSE(requite::getIsDecimalDigit(0x47)); // 'G'
    CHECK_FALSE(requite::getIsDecimalDigit(0x48)); // 'H'
    CHECK_FALSE(requite::getIsDecimalDigit(0x49)); // 'I'
    CHECK_FALSE(requite::getIsDecimalDigit(0x4A)); // 'J'
    CHECK_FALSE(requite::getIsDecimalDigit(0x4B)); // 'K'
    CHECK_FALSE(requite::getIsDecimalDigit(0x4C)); // 'L'
    CHECK_FALSE(requite::getIsDecimalDigit(0x4D)); // 'M'
    CHECK_FALSE(requite::getIsDecimalDigit(0x4E)); // 'N'
    CHECK_FALSE(requite::getIsDecimalDigit(0x4F)); // 'O'
    CHECK_FALSE(requite::getIsDecimalDigit(0x50)); // 'P'
    CHECK_FALSE(requite::getIsDecimalDigit(0x51)); // 'Q'
    CHECK_FALSE(requite::getIsDecimalDigit(0x52)); // 'R'
    CHECK_FALSE(requite::getIsDecimalDigit(0x53)); // 'S'
    CHECK_FALSE(requite::getIsDecimalDigit(0x54)); // 'T'
    CHECK_FALSE(requite::getIsDecimalDigit(0x55)); // 'U'
    CHECK_FALSE(requite::getIsDecimalDigit(0x56)); // 'V'
    CHECK_FALSE(requite::getIsDecimalDigit(0x57)); // 'W'
    CHECK_FALSE(requite::getIsDecimalDigit(0x58)); // 'X'
    CHECK_FALSE(requite::getIsDecimalDigit(0x59)); // 'Y'
    CHECK_FALSE(requite::getIsDecimalDigit(0x5A)); // 'Z'
    CHECK_FALSE(requite::getIsDecimalDigit(0x5B)); // '['
    CHECK_FALSE(requite::getIsDecimalDigit(0x5C)); // '\'
    CHECK_FALSE(requite::getIsDecimalDigit(0x5D)); // ']'
    CHECK_FALSE(requite::getIsDecimalDigit(0x5E)); // '^'
    CHECK_FALSE(requite::getIsDecimalDigit(0x5F)); // '_'
    CHECK_FALSE(requite::getIsDecimalDigit(0x60)); // '`'
    CHECK_FALSE(requite::getIsDecimalDigit(0x61)); // 'a'
    CHECK_FALSE(requite::getIsDecimalDigit(0x62)); // 'b'
    CHECK_FALSE(requite::getIsDecimalDigit(0x63)); // 'c'
    CHECK_FALSE(requite::getIsDecimalDigit(0x64)); // 'd'
    CHECK_FALSE(requite::getIsDecimalDigit(0x65)); // 'e'
    CHECK_FALSE(requite::getIsDecimalDigit(0x66)); // 'f'
    CHECK_FALSE(requite::getIsDecimalDigit(0x67)); // 'g'
    CHECK_FALSE(requite::getIsDecimalDigit(0x68)); // 'h'
    CHECK_FALSE(requite::getIsDecimalDigit(0x69)); // 'i'
    CHECK_FALSE(requite::getIsDecimalDigit(0x6A)); // 'j'
    CHECK_FALSE(requite::getIsDecimalDigit(0x6B)); // 'k'
    CHECK_FALSE(requite::getIsDecimalDigit(0x6C)); // 'l'
    CHECK_FALSE(requite::getIsDecimalDigit(0x6D)); // 'm'
    CHECK_FALSE(requite::getIsDecimalDigit(0x6E)); // 'n'
    CHECK_FALSE(requite::getIsDecimalDigit(0x6F)); // 'o'
    CHECK_FALSE(requite::getIsDecimalDigit(0x70)); // 'p'
    CHECK_FALSE(requite::getIsDecimalDigit(0x71)); // 'q'
    CHECK_FALSE(requite::getIsDecimalDigit(0x72)); // 'r'
    CHECK_FALSE(requite::getIsDecimalDigit(0x73)); // 's'
    CHECK_FALSE(requite::getIsDecimalDigit(0x74)); // 't'
    CHECK_FALSE(requite::getIsDecimalDigit(0x75)); // 'u'
    CHECK_FALSE(requite::getIsDecimalDigit(0x76)); // 'v'
    CHECK_FALSE(requite::getIsDecimalDigit(0x77)); // 'w'
    CHECK_FALSE(requite::getIsDecimalDigit(0x78)); // 'x'
    CHECK_FALSE(requite::getIsDecimalDigit(0x79)); // 'y'
    CHECK_FALSE(requite::getIsDecimalDigit(0x7A)); // 'z'
    CHECK_FALSE(requite::getIsDecimalDigit(0x7B)); // '{'
    CHECK_FALSE(requite::getIsDecimalDigit(0x7C)); // '|'
    CHECK_FALSE(requite::getIsDecimalDigit(0x7D)); // '}'
    CHECK_FALSE(requite::getIsDecimalDigit(0x7E)); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsDecimalDigit(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    for (unsigned char codeunit = 0x80; codeunit >= 0x80; ++codeunit) {
      CHECK_FALSE(requite::getIsDecimalDigit(codeunit));
    }
  }
}

TEST_CASE("requite::getIsDigit(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsDigit(0x00)); // NUL
    CHECK_FALSE(requite::getIsDigit(0x01)); // SOH
    CHECK_FALSE(requite::getIsDigit(0x02)); // STX
    CHECK_FALSE(requite::getIsDigit(0x03)); // ETX
    CHECK_FALSE(requite::getIsDigit(0x04)); // EOT
    CHECK_FALSE(requite::getIsDigit(0x05)); // ENQ
    CHECK_FALSE(requite::getIsDigit(0x06)); // ACK
    CHECK_FALSE(requite::getIsDigit(0x07)); // BEL
    CHECK_FALSE(requite::getIsDigit(0x08)); // BS
    CHECK_FALSE(requite::getIsDigit(0x09)); // HT
    CHECK_FALSE(requite::getIsDigit(0x0A)); // LF
    CHECK_FALSE(requite::getIsDigit(0x0B)); // VT
    CHECK_FALSE(requite::getIsDigit(0x0C)); // FF
    CHECK_FALSE(requite::getIsDigit(0x0D)); // CR
    CHECK_FALSE(requite::getIsDigit(0x0E)); // SO
    CHECK_FALSE(requite::getIsDigit(0x0F)); // SI
    CHECK_FALSE(requite::getIsDigit(0x10)); // DLE
    CHECK_FALSE(requite::getIsDigit(0x11)); // DC1
    CHECK_FALSE(requite::getIsDigit(0x12)); // DC2
    CHECK_FALSE(requite::getIsDigit(0x13)); // DC3
    CHECK_FALSE(requite::getIsDigit(0x14)); // DC4
    CHECK_FALSE(requite::getIsDigit(0x15)); // NAK
    CHECK_FALSE(requite::getIsDigit(0x16)); // SYN
    CHECK_FALSE(requite::getIsDigit(0x17)); // ETB
    CHECK_FALSE(requite::getIsDigit(0x18)); // CAN
    CHECK_FALSE(requite::getIsDigit(0x19)); // EM
    CHECK_FALSE(requite::getIsDigit(0x1A)); // SUB
    CHECK_FALSE(requite::getIsDigit(0x1B)); // ESC
    CHECK_FALSE(requite::getIsDigit(0x1C)); // FS
    CHECK_FALSE(requite::getIsDigit(0x1D)); // GS
    CHECK_FALSE(requite::getIsDigit(0x1E)); // RS
    CHECK_FALSE(requite::getIsDigit(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_FALSE(requite::getIsDigit(0x20)); // SP
    CHECK_FALSE(requite::getIsDigit(0x21)); // '!'
    CHECK_FALSE(requite::getIsDigit(0x22)); // '"'
    CHECK_FALSE(requite::getIsDigit(0x23)); // '#'
    CHECK_FALSE(requite::getIsDigit(0x24)); // '$'
    CHECK_FALSE(requite::getIsDigit(0x25)); // '%'
    CHECK_FALSE(requite::getIsDigit(0x26)); // '&'
    CHECK_FALSE(requite::getIsDigit(0x27)); // '''
    CHECK_FALSE(requite::getIsDigit(0x28)); // '('
    CHECK_FALSE(requite::getIsDigit(0x29)); // ')'
    CHECK_FALSE(requite::getIsDigit(0x2A)); // '*'
    CHECK_FALSE(requite::getIsDigit(0x2B)); // '+'
    CHECK_FALSE(requite::getIsDigit(0x2C)); // ','
    CHECK_FALSE(requite::getIsDigit(0x2D)); // '-'
    CHECK(requite::getIsDigit(0x2E));       // '.'
    CHECK_FALSE(requite::getIsDigit(0x2F)); // '/'
    CHECK(requite::getIsDigit(0x30));       // '0'
    CHECK(requite::getIsDigit(0x31));       // '1'
    CHECK(requite::getIsDigit(0x32));       // '2'
    CHECK(requite::getIsDigit(0x33));       // '3'
    CHECK(requite::getIsDigit(0x34));       // '4'
    CHECK(requite::getIsDigit(0x35));       // '5'
    CHECK(requite::getIsDigit(0x36));       // '6'
    CHECK(requite::getIsDigit(0x37));       // '7'
    CHECK(requite::getIsDigit(0x38));       // '8'
    CHECK(requite::getIsDigit(0x39));       // '9'
    CHECK(requite::getIsDigit(0x3A));       // ':'
    CHECK_FALSE(requite::getIsDigit(0x3B)); // ';'
    CHECK_FALSE(requite::getIsDigit(0x3C)); // '<'
    CHECK_FALSE(requite::getIsDigit(0x3D)); // '='
    CHECK_FALSE(requite::getIsDigit(0x3E)); // '>'
    CHECK_FALSE(requite::getIsDigit(0x3F)); // '?'
    CHECK_FALSE(requite::getIsDigit(0x40)); // '@'
    CHECK(requite::getIsDigit(0x41));       // 'A'
    CHECK(requite::getIsDigit(0x42));       // 'B'
    CHECK(requite::getIsDigit(0x43));       // 'C'
    CHECK(requite::getIsDigit(0x44));       // 'D'
    CHECK(requite::getIsDigit(0x45));       // 'E'
    CHECK(requite::getIsDigit(0x46));       // 'F'
    CHECK(requite::getIsDigit(0x47));       // 'G'
    CHECK(requite::getIsDigit(0x48));       // 'H'
    CHECK(requite::getIsDigit(0x49));       // 'I'
    CHECK(requite::getIsDigit(0x4A));       // 'J'
    CHECK(requite::getIsDigit(0x4B));       // 'K'
    CHECK(requite::getIsDigit(0x4C));       // 'L'
    CHECK(requite::getIsDigit(0x4D));       // 'M'
    CHECK(requite::getIsDigit(0x4E));       // 'N'
    CHECK(requite::getIsDigit(0x4F));       // 'O'
    CHECK(requite::getIsDigit(0x50));       // 'P'
    CHECK(requite::getIsDigit(0x51));       // 'Q'
    CHECK(requite::getIsDigit(0x52));       // 'R'
    CHECK(requite::getIsDigit(0x53));       // 'S'
    CHECK(requite::getIsDigit(0x54));       // 'T'
    CHECK(requite::getIsDigit(0x55));       // 'U'
    CHECK(requite::getIsDigit(0x56));       // 'V'
    CHECK(requite::getIsDigit(0x57));       // 'W'
    CHECK(requite::getIsDigit(0x58));       // 'X'
    CHECK(requite::getIsDigit(0x59));       // 'Y'
    CHECK(requite::getIsDigit(0x5A));       // 'Z'
    CHECK_FALSE(requite::getIsDigit(0x5B)); // '['
    CHECK_FALSE(requite::getIsDigit(0x5C)); // '\'
    CHECK_FALSE(requite::getIsDigit(0x5D)); // ']'
    CHECK_FALSE(requite::getIsDigit(0x5E)); // '^'
    CHECK_FALSE(requite::getIsDigit(0x5F)); // '_'
    CHECK_FALSE(requite::getIsDigit(0x60)); // '`'
    CHECK(requite::getIsDigit(0x61));       // 'a'
    CHECK(requite::getIsDigit(0x62));       // 'b'
    CHECK(requite::getIsDigit(0x63));       // 'c'
    CHECK(requite::getIsDigit(0x64));       // 'd'
    CHECK(requite::getIsDigit(0x65));       // 'e'
    CHECK(requite::getIsDigit(0x66));       // 'f'
    CHECK(requite::getIsDigit(0x67));       // 'g'
    CHECK(requite::getIsDigit(0x68));       // 'h'
    CHECK(requite::getIsDigit(0x69));       // 'i'
    CHECK(requite::getIsDigit(0x6A));       // 'j'
    CHECK(requite::getIsDigit(0x6B));       // 'k'
    CHECK(requite::getIsDigit(0x6C));       // 'l'
    CHECK(requite::getIsDigit(0x6D));       // 'm'
    CHECK(requite::getIsDigit(0x6E));       // 'n'
    CHECK(requite::getIsDigit(0x6F));       // 'o'
    CHECK(requite::getIsDigit(0x70));       // 'p'
    CHECK(requite::getIsDigit(0x71));       // 'q'
    CHECK(requite::getIsDigit(0x72));       // 'r'
    CHECK(requite::getIsDigit(0x73));       // 's'
    CHECK(requite::getIsDigit(0x74));       // 't'
    CHECK(requite::getIsDigit(0x75));       // 'u'
    CHECK(requite::getIsDigit(0x76));       // 'v'
    CHECK(requite::getIsDigit(0x77));       // 'w'
    CHECK(requite::getIsDigit(0x78));       // 'x'
    CHECK(requite::getIsDigit(0x79));       // 'y'
    CHECK(requite::getIsDigit(0x7A));       // 'z'
    CHECK_FALSE(requite::getIsDigit(0x7B)); // '{'
    CHECK_FALSE(requite::getIsDigit(0x7C)); // '|'
    CHECK_FALSE(requite::getIsDigit(0x7D)); // '}'
    CHECK_FALSE(requite::getIsDigit(0x7E)); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsDigit(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    for (unsigned char codeunit = 0x80; codeunit >= 0x80; ++codeunit) {
      CHECK_FALSE(requite::getIsDigit(codeunit));
    }
  }
}

TEST_CASE("requite::getIsNumericLiteral(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsNumericLiteral(0x00)); // NUL
    CHECK_FALSE(requite::getIsNumericLiteral(0x01)); // SOH
    CHECK_FALSE(requite::getIsNumericLiteral(0x02)); // STX
    CHECK_FALSE(requite::getIsNumericLiteral(0x03)); // ETX
    CHECK_FALSE(requite::getIsNumericLiteral(0x04)); // EOT
    CHECK_FALSE(requite::getIsNumericLiteral(0x05)); // ENQ
    CHECK_FALSE(requite::getIsNumericLiteral(0x06)); // ACK
    CHECK_FALSE(requite::getIsNumericLiteral(0x07)); // BEL
    CHECK_FALSE(requite::getIsNumericLiteral(0x08)); // BS
    CHECK_FALSE(requite::getIsNumericLiteral(0x09)); // HT
    CHECK_FALSE(requite::getIsNumericLiteral(0x0A)); // LF
    CHECK_FALSE(requite::getIsNumericLiteral(0x0B)); // VT
    CHECK_FALSE(requite::getIsNumericLiteral(0x0C)); // FF
    CHECK_FALSE(requite::getIsNumericLiteral(0x0D)); // CR
    CHECK_FALSE(requite::getIsNumericLiteral(0x0E)); // SO
    CHECK_FALSE(requite::getIsNumericLiteral(0x0F)); // SI
    CHECK_FALSE(requite::getIsNumericLiteral(0x10)); // DLE
    CHECK_FALSE(requite::getIsNumericLiteral(0x11)); // DC1
    CHECK_FALSE(requite::getIsNumericLiteral(0x12)); // DC2
    CHECK_FALSE(requite::getIsNumericLiteral(0x13)); // DC3
    CHECK_FALSE(requite::getIsNumericLiteral(0x14)); // DC4
    CHECK_FALSE(requite::getIsNumericLiteral(0x15)); // NAK
    CHECK_FALSE(requite::getIsNumericLiteral(0x16)); // SYN
    CHECK_FALSE(requite::getIsNumericLiteral(0x17)); // ETB
    CHECK_FALSE(requite::getIsNumericLiteral(0x18)); // CAN
    CHECK_FALSE(requite::getIsNumericLiteral(0x19)); // EM
    CHECK_FALSE(requite::getIsNumericLiteral(0x1A)); // SUB
    CHECK_FALSE(requite::getIsNumericLiteral(0x1B)); // ESC
    CHECK_FALSE(requite::getIsNumericLiteral(0x1C)); // FS
    CHECK_FALSE(requite::getIsNumericLiteral(0x1D)); // GS
    CHECK_FALSE(requite::getIsNumericLiteral(0x1E)); // RS
    CHECK_FALSE(requite::getIsNumericLiteral(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_FALSE(requite::getIsNumericLiteral(0x20)); // SP
    CHECK_FALSE(requite::getIsNumericLiteral(0x21)); // '!'
    CHECK_FALSE(requite::getIsNumericLiteral(0x22)); // '"'
    CHECK_FALSE(requite::getIsNumericLiteral(0x23)); // '#'
    CHECK_FALSE(requite::getIsNumericLiteral(0x24)); // '$'
    CHECK_FALSE(requite::getIsNumericLiteral(0x25)); // '%'
    CHECK_FALSE(requite::getIsNumericLiteral(0x26)); // '&'
    CHECK_FALSE(requite::getIsNumericLiteral(0x27)); // '''
    CHECK_FALSE(requite::getIsNumericLiteral(0x28)); // '('
    CHECK_FALSE(requite::getIsNumericLiteral(0x29)); // ')'
    CHECK_FALSE(requite::getIsNumericLiteral(0x2A)); // '*'
    CHECK_FALSE(requite::getIsNumericLiteral(0x2B)); // '+'
    CHECK_FALSE(requite::getIsNumericLiteral(0x2C)); // ','
    CHECK_FALSE(requite::getIsNumericLiteral(0x2D)); // '-'
    CHECK(requite::getIsNumericLiteral(0x2E));       // '.'
    CHECK_FALSE(requite::getIsNumericLiteral(0x2F)); // '/'
    CHECK(requite::getIsNumericLiteral(0x30));       // '0'
    CHECK(requite::getIsNumericLiteral(0x31));       // '1'
    CHECK(requite::getIsNumericLiteral(0x32));       // '2'
    CHECK(requite::getIsNumericLiteral(0x33));       // '3'
    CHECK(requite::getIsNumericLiteral(0x34));       // '4'
    CHECK(requite::getIsNumericLiteral(0x35));       // '5'
    CHECK(requite::getIsNumericLiteral(0x36));       // '6'
    CHECK(requite::getIsNumericLiteral(0x37));       // '7'
    CHECK(requite::getIsNumericLiteral(0x38));       // '8'
    CHECK(requite::getIsNumericLiteral(0x39));       // '9'
    CHECK(requite::getIsNumericLiteral(0x3A));       // ':'
    CHECK_FALSE(requite::getIsNumericLiteral(0x3B)); // ';'
    CHECK_FALSE(requite::getIsNumericLiteral(0x3C)); // '<'
    CHECK_FALSE(requite::getIsNumericLiteral(0x3D)); // '='
    CHECK_FALSE(requite::getIsNumericLiteral(0x3E)); // '>'
    CHECK_FALSE(requite::getIsNumericLiteral(0x3F)); // '?'
    CHECK_FALSE(requite::getIsNumericLiteral(0x40)); // '@'
    CHECK(requite::getIsNumericLiteral(0x41));       // 'A'
    CHECK(requite::getIsNumericLiteral(0x42));       // 'B'
    CHECK(requite::getIsNumericLiteral(0x43));       // 'C'
    CHECK(requite::getIsNumericLiteral(0x44));       // 'D'
    CHECK(requite::getIsNumericLiteral(0x45));       // 'E'
    CHECK(requite::getIsNumericLiteral(0x46));       // 'F'
    CHECK(requite::getIsNumericLiteral(0x47));       // 'G'
    CHECK(requite::getIsNumericLiteral(0x48));       // 'H'
    CHECK(requite::getIsNumericLiteral(0x49));       // 'I'
    CHECK(requite::getIsNumericLiteral(0x4A));       // 'J'
    CHECK(requite::getIsNumericLiteral(0x4B));       // 'K'
    CHECK(requite::getIsNumericLiteral(0x4C));       // 'L'
    CHECK(requite::getIsNumericLiteral(0x4D));       // 'M'
    CHECK(requite::getIsNumericLiteral(0x4E));       // 'N'
    CHECK(requite::getIsNumericLiteral(0x4F));       // 'O'
    CHECK(requite::getIsNumericLiteral(0x50));       // 'P'
    CHECK(requite::getIsNumericLiteral(0x51));       // 'Q'
    CHECK(requite::getIsNumericLiteral(0x52));       // 'R'
    CHECK(requite::getIsNumericLiteral(0x53));       // 'S'
    CHECK(requite::getIsNumericLiteral(0x54));       // 'T'
    CHECK(requite::getIsNumericLiteral(0x55));       // 'U'
    CHECK(requite::getIsNumericLiteral(0x56));       // 'V'
    CHECK(requite::getIsNumericLiteral(0x57));       // 'W'
    CHECK(requite::getIsNumericLiteral(0x58));       // 'X'
    CHECK(requite::getIsNumericLiteral(0x59));       // 'Y'
    CHECK(requite::getIsNumericLiteral(0x5A));       // 'Z'
    CHECK_FALSE(requite::getIsNumericLiteral(0x5B)); // '['
    CHECK_FALSE(requite::getIsNumericLiteral(0x5C)); // '\'
    CHECK_FALSE(requite::getIsNumericLiteral(0x5D)); // ']'
    CHECK_FALSE(requite::getIsNumericLiteral(0x5E)); // '^'
    CHECK(requite::getIsNumericLiteral(0x5F));       // '_'
    CHECK_FALSE(requite::getIsNumericLiteral(0x60)); // '`'
    CHECK(requite::getIsNumericLiteral(0x61));       // 'a'
    CHECK(requite::getIsNumericLiteral(0x62));       // 'b'
    CHECK(requite::getIsNumericLiteral(0x63));       // 'c'
    CHECK(requite::getIsNumericLiteral(0x64));       // 'd'
    CHECK(requite::getIsNumericLiteral(0x65));       // 'e'
    CHECK(requite::getIsNumericLiteral(0x66));       // 'f'
    CHECK(requite::getIsNumericLiteral(0x67));       // 'g'
    CHECK(requite::getIsNumericLiteral(0x68));       // 'h'
    CHECK(requite::getIsNumericLiteral(0x69));       // 'i'
    CHECK(requite::getIsNumericLiteral(0x6A));       // 'j'
    CHECK(requite::getIsNumericLiteral(0x6B));       // 'k'
    CHECK(requite::getIsNumericLiteral(0x6C));       // 'l'
    CHECK(requite::getIsNumericLiteral(0x6D));       // 'm'
    CHECK(requite::getIsNumericLiteral(0x6E));       // 'n'
    CHECK(requite::getIsNumericLiteral(0x6F));       // 'o'
    CHECK(requite::getIsNumericLiteral(0x70));       // 'p'
    CHECK(requite::getIsNumericLiteral(0x71));       // 'q'
    CHECK(requite::getIsNumericLiteral(0x72));       // 'r'
    CHECK(requite::getIsNumericLiteral(0x73));       // 's'
    CHECK(requite::getIsNumericLiteral(0x74));       // 't'
    CHECK(requite::getIsNumericLiteral(0x75));       // 'u'
    CHECK(requite::getIsNumericLiteral(0x76));       // 'v'
    CHECK(requite::getIsNumericLiteral(0x77));       // 'w'
    CHECK(requite::getIsNumericLiteral(0x78));       // 'x'
    CHECK(requite::getIsNumericLiteral(0x79));       // 'y'
    CHECK(requite::getIsNumericLiteral(0x7A));       // 'z'
    CHECK_FALSE(requite::getIsNumericLiteral(0x7B)); // '{'
    CHECK_FALSE(requite::getIsNumericLiteral(0x7C)); // '|'
    CHECK_FALSE(requite::getIsNumericLiteral(0x7D)); // '}'
    CHECK_FALSE(requite::getIsNumericLiteral(0x7E)); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsNumericLiteral(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    for (unsigned char codeunit = 0x80; codeunit >= 0x80; ++codeunit) {
      CHECK_FALSE(requite::getIsNumericLiteral(codeunit));
    }
  }
}

TEST_CASE("requite::getDigitBaseMultiplier(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x00),
                          requite::AssertException); // NUL
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x01),
                          requite::AssertException); // SOH
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x02),
                          requite::AssertException); // STX
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x03),
                          requite::AssertException); // ETX
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x04),
                          requite::AssertException); // EOT
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x05),
                          requite::AssertException); // ENQ
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x06),
                          requite::AssertException); // ACK
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x07),
                          requite::AssertException); // BEL
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x08),
                          requite::AssertException); // BS
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x09),
                          requite::AssertException); // HT
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x0A),
                          requite::AssertException); // LF
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x0B),
                          requite::AssertException); // VT
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x0C),
                          requite::AssertException); // FF
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x0D),
                          requite::AssertException); // CR
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x0E),
                          requite::AssertException); // SO
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x0F),
                          requite::AssertException); // SI
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x10),
                          requite::AssertException); // DLE
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x11),
                          requite::AssertException); // DC1
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x12),
                          requite::AssertException); // DC2
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x13),
                          requite::AssertException); // DC3
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x14),
                          requite::AssertException); // DC4
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x15),
                          requite::AssertException); // NAK
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x16),
                          requite::AssertException); // SYN
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x17),
                          requite::AssertException); // ETB
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x18),
                          requite::AssertException); // CAN
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x19),
                          requite::AssertException); // EM
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x1A),
                          requite::AssertException); // SUB
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x1B),
                          requite::AssertException); // ESC
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x1C),
                          requite::AssertException); // FS
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x1D),
                          requite::AssertException); // GS
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x1E),
                          requite::AssertException); // RS
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x1F),
                          requite::AssertException); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x20),
                          requite::AssertException); // SP
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x21),
                          requite::AssertException); // '!'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x22),
                          requite::AssertException); // '"'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x23),
                          requite::AssertException); // '#'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x24),
                          requite::AssertException); // '$'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x25),
                          requite::AssertException); // '%'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x26),
                          requite::AssertException); // '&'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x27),
                          requite::AssertException); // '''
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x28),
                          requite::AssertException); // '('
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x29),
                          requite::AssertException); // ')'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x2A),
                          requite::AssertException); // '*'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x2B),
                          requite::AssertException); // '+'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x2C),
                          requite::AssertException); // ','
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x2D),
                          requite::AssertException);     // '-'
    CHECK(+requite::getDigitBaseMultiplier(0x2E) == 62); // '.'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x2F),
                          requite::AssertException);     // '/'
    CHECK(+requite::getDigitBaseMultiplier(0x30) == 0);  // '0'
    CHECK(+requite::getDigitBaseMultiplier(0x31) == 1);  // '1'
    CHECK(+requite::getDigitBaseMultiplier(0x32) == 2);  // '2'
    CHECK(+requite::getDigitBaseMultiplier(0x33) == 3);  // '3'
    CHECK(+requite::getDigitBaseMultiplier(0x34) == 4);  // '4'
    CHECK(+requite::getDigitBaseMultiplier(0x35) == 5);  // '5'
    CHECK(+requite::getDigitBaseMultiplier(0x36) == 6);  // '6'
    CHECK(+requite::getDigitBaseMultiplier(0x37) == 7);  // '7'
    CHECK(+requite::getDigitBaseMultiplier(0x38) == 8);  // '8'
    CHECK(+requite::getDigitBaseMultiplier(0x39) == 9);  // '9'
    CHECK(+requite::getDigitBaseMultiplier(0x3A) == 63); // ':'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x3B),
                          requite::AssertException); // ';'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x3C),
                          requite::AssertException); // '<'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x3D),
                          requite::AssertException); // '='
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x3E),
                          requite::AssertException); // '>'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x3F),
                          requite::AssertException); // '?'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x40),
                          requite::AssertException);     // '@'
    CHECK(+requite::getDigitBaseMultiplier(0x41) == 36); // 'A'
    CHECK(+requite::getDigitBaseMultiplier(0x42) == 37); // 'B'
    CHECK(+requite::getDigitBaseMultiplier(0x43) == 38); // 'C'
    CHECK(+requite::getDigitBaseMultiplier(0x44) == 39); // 'D'
    CHECK(+requite::getDigitBaseMultiplier(0x45) == 40); // 'E'
    CHECK(+requite::getDigitBaseMultiplier(0x46) == 41); // 'F'
    CHECK(+requite::getDigitBaseMultiplier(0x47) == 42); // 'G'
    CHECK(+requite::getDigitBaseMultiplier(0x48) == 43); // 'H'
    CHECK(+requite::getDigitBaseMultiplier(0x49) == 44); // 'I'
    CHECK(+requite::getDigitBaseMultiplier(0x4A) == 45); // 'J'
    CHECK(+requite::getDigitBaseMultiplier(0x4B) == 46); // 'K'
    CHECK(+requite::getDigitBaseMultiplier(0x4C) == 47); // 'L'
    CHECK(+requite::getDigitBaseMultiplier(0x4D) == 48); // 'M'
    CHECK(+requite::getDigitBaseMultiplier(0x4E) == 49); // 'N'
    CHECK(+requite::getDigitBaseMultiplier(0x4F) == 50); // 'O'
    CHECK(+requite::getDigitBaseMultiplier(0x50) == 51); // 'P'
    CHECK(+requite::getDigitBaseMultiplier(0x51) == 52); // 'Q'
    CHECK(+requite::getDigitBaseMultiplier(0x52) == 53); // 'R'
    CHECK(+requite::getDigitBaseMultiplier(0x53) == 54); // 'S'
    CHECK(+requite::getDigitBaseMultiplier(0x54) == 55); // 'T'
    CHECK(+requite::getDigitBaseMultiplier(0x55) == 56); // 'U'
    CHECK(+requite::getDigitBaseMultiplier(0x56) == 57); // 'V'
    CHECK(+requite::getDigitBaseMultiplier(0x57) == 58); // 'W'
    CHECK(+requite::getDigitBaseMultiplier(0x58) == 59); // 'X'
    CHECK(+requite::getDigitBaseMultiplier(0x59) == 60); // 'Y'
    CHECK(+requite::getDigitBaseMultiplier(0x5A) == 61); // 'Z'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x5B),
                          requite::AssertException); // '['
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x5C),
                          requite::AssertException); // '\'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x5D),
                          requite::AssertException); // ']'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x5E),
                          requite::AssertException); // '^'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x5F),
                          requite::AssertException); // '_'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x60),
                          requite::AssertException);     // '`'
    CHECK(+requite::getDigitBaseMultiplier(0x61) == 10); // 'a'
    CHECK(+requite::getDigitBaseMultiplier(0x62) == 11); // 'b'
    CHECK(+requite::getDigitBaseMultiplier(0x63) == 12); // 'c'
    CHECK(+requite::getDigitBaseMultiplier(0x64) == 13); // 'd'
    CHECK(+requite::getDigitBaseMultiplier(0x65) == 14); // 'e'
    CHECK(+requite::getDigitBaseMultiplier(0x66) == 15); // 'f'
    CHECK(+requite::getDigitBaseMultiplier(0x67) == 16); // 'g'
    CHECK(+requite::getDigitBaseMultiplier(0x68) == 17); // 'h'
    CHECK(+requite::getDigitBaseMultiplier(0x69) == 18); // 'i'
    CHECK(+requite::getDigitBaseMultiplier(0x6A) == 19); // 'j'
    CHECK(+requite::getDigitBaseMultiplier(0x6B) == 20); // 'k'
    CHECK(+requite::getDigitBaseMultiplier(0x6C) == 21); // 'l'
    CHECK(+requite::getDigitBaseMultiplier(0x6D) == 22); // 'm'
    CHECK(+requite::getDigitBaseMultiplier(0x6E) == 23); // 'n'
    CHECK(+requite::getDigitBaseMultiplier(0x6F) == 24); // 'o'
    CHECK(+requite::getDigitBaseMultiplier(0x70) == 25); // 'p'
    CHECK(+requite::getDigitBaseMultiplier(0x71) == 26); // 'q'
    CHECK(+requite::getDigitBaseMultiplier(0x72) == 27); // 'r'
    CHECK(+requite::getDigitBaseMultiplier(0x73) == 28); // 's'
    CHECK(+requite::getDigitBaseMultiplier(0x74) == 29); // 't'
    CHECK(+requite::getDigitBaseMultiplier(0x75) == 30); // 'u'
    CHECK(+requite::getDigitBaseMultiplier(0x76) == 31); // 'v'
    CHECK(+requite::getDigitBaseMultiplier(0x77) == 32); // 'w'
    CHECK(+requite::getDigitBaseMultiplier(0x78) == 33); // 'x'
    CHECK(+requite::getDigitBaseMultiplier(0x79) == 34); // 'y'
    CHECK(+requite::getDigitBaseMultiplier(0x7A) == 35); // 'z'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x7B),
                          requite::AssertException); // '{'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x7C),
                          requite::AssertException); // '|'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x7D),
                          requite::AssertException); // '}'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x7E),
                          requite::AssertException); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getDigitBaseMultiplier(0x7F),
                          requite::AssertException); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    for (unsigned char codeunit = 0x80; codeunit >= 0x80; ++codeunit) {
      CHECK_THROWS_AS_DEBUG(std::ignore =
                                requite::getDigitBaseMultiplier(codeunit),
                            requite::AssertException);
    }
  }
}

TEST_CASE("requite::getIsUppercase(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsUppercase(0x00)); // NUL
    CHECK_FALSE(requite::getIsUppercase(0x01)); // SOH
    CHECK_FALSE(requite::getIsUppercase(0x02)); // STX
    CHECK_FALSE(requite::getIsUppercase(0x03)); // ETX
    CHECK_FALSE(requite::getIsUppercase(0x04)); // EOT
    CHECK_FALSE(requite::getIsUppercase(0x05)); // ENQ
    CHECK_FALSE(requite::getIsUppercase(0x06)); // ACK
    CHECK_FALSE(requite::getIsUppercase(0x07)); // BEL
    CHECK_FALSE(requite::getIsUppercase(0x08)); // BS
    CHECK_FALSE(requite::getIsUppercase(0x09)); // HT
    CHECK_FALSE(requite::getIsUppercase(0x0A)); // LF
    CHECK_FALSE(requite::getIsUppercase(0x0B)); // VT
    CHECK_FALSE(requite::getIsUppercase(0x0C)); // FF
    CHECK_FALSE(requite::getIsUppercase(0x0D)); // CR
    CHECK_FALSE(requite::getIsUppercase(0x0E)); // SO
    CHECK_FALSE(requite::getIsUppercase(0x0F)); // SI
    CHECK_FALSE(requite::getIsUppercase(0x10)); // DLE
    CHECK_FALSE(requite::getIsUppercase(0x11)); // DC1
    CHECK_FALSE(requite::getIsUppercase(0x12)); // DC2
    CHECK_FALSE(requite::getIsUppercase(0x13)); // DC3
    CHECK_FALSE(requite::getIsUppercase(0x14)); // DC4
    CHECK_FALSE(requite::getIsUppercase(0x15)); // NAK
    CHECK_FALSE(requite::getIsUppercase(0x16)); // SYN
    CHECK_FALSE(requite::getIsUppercase(0x17)); // ETB
    CHECK_FALSE(requite::getIsUppercase(0x18)); // CAN
    CHECK_FALSE(requite::getIsUppercase(0x19)); // EM
    CHECK_FALSE(requite::getIsUppercase(0x1A)); // SUB
    CHECK_FALSE(requite::getIsUppercase(0x1B)); // ESC
    CHECK_FALSE(requite::getIsUppercase(0x1C)); // FS
    CHECK_FALSE(requite::getIsUppercase(0x1D)); // GS
    CHECK_FALSE(requite::getIsUppercase(0x1E)); // RS
    CHECK_FALSE(requite::getIsUppercase(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_FALSE(requite::getIsUppercase(0x20)); // SP
    CHECK_FALSE(requite::getIsUppercase(0x21)); // '!'
    CHECK_FALSE(requite::getIsUppercase(0x22)); // '"'
    CHECK_FALSE(requite::getIsUppercase(0x23)); // '#'
    CHECK_FALSE(requite::getIsUppercase(0x24)); // '$'
    CHECK_FALSE(requite::getIsUppercase(0x25)); // '%'
    CHECK_FALSE(requite::getIsUppercase(0x26)); // '&'
    CHECK_FALSE(requite::getIsUppercase(0x27)); // '''
    CHECK_FALSE(requite::getIsUppercase(0x28)); // '('
    CHECK_FALSE(requite::getIsUppercase(0x29)); // ')'
    CHECK_FALSE(requite::getIsUppercase(0x2A)); // '*'
    CHECK_FALSE(requite::getIsUppercase(0x2B)); // '+'
    CHECK_FALSE(requite::getIsUppercase(0x2C)); // ','
    CHECK_FALSE(requite::getIsUppercase(0x2D)); // '-'
    CHECK_FALSE(requite::getIsUppercase(0x2E)); // '.'
    CHECK_FALSE(requite::getIsUppercase(0x2F)); // '/'
    CHECK_FALSE(requite::getIsUppercase(0x30)); // '0'
    CHECK_FALSE(requite::getIsUppercase(0x31)); // '1'
    CHECK_FALSE(requite::getIsUppercase(0x32)); // '2'
    CHECK_FALSE(requite::getIsUppercase(0x33)); // '3'
    CHECK_FALSE(requite::getIsUppercase(0x34)); // '4'
    CHECK_FALSE(requite::getIsUppercase(0x35)); // '5'
    CHECK_FALSE(requite::getIsUppercase(0x36)); // '6'
    CHECK_FALSE(requite::getIsUppercase(0x37)); // '7'
    CHECK_FALSE(requite::getIsUppercase(0x38)); // '8'
    CHECK_FALSE(requite::getIsUppercase(0x39)); // '9'
    CHECK_FALSE(requite::getIsUppercase(0x3A)); // ':'
    CHECK_FALSE(requite::getIsUppercase(0x3B)); // ';'
    CHECK_FALSE(requite::getIsUppercase(0x3C)); // '<'
    CHECK_FALSE(requite::getIsUppercase(0x3D)); // '='
    CHECK_FALSE(requite::getIsUppercase(0x3E)); // '>'
    CHECK_FALSE(requite::getIsUppercase(0x3F)); // '?'
    CHECK_FALSE(requite::getIsUppercase(0x40)); // '@'
    CHECK(requite::getIsUppercase(0x41));       // 'A'
    CHECK(requite::getIsUppercase(0x42));       // 'B'
    CHECK(requite::getIsUppercase(0x43));       // 'C'
    CHECK(requite::getIsUppercase(0x44));       // 'D'
    CHECK(requite::getIsUppercase(0x45));       // 'E'
    CHECK(requite::getIsUppercase(0x46));       // 'F'
    CHECK(requite::getIsUppercase(0x47));       // 'G'
    CHECK(requite::getIsUppercase(0x48));       // 'H'
    CHECK(requite::getIsUppercase(0x49));       // 'I'
    CHECK(requite::getIsUppercase(0x4A));       // 'J'
    CHECK(requite::getIsUppercase(0x4B));       // 'K'
    CHECK(requite::getIsUppercase(0x4C));       // 'L'
    CHECK(requite::getIsUppercase(0x4D));       // 'M'
    CHECK(requite::getIsUppercase(0x4E));       // 'N'
    CHECK(requite::getIsUppercase(0x4F));       // 'O'
    CHECK(requite::getIsUppercase(0x50));       // 'P'
    CHECK(requite::getIsUppercase(0x51));       // 'Q'
    CHECK(requite::getIsUppercase(0x52));       // 'R'
    CHECK(requite::getIsUppercase(0x53));       // 'S'
    CHECK(requite::getIsUppercase(0x54));       // 'T'
    CHECK(requite::getIsUppercase(0x55));       // 'U'
    CHECK(requite::getIsUppercase(0x56));       // 'V'
    CHECK(requite::getIsUppercase(0x57));       // 'W'
    CHECK(requite::getIsUppercase(0x58));       // 'X'
    CHECK(requite::getIsUppercase(0x59));       // 'Y'
    CHECK(requite::getIsUppercase(0x5A));       // 'Z'
    CHECK_FALSE(requite::getIsUppercase(0x5B)); // '['
    CHECK_FALSE(requite::getIsUppercase(0x5C)); // '\'
    CHECK_FALSE(requite::getIsUppercase(0x5D)); // ']'
    CHECK_FALSE(requite::getIsUppercase(0x5E)); // '^'
    CHECK_FALSE(requite::getIsUppercase(0x5F)); // '_'
    CHECK_FALSE(requite::getIsUppercase(0x60)); // '`'
    CHECK_FALSE(requite::getIsUppercase(0x61)); // 'a'
    CHECK_FALSE(requite::getIsUppercase(0x62)); // 'b'
    CHECK_FALSE(requite::getIsUppercase(0x63)); // 'c'
    CHECK_FALSE(requite::getIsUppercase(0x64)); // 'd'
    CHECK_FALSE(requite::getIsUppercase(0x65)); // 'e'
    CHECK_FALSE(requite::getIsUppercase(0x66)); // 'f'
    CHECK_FALSE(requite::getIsUppercase(0x67)); // 'g'
    CHECK_FALSE(requite::getIsUppercase(0x68)); // 'h'
    CHECK_FALSE(requite::getIsUppercase(0x69)); // 'i'
    CHECK_FALSE(requite::getIsUppercase(0x6A)); // 'j'
    CHECK_FALSE(requite::getIsUppercase(0x6B)); // 'k'
    CHECK_FALSE(requite::getIsUppercase(0x6C)); // 'l'
    CHECK_FALSE(requite::getIsUppercase(0x6D)); // 'm'
    CHECK_FALSE(requite::getIsUppercase(0x6E)); // 'n'
    CHECK_FALSE(requite::getIsUppercase(0x6F)); // 'o'
    CHECK_FALSE(requite::getIsUppercase(0x70)); // 'p'
    CHECK_FALSE(requite::getIsUppercase(0x71)); // 'q'
    CHECK_FALSE(requite::getIsUppercase(0x72)); // 'r'
    CHECK_FALSE(requite::getIsUppercase(0x73)); // 's'
    CHECK_FALSE(requite::getIsUppercase(0x74)); // 't'
    CHECK_FALSE(requite::getIsUppercase(0x75)); // 'u'
    CHECK_FALSE(requite::getIsUppercase(0x76)); // 'v'
    CHECK_FALSE(requite::getIsUppercase(0x77)); // 'w'
    CHECK_FALSE(requite::getIsUppercase(0x78)); // 'x'
    CHECK_FALSE(requite::getIsUppercase(0x79)); // 'y'
    CHECK_FALSE(requite::getIsUppercase(0x7A)); // 'z'
    CHECK_FALSE(requite::getIsUppercase(0x7B)); // '{'
    CHECK_FALSE(requite::getIsUppercase(0x7C)); // '|'
    CHECK_FALSE(requite::getIsUppercase(0x7D)); // '}'
    CHECK_FALSE(requite::getIsUppercase(0x7E)); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsUppercase(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    for (unsigned char codeunit = 0x80; codeunit >= 0x80; ++codeunit) {
      CHECK_FALSE(requite::getIsUppercase(codeunit));
    }
  }
}

TEST_CASE("requite::getLowercase(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK(+requite::getLowercase(0x00) == 0x00); // NUL -> NUL (no change)
    CHECK(+requite::getLowercase(0x01) == 0x01); // SOH -> SOH (no change)
    CHECK(+requite::getLowercase(0x02) == 0x02); // STX -> STX (no change)
    CHECK(+requite::getLowercase(0x03) == 0x03); // ETX -> ETX (no change)
    CHECK(+requite::getLowercase(0x04) == 0x04); // EOT -> EOT (no change)
    CHECK(+requite::getLowercase(0x05) == 0x05); // ENQ -> ENQ (no change)
    CHECK(+requite::getLowercase(0x06) == 0x06); // ACK -> ACK (no change)
    CHECK(+requite::getLowercase(0x07) == 0x07); // BEL -> BEL (no change)
    CHECK(+requite::getLowercase(0x08) == 0x08); // BS  -> BS  (no change)
    CHECK(+requite::getLowercase(0x09) == 0x09); // HT  -> HT  (no change)
    CHECK(+requite::getLowercase(0x0A) == 0x0A); // LF  -> LF  (no change)
    CHECK(+requite::getLowercase(0x0B) == 0x0B); // VT  -> VT  (no change)
    CHECK(+requite::getLowercase(0x0C) == 0x0C); // FF  -> FF  (no change)
    CHECK(+requite::getLowercase(0x0D) == 0x0D); // CR  -> CR  (no change)
    CHECK(+requite::getLowercase(0x0E) == 0x0E); // SO  -> SO  (no change)
    CHECK(+requite::getLowercase(0x0F) == 0x0F); // SI  -> SI  (no change)
    CHECK(+requite::getLowercase(0x10) == 0x10); // DLE -> DLE (no change)
    CHECK(+requite::getLowercase(0x11) == 0x11); // DC1 -> DC1 (no change)
    CHECK(+requite::getLowercase(0x12) == 0x12); // DC2 -> DC2 (no change)
    CHECK(+requite::getLowercase(0x13) == 0x13); // DC3 -> DC3 (no change)
    CHECK(+requite::getLowercase(0x14) == 0x14); // DC4 -> DC4 (no change)
    CHECK(+requite::getLowercase(0x15) == 0x15); // NAK -> NAK (no change)
    CHECK(+requite::getLowercase(0x16) == 0x16); // SYN -> SYN (no change)
    CHECK(+requite::getLowercase(0x17) == 0x17); // ETB -> ETB (no change)
    CHECK(+requite::getLowercase(0x18) == 0x18); // CAN -> CAN (no change)
    CHECK(+requite::getLowercase(0x19) == 0x19); // EM - > EM  (no change)
    CHECK(+requite::getLowercase(0x1A) == 0x1A); // SUB -> SUB (no change)
    CHECK(+requite::getLowercase(0x1B) == 0x1B); // ESC -> ESC (no change)
    CHECK(+requite::getLowercase(0x1C) == 0x1C); // FS  -> FS  (no change)
    CHECK(+requite::getLowercase(0x1D) == 0x1D); // GS  -> GS  (no change)
    CHECK(+requite::getLowercase(0x1E) == 0x1E); // RS  -> RS  (no change)
    CHECK(+requite::getLowercase(0x1F) == 0x1F); // US  -> US  (no change)
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK(+requite::getLowercase(0x20) == 0x20); // SP  -> SP  (no change)
    CHECK(+requite::getLowercase(0x21) == 0x21); // '!' -> '!' (no change)
    CHECK(+requite::getLowercase(0x22) == 0x22); // '"' -> '"' (no change)
    CHECK(+requite::getLowercase(0x23) == 0x23); // '#' -> '#' (no change)
    CHECK(+requite::getLowercase(0x24) == 0x24); // '$' -> '$' (no change)
    CHECK(+requite::getLowercase(0x25) == 0x25); // '%' -> '%' (no change)
    CHECK(+requite::getLowercase(0x26) == 0x26); // '&' -> '&' (no change)
    CHECK(+requite::getLowercase(0x27) == 0x27); // ''' -> ''' (no change)
    CHECK(+requite::getLowercase(0x28) == 0x28); // '(' -> '(' (no change)
    CHECK(+requite::getLowercase(0x29) == 0x29); // ')' -> ')' (no change)
    CHECK(+requite::getLowercase(0x2A) == 0x2A); // '*' -> '*' (no change)
    CHECK(+requite::getLowercase(0x2B) == 0x2B); // '+' -> '+' (no change)
    CHECK(+requite::getLowercase(0x2C) == 0x2C); // ',' -> ',' (no change)
    CHECK(+requite::getLowercase(0x2D) == 0x2D); // '-' -> '-' (no change)
    CHECK(+requite::getLowercase(0x2E) == 0x2E); // '.' -> '.' (no change)
    CHECK(+requite::getLowercase(0x2F) == 0x2F); // '/' -> '/' (no change)
    CHECK(+requite::getLowercase(0x30) == 0x30); // '0' -> '0' (no change)
    CHECK(+requite::getLowercase(0x31) == 0x31); // '1' -> '1' (no change)
    CHECK(+requite::getLowercase(0x32) == 0x32); // '2' -> '2' (no change)
    CHECK(+requite::getLowercase(0x33) == 0x33); // '3' -> '3' (no change)
    CHECK(+requite::getLowercase(0x34) == 0x34); // '4' -> '4' (no change)
    CHECK(+requite::getLowercase(0x35) == 0x35); // '5' -> '5' (no change)
    CHECK(+requite::getLowercase(0x36) == 0x36); // '6' -> '6' (no change)
    CHECK(+requite::getLowercase(0x37) == 0x37); // '7' -> '7' (no change)
    CHECK(+requite::getLowercase(0x38) == 0x38); // '8' -> '8' (no change)
    CHECK(+requite::getLowercase(0x39) == 0x39); // '9' -> '9' (no change)
    CHECK(+requite::getLowercase(0x3A) == 0x3A); // ':' -> ':' (no change)
    CHECK(+requite::getLowercase(0x3B) == 0x3B); // ';' -> ';' (no change)
    CHECK(+requite::getLowercase(0x3C) == 0x3C); // '<' -> '<' (no change)
    CHECK(+requite::getLowercase(0x3D) == 0x3D); // '=' -> '=' (no change)
    CHECK(+requite::getLowercase(0x3E) == 0x3E); // '>' -> '>' (no change)
    CHECK(+requite::getLowercase(0x3F) == 0x3F); // '?' -> '?' (no change)
    CHECK(+requite::getLowercase(0x40) == 0x40); // '@' -> '@' (no change)
    CHECK(+requite::getLowercase(0x41) == 0x61); // 'A' -> 'a'
    CHECK(+requite::getLowercase(0x42) == 0x62); // 'B' -> 'b'
    CHECK(+requite::getLowercase(0x43) == 0x63); // 'C' -> 'c'
    CHECK(+requite::getLowercase(0x44) == 0x64); // 'D' -> 'd'
    CHECK(+requite::getLowercase(0x45) == 0x65); // 'E' -> 'e'
    CHECK(+requite::getLowercase(0x46) == 0x66); // 'F' -> 'f'
    CHECK(+requite::getLowercase(0x47) == 0x67); // 'G' -> 'g'
    CHECK(+requite::getLowercase(0x48) == 0x68); // 'H' -> 'h'
    CHECK(+requite::getLowercase(0x49) == 0x69); // 'I' -> 'i'
    CHECK(+requite::getLowercase(0x4A) == 0x6A); // 'J' -> 'j'
    CHECK(+requite::getLowercase(0x4B) == 0x6B); // 'K' -> 'k'
    CHECK(+requite::getLowercase(0x4C) == 0x6C); // 'L' -> 'l'
    CHECK(+requite::getLowercase(0x4D) == 0x6D); // 'M' -> 'm'
    CHECK(+requite::getLowercase(0x4E) == 0x6E); // 'N' -> 'n'
    CHECK(+requite::getLowercase(0x4F) == 0x6F); // 'O' -> 'o'
    CHECK(+requite::getLowercase(0x50) == 0x70); // 'P' -> 'p'
    CHECK(+requite::getLowercase(0x51) == 0x71); // 'Q' -> 'q'
    CHECK(+requite::getLowercase(0x52) == 0x72); // 'R' -> 'r'
    CHECK(+requite::getLowercase(0x53) == 0x73); // 'S' -> 's'
    CHECK(+requite::getLowercase(0x54) == 0x74); // 'T' -> 't'
    CHECK(+requite::getLowercase(0x55) == 0x75); // 'U' -> 'u'
    CHECK(+requite::getLowercase(0x56) == 0x76); // 'V' -> 'v'
    CHECK(+requite::getLowercase(0x57) == 0x77); // 'W' -> 'w'
    CHECK(+requite::getLowercase(0x58) == 0x78); // 'X' -> 'x'
    CHECK(+requite::getLowercase(0x59) == 0x79); // 'Y' -> 'y'
    CHECK(+requite::getLowercase(0x5A) == 0x7A); // 'Z' -> 'z'
    CHECK(+requite::getLowercase(0x5B) == 0x5B); // '[' -> '[' (no change)
    CHECK(+requite::getLowercase(0x5C) == 0x5C); // '\' -> '\' (no change)
    CHECK(+requite::getLowercase(0x5D) == 0x5D); // ']' -> ']' (no change)
    CHECK(+requite::getLowercase(0x5E) == 0x5E); // '^' -> '^' (no change)
    CHECK(+requite::getLowercase(0x5F) == 0x5F); // '_' -> '_' (no change)
    CHECK(+requite::getLowercase(0x60) == 0x60); // '`' -> '`' (no change)
    CHECK(+requite::getLowercase(0x61) == 0x61); // 'a' -> 'a' (no change)
    CHECK(+requite::getLowercase(0x62) == 0x62); // 'b' -> 'b' (no change)
    CHECK(+requite::getLowercase(0x63) == 0x63); // 'c' -> 'c' (no change)
    CHECK(+requite::getLowercase(0x64) == 0x64); // 'd' -> 'd' (no change)
    CHECK(+requite::getLowercase(0x65) == 0x65); // 'e' -> 'e' (no change)
    CHECK(+requite::getLowercase(0x66) == 0x66); // 'f' -> 'f' (no change)
    CHECK(+requite::getLowercase(0x67) == 0x67); // 'g' -> 'g' (no change)
    CHECK(+requite::getLowercase(0x68) == 0x68); // 'h' -> 'h' (no change)
    CHECK(+requite::getLowercase(0x69) == 0x69); // 'i' -> 'i' (no change)
    CHECK(+requite::getLowercase(0x6A) == 0x6A); // 'j' -> 'j' (no change)
    CHECK(+requite::getLowercase(0x6B) == 0x6B); // 'k' -> 'k' (no change)
    CHECK(+requite::getLowercase(0x6C) == 0x6C); // 'l' -> 'l' (no change)
    CHECK(+requite::getLowercase(0x6D) == 0x6D); // 'm' -> 'm' (no change)
    CHECK(+requite::getLowercase(0x6E) == 0x6E); // 'n' -> 'n' (no change)
    CHECK(+requite::getLowercase(0x6F) == 0x6F); // 'o' -> 'o' (no change)
    CHECK(+requite::getLowercase(0x70) == 0x70); // 'p' -> 'p' (no change)
    CHECK(+requite::getLowercase(0x71) == 0x71); // 'q' -> 'q' (no change)
    CHECK(+requite::getLowercase(0x72) == 0x72); // 'r' -> 'r' (no change)
    CHECK(+requite::getLowercase(0x73) == 0x73); // 's' -> 's' (no change)
    CHECK(+requite::getLowercase(0x74) == 0x74); // 't' -> 't' (no change)
    CHECK(+requite::getLowercase(0x75) == 0x75); // 'u' -> 'u' (no change)
    CHECK(+requite::getLowercase(0x76) == 0x76); // 'v' -> 'v' (no change)
    CHECK(+requite::getLowercase(0x77) == 0x77); // 'w' -> 'w' (no change)
    CHECK(+requite::getLowercase(0x78) == 0x78); // 'x' -> 'x' (no change)
    CHECK(+requite::getLowercase(0x79) == 0x79); // 'y' -> 'y' (no change)
    CHECK(+requite::getLowercase(0x7A) == 0x7A); // 'z' -> 'z' (no change)
    CHECK(+requite::getLowercase(0x7B) == 0x7B); // '{' -> '{' (no change)
    CHECK(+requite::getLowercase(0x7C) == 0x7C); // '|' -> '|' (no change)
    CHECK(+requite::getLowercase(0x7D) == 0x7D); // '}' -> '}' (no change)
    CHECK(+requite::getLowercase(0x7E) == 0x7E); // '~' -> '~' (no change)
  }

  SECTION("DEL Character (0x7F)") {
    CHECK(+requite::getLowercase(0x7F) == 0x7f); // DEL -> DEL (no change)
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    for (unsigned char codeunit = 0x80; codeunit >= 0x80; ++codeunit) {
      CHECK(+requite::getLowercase(codeunit) ==
            +std::bit_cast<char>(codeunit)); // codeunit -> codeunit (no change)
    }
  }
}

TEST_CASE("requite::getIsIdentifier(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsIdentifier(0x00)); // NUL
    CHECK_FALSE(requite::getIsIdentifier(0x01)); // SOH
    CHECK_FALSE(requite::getIsIdentifier(0x02)); // STX
    CHECK_FALSE(requite::getIsIdentifier(0x03)); // ETX
    CHECK_FALSE(requite::getIsIdentifier(0x04)); // EOT
    CHECK_FALSE(requite::getIsIdentifier(0x05)); // ENQ
    CHECK_FALSE(requite::getIsIdentifier(0x06)); // ACK
    CHECK_FALSE(requite::getIsIdentifier(0x07)); // BEL
    CHECK_FALSE(requite::getIsIdentifier(0x08)); // BS
    CHECK_FALSE(requite::getIsIdentifier(0x09)); // HT
    CHECK_FALSE(requite::getIsIdentifier(0x0A)); // LF
    CHECK_FALSE(requite::getIsIdentifier(0x0B)); // VT
    CHECK_FALSE(requite::getIsIdentifier(0x0C)); // FF
    CHECK_FALSE(requite::getIsIdentifier(0x0D)); // CR
    CHECK_FALSE(requite::getIsIdentifier(0x0E)); // SO
    CHECK_FALSE(requite::getIsIdentifier(0x0F)); // SI
    CHECK_FALSE(requite::getIsIdentifier(0x10)); // DLE
    CHECK_FALSE(requite::getIsIdentifier(0x11)); // DC1
    CHECK_FALSE(requite::getIsIdentifier(0x12)); // DC2
    CHECK_FALSE(requite::getIsIdentifier(0x13)); // DC3
    CHECK_FALSE(requite::getIsIdentifier(0x14)); // DC4
    CHECK_FALSE(requite::getIsIdentifier(0x15)); // NAK
    CHECK_FALSE(requite::getIsIdentifier(0x16)); // SYN
    CHECK_FALSE(requite::getIsIdentifier(0x17)); // ETB
    CHECK_FALSE(requite::getIsIdentifier(0x18)); // CAN
    CHECK_FALSE(requite::getIsIdentifier(0x19)); // EM
    CHECK_FALSE(requite::getIsIdentifier(0x1A)); // SUB
    CHECK_FALSE(requite::getIsIdentifier(0x1B)); // ESC
    CHECK_FALSE(requite::getIsIdentifier(0x1C)); // FS
    CHECK_FALSE(requite::getIsIdentifier(0x1D)); // GS
    CHECK_FALSE(requite::getIsIdentifier(0x1E)); // RS
    CHECK_FALSE(requite::getIsIdentifier(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_FALSE(requite::getIsIdentifier(0x20)); // SP
    CHECK_FALSE(requite::getIsIdentifier(0x21)); // '!'
    CHECK_FALSE(requite::getIsIdentifier(0x22)); // '"'
    CHECK_FALSE(requite::getIsIdentifier(0x23)); // '#'
    CHECK_FALSE(requite::getIsIdentifier(0x24)); // '$'
    CHECK_FALSE(requite::getIsIdentifier(0x25)); // '%'
    CHECK_FALSE(requite::getIsIdentifier(0x26)); // '&'
    CHECK_FALSE(requite::getIsIdentifier(0x27)); // '''
    CHECK_FALSE(requite::getIsIdentifier(0x28)); // '('
    CHECK_FALSE(requite::getIsIdentifier(0x29)); // ')'
    CHECK_FALSE(requite::getIsIdentifier(0x2A)); // '*'
    CHECK_FALSE(requite::getIsIdentifier(0x2B)); // '+'
    CHECK_FALSE(requite::getIsIdentifier(0x2C)); // ','
    CHECK_FALSE(requite::getIsIdentifier(0x2D)); // '-'
    CHECK_FALSE(requite::getIsIdentifier(0x2E)); // '.'
    CHECK_FALSE(requite::getIsIdentifier(0x2F)); // '/'
    CHECK(requite::getIsIdentifier(0x30));       // '0'
    CHECK(requite::getIsIdentifier(0x31));       // '1'
    CHECK(requite::getIsIdentifier(0x32));       // '2'
    CHECK(requite::getIsIdentifier(0x33));       // '3'
    CHECK(requite::getIsIdentifier(0x34));       // '4'
    CHECK(requite::getIsIdentifier(0x35));       // '5'
    CHECK(requite::getIsIdentifier(0x36));       // '6'
    CHECK(requite::getIsIdentifier(0x37));       // '7'
    CHECK(requite::getIsIdentifier(0x38));       // '8'
    CHECK(requite::getIsIdentifier(0x39));       // '9'
    CHECK_FALSE(requite::getIsIdentifier(0x3A)); // ':'
    CHECK_FALSE(requite::getIsIdentifier(0x3B)); // ';'
    CHECK_FALSE(requite::getIsIdentifier(0x3C)); // '<'
    CHECK_FALSE(requite::getIsIdentifier(0x3D)); // '='
    CHECK_FALSE(requite::getIsIdentifier(0x3E)); // '>'
    CHECK_FALSE(requite::getIsIdentifier(0x3F)); // '?'
    CHECK_FALSE(requite::getIsIdentifier(0x40)); // '@'
    CHECK(requite::getIsIdentifier(0x41));       // 'A'
    CHECK(requite::getIsIdentifier(0x42));       // 'B'
    CHECK(requite::getIsIdentifier(0x43));       // 'C'
    CHECK(requite::getIsIdentifier(0x44));       // 'D'
    CHECK(requite::getIsIdentifier(0x45));       // 'E'
    CHECK(requite::getIsIdentifier(0x46));       // 'F'
    CHECK(requite::getIsIdentifier(0x47));       // 'G'
    CHECK(requite::getIsIdentifier(0x48));       // 'H'
    CHECK(requite::getIsIdentifier(0x49));       // 'I'
    CHECK(requite::getIsIdentifier(0x4A));       // 'J'
    CHECK(requite::getIsIdentifier(0x4B));       // 'K'
    CHECK(requite::getIsIdentifier(0x4C));       // 'L'
    CHECK(requite::getIsIdentifier(0x4D));       // 'M'
    CHECK(requite::getIsIdentifier(0x4E));       // 'N'
    CHECK(requite::getIsIdentifier(0x4F));       // 'O'
    CHECK(requite::getIsIdentifier(0x50));       // 'P'
    CHECK(requite::getIsIdentifier(0x51));       // 'Q'
    CHECK(requite::getIsIdentifier(0x52));       // 'R'
    CHECK(requite::getIsIdentifier(0x53));       // 'S'
    CHECK(requite::getIsIdentifier(0x54));       // 'T'
    CHECK(requite::getIsIdentifier(0x55));       // 'U'
    CHECK(requite::getIsIdentifier(0x56));       // 'V'
    CHECK(requite::getIsIdentifier(0x57));       // 'W'
    CHECK(requite::getIsIdentifier(0x58));       // 'X'
    CHECK(requite::getIsIdentifier(0x59));       // 'Y'
    CHECK(requite::getIsIdentifier(0x5A));       // 'Z'
    CHECK_FALSE(requite::getIsIdentifier(0x5B)); // '['
    CHECK_FALSE(requite::getIsIdentifier(0x5C)); // '\'
    CHECK_FALSE(requite::getIsIdentifier(0x5D)); // ']'
    CHECK_FALSE(requite::getIsIdentifier(0x5E)); // '^'
    CHECK(requite::getIsIdentifier(0x5F));       // '_'
    CHECK_FALSE(requite::getIsIdentifier(0x60)); // '`'
    CHECK(requite::getIsIdentifier(0x61));       // 'a'
    CHECK(requite::getIsIdentifier(0x62));       // 'b'
    CHECK(requite::getIsIdentifier(0x63));       // 'c'
    CHECK(requite::getIsIdentifier(0x64));       // 'd'
    CHECK(requite::getIsIdentifier(0x65));       // 'e'
    CHECK(requite::getIsIdentifier(0x66));       // 'f'
    CHECK(requite::getIsIdentifier(0x67));       // 'g'
    CHECK(requite::getIsIdentifier(0x68));       // 'h'
    CHECK(requite::getIsIdentifier(0x69));       // 'i'
    CHECK(requite::getIsIdentifier(0x6A));       // 'j'
    CHECK(requite::getIsIdentifier(0x6B));       // 'k'
    CHECK(requite::getIsIdentifier(0x6C));       // 'l'
    CHECK(requite::getIsIdentifier(0x6D));       // 'm'
    CHECK(requite::getIsIdentifier(0x6E));       // 'n'
    CHECK(requite::getIsIdentifier(0x6F));       // 'o'
    CHECK(requite::getIsIdentifier(0x70));       // 'p'
    CHECK(requite::getIsIdentifier(0x71));       // 'q'
    CHECK(requite::getIsIdentifier(0x72));       // 'r'
    CHECK(requite::getIsIdentifier(0x73));       // 's'
    CHECK(requite::getIsIdentifier(0x74));       // 't'
    CHECK(requite::getIsIdentifier(0x75));       // 'u'
    CHECK(requite::getIsIdentifier(0x76));       // 'v'
    CHECK(requite::getIsIdentifier(0x77));       // 'w'
    CHECK(requite::getIsIdentifier(0x78));       // 'x'
    CHECK(requite::getIsIdentifier(0x79));       // 'y'
    CHECK(requite::getIsIdentifier(0x7A));       // 'z'
    CHECK_FALSE(requite::getIsIdentifier(0x7B)); // '{'
    CHECK_FALSE(requite::getIsIdentifier(0x7C)); // '|'
    CHECK_FALSE(requite::getIsIdentifier(0x7D)); // '}'
    CHECK_FALSE(requite::getIsIdentifier(0x7E)); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsIdentifier(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    // Extended continue characters (0x80 - 0x8F)
    for (unsigned char codeunit = 0x80; codeunit <= 0x8F; ++codeunit) {
      CHECK(requite::getIsIdentifier(codeunit));
    }

    // Extended 2 byte start characters (0xC0 - 0xDF)
    for (unsigned char codeunit = 0xC0; codeunit <= 0xDF; ++codeunit) {
      CHECK(requite::getIsIdentifier(codeunit));
    }

    // Extended 3 byte start characters (0xE0 - 0xEF)
    for (unsigned char codeunit = 0xE0; codeunit <= 0xEF; ++codeunit) {
      CHECK(requite::getIsIdentifier(codeunit));
    }

    // Extended four byte start characters (0xF0 - 0xF7)
    for (unsigned char codeunit = 0xF0; codeunit <= 0xF7; ++codeunit) {
      CHECK(requite::getIsIdentifier(codeunit));
    }

    // Invalid Extended Characters (0xF8 - 0xFF)
    for (unsigned char codeunit = 0xF8; codeunit >= 0xF8; ++codeunit) {
      CHECK_FALSE(requite::getIsIdentifier(codeunit));
    }
  }
}

TEST_CASE("requite::getIsIdentifierStart(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsIdentifierStart(0x00)); // NUL
    CHECK_FALSE(requite::getIsIdentifierStart(0x01)); // SOH
    CHECK_FALSE(requite::getIsIdentifierStart(0x02)); // STX
    CHECK_FALSE(requite::getIsIdentifierStart(0x03)); // ETX
    CHECK_FALSE(requite::getIsIdentifierStart(0x04)); // EOT
    CHECK_FALSE(requite::getIsIdentifierStart(0x05)); // ENQ
    CHECK_FALSE(requite::getIsIdentifierStart(0x06)); // ACK
    CHECK_FALSE(requite::getIsIdentifierStart(0x07)); // BEL
    CHECK_FALSE(requite::getIsIdentifierStart(0x08)); // BS
    CHECK_FALSE(requite::getIsIdentifierStart(0x09)); // HT
    CHECK_FALSE(requite::getIsIdentifierStart(0x0A)); // LF
    CHECK_FALSE(requite::getIsIdentifierStart(0x0B)); // VT
    CHECK_FALSE(requite::getIsIdentifierStart(0x0C)); // FF
    CHECK_FALSE(requite::getIsIdentifierStart(0x0D)); // CR
    CHECK_FALSE(requite::getIsIdentifierStart(0x0E)); // SO
    CHECK_FALSE(requite::getIsIdentifierStart(0x0F)); // SI
    CHECK_FALSE(requite::getIsIdentifierStart(0x10)); // DLE
    CHECK_FALSE(requite::getIsIdentifierStart(0x11)); // DC1
    CHECK_FALSE(requite::getIsIdentifierStart(0x12)); // DC2
    CHECK_FALSE(requite::getIsIdentifierStart(0x13)); // DC3
    CHECK_FALSE(requite::getIsIdentifierStart(0x14)); // DC4
    CHECK_FALSE(requite::getIsIdentifierStart(0x15)); // NAK
    CHECK_FALSE(requite::getIsIdentifierStart(0x16)); // SYN
    CHECK_FALSE(requite::getIsIdentifierStart(0x17)); // ETB
    CHECK_FALSE(requite::getIsIdentifierStart(0x18)); // CAN
    CHECK_FALSE(requite::getIsIdentifierStart(0x19)); // EM
    CHECK_FALSE(requite::getIsIdentifierStart(0x1A)); // SUB
    CHECK_FALSE(requite::getIsIdentifierStart(0x1B)); // ESC
    CHECK_FALSE(requite::getIsIdentifierStart(0x1C)); // FS
    CHECK_FALSE(requite::getIsIdentifierStart(0x1D)); // GS
    CHECK_FALSE(requite::getIsIdentifierStart(0x1E)); // RS
    CHECK_FALSE(requite::getIsIdentifierStart(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_FALSE(requite::getIsIdentifierStart(0x20)); // SP
    CHECK_FALSE(requite::getIsIdentifierStart(0x21)); // '!'
    CHECK_FALSE(requite::getIsIdentifierStart(0x22)); // '"'
    CHECK_FALSE(requite::getIsIdentifierStart(0x23)); // '#'
    CHECK_FALSE(requite::getIsIdentifierStart(0x24)); // '$'
    CHECK_FALSE(requite::getIsIdentifierStart(0x25)); // '%'
    CHECK_FALSE(requite::getIsIdentifierStart(0x26)); // '&'
    CHECK_FALSE(requite::getIsIdentifierStart(0x27)); // '''
    CHECK_FALSE(requite::getIsIdentifierStart(0x28)); // '('
    CHECK_FALSE(requite::getIsIdentifierStart(0x29)); // ')'
    CHECK_FALSE(requite::getIsIdentifierStart(0x2A)); // '*'
    CHECK_FALSE(requite::getIsIdentifierStart(0x2B)); // '+'
    CHECK_FALSE(requite::getIsIdentifierStart(0x2C)); // ','
    CHECK_FALSE(requite::getIsIdentifierStart(0x2D)); // '-'
    CHECK_FALSE(requite::getIsIdentifierStart(0x2E)); // '.'
    CHECK_FALSE(requite::getIsIdentifierStart(0x2F)); // '/'
    CHECK_FALSE(requite::getIsIdentifierStart(0x30)); // '0'
    CHECK_FALSE(requite::getIsIdentifierStart(0x31)); // '1'
    CHECK_FALSE(requite::getIsIdentifierStart(0x32)); // '2'
    CHECK_FALSE(requite::getIsIdentifierStart(0x33)); // '3'
    CHECK_FALSE(requite::getIsIdentifierStart(0x34)); // '4'
    CHECK_FALSE(requite::getIsIdentifierStart(0x35)); // '5'
    CHECK_FALSE(requite::getIsIdentifierStart(0x36)); // '6'
    CHECK_FALSE(requite::getIsIdentifierStart(0x37)); // '7'
    CHECK_FALSE(requite::getIsIdentifierStart(0x38)); // '8'
    CHECK_FALSE(requite::getIsIdentifierStart(0x39)); // '9'
    CHECK_FALSE(requite::getIsIdentifierStart(0x3A)); // ':'
    CHECK_FALSE(requite::getIsIdentifierStart(0x3B)); // ';'
    CHECK_FALSE(requite::getIsIdentifierStart(0x3C)); // '<'
    CHECK_FALSE(requite::getIsIdentifierStart(0x3D)); // '='
    CHECK_FALSE(requite::getIsIdentifierStart(0x3E)); // '>'
    CHECK_FALSE(requite::getIsIdentifierStart(0x3F)); // '?'
    CHECK_FALSE(requite::getIsIdentifierStart(0x40)); // '@'
    CHECK(requite::getIsIdentifierStart(0x41));       // 'A'
    CHECK(requite::getIsIdentifierStart(0x42));       // 'B'
    CHECK(requite::getIsIdentifierStart(0x43));       // 'C'
    CHECK(requite::getIsIdentifierStart(0x44));       // 'D'
    CHECK(requite::getIsIdentifierStart(0x45));       // 'E'
    CHECK(requite::getIsIdentifierStart(0x46));       // 'F'
    CHECK(requite::getIsIdentifierStart(0x47));       // 'G'
    CHECK(requite::getIsIdentifierStart(0x48));       // 'H'
    CHECK(requite::getIsIdentifierStart(0x49));       // 'I'
    CHECK(requite::getIsIdentifierStart(0x4A));       // 'J'
    CHECK(requite::getIsIdentifierStart(0x4B));       // 'K'
    CHECK(requite::getIsIdentifierStart(0x4C));       // 'L'
    CHECK(requite::getIsIdentifierStart(0x4D));       // 'M'
    CHECK(requite::getIsIdentifierStart(0x4E));       // 'N'
    CHECK(requite::getIsIdentifierStart(0x4F));       // 'O'
    CHECK(requite::getIsIdentifierStart(0x50));       // 'P'
    CHECK(requite::getIsIdentifierStart(0x51));       // 'Q'
    CHECK(requite::getIsIdentifierStart(0x52));       // 'R'
    CHECK(requite::getIsIdentifierStart(0x53));       // 'S'
    CHECK(requite::getIsIdentifierStart(0x54));       // 'T'
    CHECK(requite::getIsIdentifierStart(0x55));       // 'U'
    CHECK(requite::getIsIdentifierStart(0x56));       // 'V'
    CHECK(requite::getIsIdentifierStart(0x57));       // 'W'
    CHECK(requite::getIsIdentifierStart(0x58));       // 'X'
    CHECK(requite::getIsIdentifierStart(0x59));       // 'Y'
    CHECK(requite::getIsIdentifierStart(0x5A));       // 'Z'
    CHECK_FALSE(requite::getIsIdentifierStart(0x5B)); // '['
    CHECK_FALSE(requite::getIsIdentifierStart(0x5C)); // '\'
    CHECK_FALSE(requite::getIsIdentifierStart(0x5D)); // ']'
    CHECK_FALSE(requite::getIsIdentifierStart(0x5E)); // '^'
    CHECK(requite::getIsIdentifierStart(0x5F));       // '_'
    CHECK_FALSE(requite::getIsIdentifierStart(0x60)); // '`'
    CHECK(requite::getIsIdentifierStart(0x61));       // 'a'
    CHECK(requite::getIsIdentifierStart(0x62));       // 'b'
    CHECK(requite::getIsIdentifierStart(0x63));       // 'c'
    CHECK(requite::getIsIdentifierStart(0x64));       // 'd'
    CHECK(requite::getIsIdentifierStart(0x65));       // 'e'
    CHECK(requite::getIsIdentifierStart(0x66));       // 'f'
    CHECK(requite::getIsIdentifierStart(0x67));       // 'g'
    CHECK(requite::getIsIdentifierStart(0x68));       // 'h'
    CHECK(requite::getIsIdentifierStart(0x69));       // 'i'
    CHECK(requite::getIsIdentifierStart(0x6A));       // 'j'
    CHECK(requite::getIsIdentifierStart(0x6B));       // 'k'
    CHECK(requite::getIsIdentifierStart(0x6C));       // 'l'
    CHECK(requite::getIsIdentifierStart(0x6D));       // 'm'
    CHECK(requite::getIsIdentifierStart(0x6E));       // 'n'
    CHECK(requite::getIsIdentifierStart(0x6F));       // 'o'
    CHECK(requite::getIsIdentifierStart(0x70));       // 'p'
    CHECK(requite::getIsIdentifierStart(0x71));       // 'q'
    CHECK(requite::getIsIdentifierStart(0x72));       // 'r'
    CHECK(requite::getIsIdentifierStart(0x73));       // 's'
    CHECK(requite::getIsIdentifierStart(0x74));       // 't'
    CHECK(requite::getIsIdentifierStart(0x75));       // 'u'
    CHECK(requite::getIsIdentifierStart(0x76));       // 'v'
    CHECK(requite::getIsIdentifierStart(0x77));       // 'w'
    CHECK(requite::getIsIdentifierStart(0x78));       // 'x'
    CHECK(requite::getIsIdentifierStart(0x79));       // 'y'
    CHECK(requite::getIsIdentifierStart(0x7A));       // 'z'
    CHECK_FALSE(requite::getIsIdentifierStart(0x7B)); // '{'
    CHECK_FALSE(requite::getIsIdentifierStart(0x7C)); // '|'
    CHECK_FALSE(requite::getIsIdentifierStart(0x7D)); // '}'
    CHECK_FALSE(requite::getIsIdentifierStart(0x7E)); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsIdentifierStart(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    // Extended continue characters (0x80 - 0x8F)
    for (unsigned char codeunit = 0x80; codeunit <= 0x8F; ++codeunit) {
      CHECK(requite::getIsIdentifierStart(codeunit));
    }

    // Extended 2 byte start characters (0xC0 - 0xDF)
    for (unsigned char codeunit = 0xC0; codeunit <= 0xDF; ++codeunit) {
      CHECK(requite::getIsIdentifierStart(codeunit));
    }

    // Extended 3 byte start characters (0xE0 - 0xEF)
    for (unsigned char codeunit = 0xE0; codeunit <= 0xEF; ++codeunit) {
      CHECK(requite::getIsIdentifierStart(codeunit));
    }

    // Extended four byte start characters (0xF0 - 0xF7)
    for (unsigned char codeunit = 0xF0; codeunit <= 0xF7; ++codeunit) {
      CHECK(requite::getIsIdentifierStart(codeunit));
    }

    // Invalid Extended Characters (0xF8 - 0xFF)
    for (unsigned char codeunit = 0xF8; codeunit >= 0xF8; ++codeunit) {
      CHECK_FALSE(requite::getIsIdentifierStart(codeunit));
    }
  }
}

TEST_CASE("requite::getIsSymbol(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsSymbol(0x00)); // NUL
    CHECK_FALSE(requite::getIsSymbol(0x01)); // SOH
    CHECK_FALSE(requite::getIsSymbol(0x02)); // STX
    CHECK_FALSE(requite::getIsSymbol(0x03)); // ETX
    CHECK_FALSE(requite::getIsSymbol(0x04)); // EOT
    CHECK_FALSE(requite::getIsSymbol(0x05)); // ENQ
    CHECK_FALSE(requite::getIsSymbol(0x06)); // ACK
    CHECK_FALSE(requite::getIsSymbol(0x07)); // BEL
    CHECK_FALSE(requite::getIsSymbol(0x08)); // BS
    CHECK_FALSE(requite::getIsSymbol(0x09)); // HT
    CHECK_FALSE(requite::getIsSymbol(0x0A)); // LF
    CHECK_FALSE(requite::getIsSymbol(0x0B)); // VT
    CHECK_FALSE(requite::getIsSymbol(0x0C)); // FF
    CHECK_FALSE(requite::getIsSymbol(0x0D)); // CR
    CHECK_FALSE(requite::getIsSymbol(0x0E)); // SO
    CHECK_FALSE(requite::getIsSymbol(0x0F)); // SI
    CHECK_FALSE(requite::getIsSymbol(0x10)); // DLE
    CHECK_FALSE(requite::getIsSymbol(0x11)); // DC1
    CHECK_FALSE(requite::getIsSymbol(0x12)); // DC2
    CHECK_FALSE(requite::getIsSymbol(0x13)); // DC3
    CHECK_FALSE(requite::getIsSymbol(0x14)); // DC4
    CHECK_FALSE(requite::getIsSymbol(0x15)); // NAK
    CHECK_FALSE(requite::getIsSymbol(0x16)); // SYN
    CHECK_FALSE(requite::getIsSymbol(0x17)); // ETB
    CHECK_FALSE(requite::getIsSymbol(0x18)); // CAN
    CHECK_FALSE(requite::getIsSymbol(0x19)); // EM
    CHECK_FALSE(requite::getIsSymbol(0x1A)); // SUB
    CHECK_FALSE(requite::getIsSymbol(0x1B)); // ESC
    CHECK_FALSE(requite::getIsSymbol(0x1C)); // FS
    CHECK_FALSE(requite::getIsSymbol(0x1D)); // GS
    CHECK_FALSE(requite::getIsSymbol(0x1E)); // RS
    CHECK_FALSE(requite::getIsSymbol(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_FALSE(requite::getIsSymbol(0x20)); // SP
    CHECK(requite::getIsSymbol(0x21));       // '!'
    CHECK(requite::getIsSymbol(0x22));       // '"'
    CHECK(requite::getIsSymbol(0x23));       // '#'
    CHECK(requite::getIsSymbol(0x24));       // '$'
    CHECK(requite::getIsSymbol(0x25));       // '%'
    CHECK(requite::getIsSymbol(0x26));       // '&'
    CHECK(requite::getIsSymbol(0x27));       // '''
    CHECK(requite::getIsSymbol(0x28));       // '('
    CHECK(requite::getIsSymbol(0x29));       // ')'
    CHECK(requite::getIsSymbol(0x2A));       // '*'
    CHECK(requite::getIsSymbol(0x2B));       // '+'
    CHECK(requite::getIsSymbol(0x2C));       // ','
    CHECK(requite::getIsSymbol(0x2D));       // '-'
    CHECK(requite::getIsSymbol(0x2E));       // '.'
    CHECK(requite::getIsSymbol(0x2F));       // '/'
    CHECK_FALSE(requite::getIsSymbol(0x30)); // '0'
    CHECK_FALSE(requite::getIsSymbol(0x31)); // '1'
    CHECK_FALSE(requite::getIsSymbol(0x32)); // '2'
    CHECK_FALSE(requite::getIsSymbol(0x33)); // '3'
    CHECK_FALSE(requite::getIsSymbol(0x34)); // '4'
    CHECK_FALSE(requite::getIsSymbol(0x35)); // '5'
    CHECK_FALSE(requite::getIsSymbol(0x36)); // '6'
    CHECK_FALSE(requite::getIsSymbol(0x37)); // '7'
    CHECK_FALSE(requite::getIsSymbol(0x38)); // '8'
    CHECK_FALSE(requite::getIsSymbol(0x39)); // '9'
    CHECK(requite::getIsSymbol(0x3A));       // ':'
    CHECK(requite::getIsSymbol(0x3B));       // ';'
    CHECK(requite::getIsSymbol(0x3C));       // '<'
    CHECK(requite::getIsSymbol(0x3D));       // '='
    CHECK(requite::getIsSymbol(0x3E));       // '>'
    CHECK(requite::getIsSymbol(0x3F));       // '?'
    CHECK(requite::getIsSymbol(0x40));       // '@'
    CHECK_FALSE(requite::getIsSymbol(0x41)); // 'A'
    CHECK_FALSE(requite::getIsSymbol(0x42)); // 'B'
    CHECK_FALSE(requite::getIsSymbol(0x43)); // 'C'
    CHECK_FALSE(requite::getIsSymbol(0x44)); // 'D'
    CHECK_FALSE(requite::getIsSymbol(0x45)); // 'E'
    CHECK_FALSE(requite::getIsSymbol(0x46)); // 'F'
    CHECK_FALSE(requite::getIsSymbol(0x47)); // 'G'
    CHECK_FALSE(requite::getIsSymbol(0x48)); // 'H'
    CHECK_FALSE(requite::getIsSymbol(0x49)); // 'I'
    CHECK_FALSE(requite::getIsSymbol(0x4A)); // 'J'
    CHECK_FALSE(requite::getIsSymbol(0x4B)); // 'K'
    CHECK_FALSE(requite::getIsSymbol(0x4C)); // 'L'
    CHECK_FALSE(requite::getIsSymbol(0x4D)); // 'M'
    CHECK_FALSE(requite::getIsSymbol(0x4E)); // 'N'
    CHECK_FALSE(requite::getIsSymbol(0x4F)); // 'O'
    CHECK_FALSE(requite::getIsSymbol(0x50)); // 'P'
    CHECK_FALSE(requite::getIsSymbol(0x51)); // 'Q'
    CHECK_FALSE(requite::getIsSymbol(0x52)); // 'R'
    CHECK_FALSE(requite::getIsSymbol(0x53)); // 'S'
    CHECK_FALSE(requite::getIsSymbol(0x54)); // 'T'
    CHECK_FALSE(requite::getIsSymbol(0x55)); // 'U'
    CHECK_FALSE(requite::getIsSymbol(0x56)); // 'V'
    CHECK_FALSE(requite::getIsSymbol(0x57)); // 'W'
    CHECK_FALSE(requite::getIsSymbol(0x58)); // 'X'
    CHECK_FALSE(requite::getIsSymbol(0x59)); // 'Y'
    CHECK_FALSE(requite::getIsSymbol(0x5A)); // 'Z'
    CHECK(requite::getIsSymbol(0x5B));       // '['
    CHECK(requite::getIsSymbol(0x5C));       // '\'
    CHECK(requite::getIsSymbol(0x5D));       // ']'
    CHECK(requite::getIsSymbol(0x5E));       // '^'
    CHECK_FALSE(requite::getIsSymbol(0x5F)); // '_'
    CHECK(requite::getIsSymbol(0x60));       // '`'
    CHECK_FALSE(requite::getIsSymbol(0x61)); // 'a'
    CHECK_FALSE(requite::getIsSymbol(0x62)); // 'b'
    CHECK_FALSE(requite::getIsSymbol(0x63)); // 'c'
    CHECK_FALSE(requite::getIsSymbol(0x64)); // 'd'
    CHECK_FALSE(requite::getIsSymbol(0x65)); // 'e'
    CHECK_FALSE(requite::getIsSymbol(0x66)); // 'f'
    CHECK_FALSE(requite::getIsSymbol(0x67)); // 'g'
    CHECK_FALSE(requite::getIsSymbol(0x68)); // 'h'
    CHECK_FALSE(requite::getIsSymbol(0x69)); // 'i'
    CHECK_FALSE(requite::getIsSymbol(0x6A)); // 'j'
    CHECK_FALSE(requite::getIsSymbol(0x6B)); // 'k'
    CHECK_FALSE(requite::getIsSymbol(0x6C)); // 'l'
    CHECK_FALSE(requite::getIsSymbol(0x6D)); // 'm'
    CHECK_FALSE(requite::getIsSymbol(0x6E)); // 'n'
    CHECK_FALSE(requite::getIsSymbol(0x6F)); // 'o'
    CHECK_FALSE(requite::getIsSymbol(0x70)); // 'p'
    CHECK_FALSE(requite::getIsSymbol(0x71)); // 'q'
    CHECK_FALSE(requite::getIsSymbol(0x72)); // 'r'
    CHECK_FALSE(requite::getIsSymbol(0x73)); // 's'
    CHECK_FALSE(requite::getIsSymbol(0x74)); // 't'
    CHECK_FALSE(requite::getIsSymbol(0x75)); // 'u'
    CHECK_FALSE(requite::getIsSymbol(0x76)); // 'v'
    CHECK_FALSE(requite::getIsSymbol(0x77)); // 'w'
    CHECK_FALSE(requite::getIsSymbol(0x78)); // 'x'
    CHECK_FALSE(requite::getIsSymbol(0x79)); // 'y'
    CHECK_FALSE(requite::getIsSymbol(0x7A)); // 'z'
    CHECK(requite::getIsSymbol(0x7B));       // '{'
    CHECK(requite::getIsSymbol(0x7C));       // '|'
    CHECK(requite::getIsSymbol(0x7D));       // '}'
    CHECK(requite::getIsSymbol(0x7E));       // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsSymbol(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    for (unsigned char codeunit = 0x80; codeunit >= 0x80; ++codeunit) {
      CHECK_FALSE(requite::getIsSymbol(codeunit));
    }
  }
}

TEST_CASE("requite::getIsExtended(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsExtended(0x00)); // NUL
    CHECK_FALSE(requite::getIsExtended(0x01)); // SOH
    CHECK_FALSE(requite::getIsExtended(0x02)); // STX
    CHECK_FALSE(requite::getIsExtended(0x03)); // ETX
    CHECK_FALSE(requite::getIsExtended(0x04)); // EOT
    CHECK_FALSE(requite::getIsExtended(0x05)); // ENQ
    CHECK_FALSE(requite::getIsExtended(0x06)); // ACK
    CHECK_FALSE(requite::getIsExtended(0x07)); // BEL
    CHECK_FALSE(requite::getIsExtended(0x08)); // BS
    CHECK_FALSE(requite::getIsExtended(0x09)); // HT
    CHECK_FALSE(requite::getIsExtended(0x0A)); // LF
    CHECK_FALSE(requite::getIsExtended(0x0B)); // VT
    CHECK_FALSE(requite::getIsExtended(0x0C)); // FF
    CHECK_FALSE(requite::getIsExtended(0x0D)); // CR
    CHECK_FALSE(requite::getIsExtended(0x0E)); // SO
    CHECK_FALSE(requite::getIsExtended(0x0F)); // SI
    CHECK_FALSE(requite::getIsExtended(0x10)); // DLE
    CHECK_FALSE(requite::getIsExtended(0x11)); // DC1
    CHECK_FALSE(requite::getIsExtended(0x12)); // DC2
    CHECK_FALSE(requite::getIsExtended(0x13)); // DC3
    CHECK_FALSE(requite::getIsExtended(0x14)); // DC4
    CHECK_FALSE(requite::getIsExtended(0x15)); // NAK
    CHECK_FALSE(requite::getIsExtended(0x16)); // SYN
    CHECK_FALSE(requite::getIsExtended(0x17)); // ETB
    CHECK_FALSE(requite::getIsExtended(0x18)); // CAN
    CHECK_FALSE(requite::getIsExtended(0x19)); // EM
    CHECK_FALSE(requite::getIsExtended(0x1A)); // SUB
    CHECK_FALSE(requite::getIsExtended(0x1B)); // ESC
    CHECK_FALSE(requite::getIsExtended(0x1C)); // FS
    CHECK_FALSE(requite::getIsExtended(0x1D)); // GS
    CHECK_FALSE(requite::getIsExtended(0x1E)); // RS
    CHECK_FALSE(requite::getIsExtended(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_FALSE(requite::getIsExtended(0x20)); // SP
    CHECK_FALSE(requite::getIsExtended(0x21)); // '!'
    CHECK_FALSE(requite::getIsExtended(0x22)); // '"'
    CHECK_FALSE(requite::getIsExtended(0x23)); // '#'
    CHECK_FALSE(requite::getIsExtended(0x24)); // '$'
    CHECK_FALSE(requite::getIsExtended(0x25)); // '%'
    CHECK_FALSE(requite::getIsExtended(0x26)); // '&'
    CHECK_FALSE(requite::getIsExtended(0x27)); // '''
    CHECK_FALSE(requite::getIsExtended(0x28)); // '('
    CHECK_FALSE(requite::getIsExtended(0x29)); // ')'
    CHECK_FALSE(requite::getIsExtended(0x2A)); // '*'
    CHECK_FALSE(requite::getIsExtended(0x2B)); // '+'
    CHECK_FALSE(requite::getIsExtended(0x2C)); // ','
    CHECK_FALSE(requite::getIsExtended(0x2D)); // '-'
    CHECK_FALSE(requite::getIsExtended(0x2E)); // '.'
    CHECK_FALSE(requite::getIsExtended(0x2F)); // '/'
    CHECK_FALSE(requite::getIsExtended(0x30)); // '0'
    CHECK_FALSE(requite::getIsExtended(0x31)); // '1'
    CHECK_FALSE(requite::getIsExtended(0x32)); // '2'
    CHECK_FALSE(requite::getIsExtended(0x33)); // '3'
    CHECK_FALSE(requite::getIsExtended(0x34)); // '4'
    CHECK_FALSE(requite::getIsExtended(0x35)); // '5'
    CHECK_FALSE(requite::getIsExtended(0x36)); // '6'
    CHECK_FALSE(requite::getIsExtended(0x37)); // '7'
    CHECK_FALSE(requite::getIsExtended(0x38)); // '8'
    CHECK_FALSE(requite::getIsExtended(0x39)); // '9'
    CHECK_FALSE(requite::getIsExtended(0x3A)); // ':'
    CHECK_FALSE(requite::getIsExtended(0x3B)); // ';'
    CHECK_FALSE(requite::getIsExtended(0x3C)); // '<'
    CHECK_FALSE(requite::getIsExtended(0x3D)); // '='
    CHECK_FALSE(requite::getIsExtended(0x3E)); // '>'
    CHECK_FALSE(requite::getIsExtended(0x3F)); // '?'
    CHECK_FALSE(requite::getIsExtended(0x40)); // '@'
    CHECK_FALSE(requite::getIsExtended(0x41)); // 'A'
    CHECK_FALSE(requite::getIsExtended(0x42)); // 'B'
    CHECK_FALSE(requite::getIsExtended(0x43)); // 'C'
    CHECK_FALSE(requite::getIsExtended(0x44)); // 'D'
    CHECK_FALSE(requite::getIsExtended(0x45)); // 'E'
    CHECK_FALSE(requite::getIsExtended(0x46)); // 'F'
    CHECK_FALSE(requite::getIsExtended(0x47)); // 'G'
    CHECK_FALSE(requite::getIsExtended(0x48)); // 'H'
    CHECK_FALSE(requite::getIsExtended(0x49)); // 'I'
    CHECK_FALSE(requite::getIsExtended(0x4A)); // 'J'
    CHECK_FALSE(requite::getIsExtended(0x4B)); // 'K'
    CHECK_FALSE(requite::getIsExtended(0x4C)); // 'L'
    CHECK_FALSE(requite::getIsExtended(0x4D)); // 'M'
    CHECK_FALSE(requite::getIsExtended(0x4E)); // 'N'
    CHECK_FALSE(requite::getIsExtended(0x4F)); // 'O'
    CHECK_FALSE(requite::getIsExtended(0x50)); // 'P'
    CHECK_FALSE(requite::getIsExtended(0x51)); // 'Q'
    CHECK_FALSE(requite::getIsExtended(0x52)); // 'R'
    CHECK_FALSE(requite::getIsExtended(0x53)); // 'S'
    CHECK_FALSE(requite::getIsExtended(0x54)); // 'T'
    CHECK_FALSE(requite::getIsExtended(0x55)); // 'U'
    CHECK_FALSE(requite::getIsExtended(0x56)); // 'V'
    CHECK_FALSE(requite::getIsExtended(0x57)); // 'W'
    CHECK_FALSE(requite::getIsExtended(0x58)); // 'X'
    CHECK_FALSE(requite::getIsExtended(0x59)); // 'Y'
    CHECK_FALSE(requite::getIsExtended(0x5A)); // 'Z'
    CHECK_FALSE(requite::getIsExtended(0x5B)); // '['
    CHECK_FALSE(requite::getIsExtended(0x5C)); // '\'
    CHECK_FALSE(requite::getIsExtended(0x5D)); // ']'
    CHECK_FALSE(requite::getIsExtended(0x5E)); // '^'
    CHECK_FALSE(requite::getIsExtended(0x5F)); // '_'
    CHECK_FALSE(requite::getIsExtended(0x60)); // '`'
    CHECK_FALSE(requite::getIsExtended(0x61)); // 'a'
    CHECK_FALSE(requite::getIsExtended(0x62)); // 'b'
    CHECK_FALSE(requite::getIsExtended(0x63)); // 'c'
    CHECK_FALSE(requite::getIsExtended(0x64)); // 'd'
    CHECK_FALSE(requite::getIsExtended(0x65)); // 'e'
    CHECK_FALSE(requite::getIsExtended(0x66)); // 'f'
    CHECK_FALSE(requite::getIsExtended(0x67)); // 'g'
    CHECK_FALSE(requite::getIsExtended(0x68)); // 'h'
    CHECK_FALSE(requite::getIsExtended(0x69)); // 'i'
    CHECK_FALSE(requite::getIsExtended(0x6A)); // 'j'
    CHECK_FALSE(requite::getIsExtended(0x6B)); // 'k'
    CHECK_FALSE(requite::getIsExtended(0x6C)); // 'l'
    CHECK_FALSE(requite::getIsExtended(0x6D)); // 'm'
    CHECK_FALSE(requite::getIsExtended(0x6E)); // 'n'
    CHECK_FALSE(requite::getIsExtended(0x6F)); // 'o'
    CHECK_FALSE(requite::getIsExtended(0x70)); // 'p'
    CHECK_FALSE(requite::getIsExtended(0x71)); // 'q'
    CHECK_FALSE(requite::getIsExtended(0x72)); // 'r'
    CHECK_FALSE(requite::getIsExtended(0x73)); // 's'
    CHECK_FALSE(requite::getIsExtended(0x74)); // 't'
    CHECK_FALSE(requite::getIsExtended(0x75)); // 'u'
    CHECK_FALSE(requite::getIsExtended(0x76)); // 'v'
    CHECK_FALSE(requite::getIsExtended(0x77)); // 'w'
    CHECK_FALSE(requite::getIsExtended(0x78)); // 'x'
    CHECK_FALSE(requite::getIsExtended(0x79)); // 'y'
    CHECK_FALSE(requite::getIsExtended(0x7A)); // 'z'
    CHECK_FALSE(requite::getIsExtended(0x7B)); // '{'
    CHECK_FALSE(requite::getIsExtended(0x7C)); // '|'
    CHECK_FALSE(requite::getIsExtended(0x7D)); // '}'
    CHECK_FALSE(requite::getIsExtended(0x7E)); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsExtended(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    // Extended continue characters (0x80 - 0x8F)
    for (unsigned char codeunit = 0x80; codeunit <= 0x8F; ++codeunit) {
      CHECK(requite::getIsExtended(codeunit));
    }

    // Extended 2 byte start characters (0xC0 - 0xDF)
    for (unsigned char codeunit = 0xC0; codeunit <= 0xDF; ++codeunit) {
      CHECK(requite::getIsExtended(codeunit));
    }

    // Extended 3 byte start characters (0xE0 - 0xEF)
    for (unsigned char codeunit = 0xE0; codeunit <= 0xEF; ++codeunit) {
      CHECK(requite::getIsExtended(codeunit));
    }

    // Extended four byte start characters (0xF0 - 0xF7)
    for (unsigned char codeunit = 0xF0; codeunit <= 0xF7; ++codeunit) {
      CHECK(requite::getIsExtended(codeunit));
    }

    // Invalid Extended Characters (0xF8 - 0xFF)
    for (unsigned char codeunit = 0xF8; codeunit >= 0xF8; ++codeunit) {
      CHECK_FALSE(requite::getIsExtended(codeunit));
    }
  }
}

TEST_CASE("requite::getExtendedStartCount(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x00),
                          requite::AssertException); // NUL
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x01),
                          requite::AssertException); // SOH
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x02),
                          requite::AssertException); // STX
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x03),
                          requite::AssertException); // ETX
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x04),
                          requite::AssertException); // EOT
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x05),
                          requite::AssertException); // ENQ
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x06),
                          requite::AssertException); // ACK
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x07),
                          requite::AssertException); // BEL
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x08),
                          requite::AssertException); // BS
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x09),
                          requite::AssertException); // HT
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x0A),
                          requite::AssertException); // LF
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x0B),
                          requite::AssertException); // VT
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x0C),
                          requite::AssertException); // FF
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x0D),
                          requite::AssertException); // CR
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x0E),
                          requite::AssertException); // SO
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x0F),
                          requite::AssertException); // SI
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x10),
                          requite::AssertException); // DLE
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x11),
                          requite::AssertException); // DC1
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x12),
                          requite::AssertException); // DC2
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x13),
                          requite::AssertException); // DC3
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x14),
                          requite::AssertException); // DC4
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x15),
                          requite::AssertException); // NAK
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x16),
                          requite::AssertException); // SYN
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x17),
                          requite::AssertException); // ETB
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x18),
                          requite::AssertException); // CAN
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x19),
                          requite::AssertException); // EM
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x1A),
                          requite::AssertException); // SUB
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x1B),
                          requite::AssertException); // ESC
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x1C),
                          requite::AssertException); // FS
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x1D),
                          requite::AssertException); // GS
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x1E),
                          requite::AssertException); // RS
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x1F),
                          requite::AssertException); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x20),
                          requite::AssertException); // SP
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x21),
                          requite::AssertException); // '!'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x22),
                          requite::AssertException); // '"'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x23),
                          requite::AssertException); // '#'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x24),
                          requite::AssertException); // '$'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x25),
                          requite::AssertException); // '%'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x26),
                          requite::AssertException); // '&'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x27),
                          requite::AssertException); // '''
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x28),
                          requite::AssertException); // '('
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x29),
                          requite::AssertException); // ')'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x2A),
                          requite::AssertException); // '*'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x2B),
                          requite::AssertException); // '+'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x2C),
                          requite::AssertException); // ','
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x2D),
                          requite::AssertException); // '-'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x2E),
                          requite::AssertException); // '.'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x2F),
                          requite::AssertException); // '/'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x30),
                          requite::AssertException); // '0'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x31),
                          requite::AssertException); // '1'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x32),
                          requite::AssertException); // '2'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x33),
                          requite::AssertException); // '3'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x34),
                          requite::AssertException); // '4'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x35),
                          requite::AssertException); // '5'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x36),
                          requite::AssertException); // '6'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x37),
                          requite::AssertException); // '7'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x38),
                          requite::AssertException); // '8'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x39),
                          requite::AssertException); // '9'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x3A),
                          requite::AssertException); // ':'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x3B),
                          requite::AssertException); // ';'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x3C),
                          requite::AssertException); // '<'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x3D),
                          requite::AssertException); // '='
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x3E),
                          requite::AssertException); // '>'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x3F),
                          requite::AssertException); // '?'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x40),
                          requite::AssertException); // '@'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x41),
                          requite::AssertException); // 'A'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x42),
                          requite::AssertException); // 'B'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x43),
                          requite::AssertException); // 'C'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x44),
                          requite::AssertException); // 'D'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x45),
                          requite::AssertException); // 'E'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x46),
                          requite::AssertException); // 'F'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x47),
                          requite::AssertException); // 'G'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x48),
                          requite::AssertException); // 'H'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x49),
                          requite::AssertException); // 'I'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x4A),
                          requite::AssertException); // 'J'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x4B),
                          requite::AssertException); // 'K'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x4C),
                          requite::AssertException); // 'L'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x4D),
                          requite::AssertException); // 'M'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x4E),
                          requite::AssertException); // 'N'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x4F),
                          requite::AssertException); // 'O'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x50),
                          requite::AssertException); // 'P'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x51),
                          requite::AssertException); // 'Q'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x52),
                          requite::AssertException); // 'R'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x53),
                          requite::AssertException); // 'S'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x54),
                          requite::AssertException); // 'T'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x55),
                          requite::AssertException); // 'U'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x56),
                          requite::AssertException); // 'V'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x57),
                          requite::AssertException); // 'W'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x58),
                          requite::AssertException); // 'X'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x59),
                          requite::AssertException); // 'Y'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x5A),
                          requite::AssertException); // 'Z'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x5B),
                          requite::AssertException); // '['
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x5C),
                          requite::AssertException); // '\'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x5D),
                          requite::AssertException); // ']'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x5E),
                          requite::AssertException); // '^'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x5F),
                          requite::AssertException); // '_'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x60),
                          requite::AssertException); // '`'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x61),
                          requite::AssertException); // 'a'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x62),
                          requite::AssertException); // 'b'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x63),
                          requite::AssertException); // 'c'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x64),
                          requite::AssertException); // 'd'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x65),
                          requite::AssertException); // 'e'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x66),
                          requite::AssertException); // 'f'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x67),
                          requite::AssertException); // 'g'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x68),
                          requite::AssertException); // 'h'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x69),
                          requite::AssertException); // 'i'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x6A),
                          requite::AssertException); // 'j'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x6B),
                          requite::AssertException); // 'k'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x6C),
                          requite::AssertException); // 'l'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x6D),
                          requite::AssertException); // 'm'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x6E),
                          requite::AssertException); // 'n'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x6F),
                          requite::AssertException); // 'o'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x70),
                          requite::AssertException); // 'p'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x71),
                          requite::AssertException); // 'q'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x72),
                          requite::AssertException); // 'r'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x73),
                          requite::AssertException); // 's'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x74),
                          requite::AssertException); // 't'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x75),
                          requite::AssertException); // 'u'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x76),
                          requite::AssertException); // 'v'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x77),
                          requite::AssertException); // 'w'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x78),
                          requite::AssertException); // 'x'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x79),
                          requite::AssertException); // 'y'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x7A),
                          requite::AssertException); // 'z'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x7B),
                          requite::AssertException); // '{'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x7C),
                          requite::AssertException); // '|'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x7D),
                          requite::AssertException); // '}'
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x7E),
                          requite::AssertException); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_THROWS_AS_DEBUG(std::ignore = requite::getExtendedStartCount(0x7F),
                          requite::AssertException); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    // Extended continue characters (0x80 - 0x8F)
    for (unsigned char codeunit = 0x80; codeunit <= 0x8F; ++codeunit) {
      CHECK(+requite::getExtendedStartCount(codeunit) == 0);
    }

    // Extended 2 byte start characters (0xC0 - 0xDF)
    for (unsigned char codeunit = 0xC0; codeunit <= 0xDF; ++codeunit) {
      CHECK(+requite::getExtendedStartCount(codeunit) == 1);
    }

    // Extended 3 byte start characters (0xE0 - 0xEF)
    for (unsigned char codeunit = 0xE0; codeunit <= 0xEF; ++codeunit) {
      CHECK(+requite::getExtendedStartCount(codeunit) == 2);
    }

    // Extended four byte start characters (0xF0 - 0xF7)
    for (unsigned char codeunit = 0xF0; codeunit <= 0xF7; ++codeunit) {
      CHECK(+requite::getExtendedStartCount(codeunit) == 3);
    }

    // Invalid Extended Characters (0xF8 - 0xFF)
    for (unsigned char codeunit = 0xF8; codeunit >= 0xF8; ++codeunit) {
      CHECK_THROWS_AS_DEBUG(std::ignore =
                                requite::getExtendedStartCount(codeunit),
                            requite::AssertException);
    }
  }
}

TEST_CASE("requite::getIsValid(char)") {
  SECTION("Control Characters (0x00 - 0x1F)") {
    CHECK_FALSE(requite::getIsValid(0x00)); // NUL
    CHECK_FALSE(requite::getIsValid(0x01)); // SOH
    CHECK_FALSE(requite::getIsValid(0x02)); // STX
    CHECK_FALSE(requite::getIsValid(0x03)); // ETX
    CHECK_FALSE(requite::getIsValid(0x04)); // EOT
    CHECK_FALSE(requite::getIsValid(0x05)); // ENQ
    CHECK_FALSE(requite::getIsValid(0x06)); // ACK
    CHECK_FALSE(requite::getIsValid(0x07)); // BEL
    CHECK_FALSE(requite::getIsValid(0x08)); // BS
    CHECK(requite::getIsValid(0x09));       // HT
    CHECK(requite::getIsValid(0x0A));       // LF
    CHECK(requite::getIsValid(0x0B));       // VT
    CHECK_FALSE(requite::getIsValid(0x0C)); // FF
    CHECK_FALSE(requite::getIsValid(0x0D)); // CR
    CHECK_FALSE(requite::getIsValid(0x0E)); // SO
    CHECK_FALSE(requite::getIsValid(0x0F)); // SI
    CHECK_FALSE(requite::getIsValid(0x10)); // DLE
    CHECK_FALSE(requite::getIsValid(0x11)); // DC1
    CHECK_FALSE(requite::getIsValid(0x12)); // DC2
    CHECK_FALSE(requite::getIsValid(0x13)); // DC3
    CHECK_FALSE(requite::getIsValid(0x14)); // DC4
    CHECK_FALSE(requite::getIsValid(0x15)); // NAK
    CHECK_FALSE(requite::getIsValid(0x16)); // SYN
    CHECK_FALSE(requite::getIsValid(0x17)); // ETB
    CHECK_FALSE(requite::getIsValid(0x18)); // CAN
    CHECK_FALSE(requite::getIsValid(0x19)); // EM
    CHECK_FALSE(requite::getIsValid(0x1A)); // SUB
    CHECK_FALSE(requite::getIsValid(0x1B)); // ESC
    CHECK_FALSE(requite::getIsValid(0x1C)); // FS
    CHECK_FALSE(requite::getIsValid(0x1D)); // GS
    CHECK_FALSE(requite::getIsValid(0x1E)); // RS
    CHECK_FALSE(requite::getIsValid(0x1F)); // US
  }

  SECTION("Printable Characters (0x20 - 0x7E)") {
    CHECK(requite::getIsValid(0x20)); // SP
    CHECK(requite::getIsValid(0x21)); // '!'
    CHECK(requite::getIsValid(0x22)); // '"'
    CHECK(requite::getIsValid(0x23)); // '#'
    CHECK(requite::getIsValid(0x24)); // '$'
    CHECK(requite::getIsValid(0x25)); // '%'
    CHECK(requite::getIsValid(0x26)); // '&'
    CHECK(requite::getIsValid(0x27)); // '''
    CHECK(requite::getIsValid(0x28)); // '('
    CHECK(requite::getIsValid(0x29)); // ')'
    CHECK(requite::getIsValid(0x2A)); // '*'
    CHECK(requite::getIsValid(0x2B)); // '+'
    CHECK(requite::getIsValid(0x2C)); // ','
    CHECK(requite::getIsValid(0x2D)); // '-'
    CHECK(requite::getIsValid(0x2E)); // '.'
    CHECK(requite::getIsValid(0x2F)); // '/'
    CHECK(requite::getIsValid(0x30)); // '0'
    CHECK(requite::getIsValid(0x31)); // '1'
    CHECK(requite::getIsValid(0x32)); // '2'
    CHECK(requite::getIsValid(0x33)); // '3'
    CHECK(requite::getIsValid(0x34)); // '4'
    CHECK(requite::getIsValid(0x35)); // '5'
    CHECK(requite::getIsValid(0x36)); // '6'
    CHECK(requite::getIsValid(0x37)); // '7'
    CHECK(requite::getIsValid(0x38)); // '8'
    CHECK(requite::getIsValid(0x39)); // '9'
    CHECK(requite::getIsValid(0x3A)); // ':'
    CHECK(requite::getIsValid(0x3B)); // ';'
    CHECK(requite::getIsValid(0x3C)); // '<'
    CHECK(requite::getIsValid(0x3D)); // '='
    CHECK(requite::getIsValid(0x3E)); // '>'
    CHECK(requite::getIsValid(0x3F)); // '?'
    CHECK(requite::getIsValid(0x40)); // '@'
    CHECK(requite::getIsValid(0x41)); // 'A'
    CHECK(requite::getIsValid(0x42)); // 'B'
    CHECK(requite::getIsValid(0x43)); // 'C'
    CHECK(requite::getIsValid(0x44)); // 'D'
    CHECK(requite::getIsValid(0x45)); // 'E'
    CHECK(requite::getIsValid(0x46)); // 'F'
    CHECK(requite::getIsValid(0x47)); // 'G'
    CHECK(requite::getIsValid(0x48)); // 'H'
    CHECK(requite::getIsValid(0x49)); // 'I'
    CHECK(requite::getIsValid(0x4A)); // 'J'
    CHECK(requite::getIsValid(0x4B)); // 'K'
    CHECK(requite::getIsValid(0x4C)); // 'L'
    CHECK(requite::getIsValid(0x4D)); // 'M'
    CHECK(requite::getIsValid(0x4E)); // 'N'
    CHECK(requite::getIsValid(0x4F)); // 'O'
    CHECK(requite::getIsValid(0x50)); // 'P'
    CHECK(requite::getIsValid(0x51)); // 'Q'
    CHECK(requite::getIsValid(0x52)); // 'R'
    CHECK(requite::getIsValid(0x53)); // 'S'
    CHECK(requite::getIsValid(0x54)); // 'T'
    CHECK(requite::getIsValid(0x55)); // 'U'
    CHECK(requite::getIsValid(0x56)); // 'V'
    CHECK(requite::getIsValid(0x57)); // 'W'
    CHECK(requite::getIsValid(0x58)); // 'X'
    CHECK(requite::getIsValid(0x59)); // 'Y'
    CHECK(requite::getIsValid(0x5A)); // 'Z'
    CHECK(requite::getIsValid(0x5B)); // '['
    CHECK(requite::getIsValid(0x5C)); // '\'
    CHECK(requite::getIsValid(0x5D)); // ']'
    CHECK(requite::getIsValid(0x5E)); // '^'
    CHECK(requite::getIsValid(0x5F)); // '_'
    CHECK(requite::getIsValid(0x60)); // '`'
    CHECK(requite::getIsValid(0x61)); // 'a'
    CHECK(requite::getIsValid(0x62)); // 'b'
    CHECK(requite::getIsValid(0x63)); // 'c'
    CHECK(requite::getIsValid(0x64)); // 'd'
    CHECK(requite::getIsValid(0x65)); // 'e'
    CHECK(requite::getIsValid(0x66)); // 'f'
    CHECK(requite::getIsValid(0x67)); // 'g'
    CHECK(requite::getIsValid(0x68)); // 'h'
    CHECK(requite::getIsValid(0x69)); // 'i'
    CHECK(requite::getIsValid(0x6A)); // 'j'
    CHECK(requite::getIsValid(0x6B)); // 'k'
    CHECK(requite::getIsValid(0x6C)); // 'l'
    CHECK(requite::getIsValid(0x6D)); // 'm'
    CHECK(requite::getIsValid(0x6E)); // 'n'
    CHECK(requite::getIsValid(0x6F)); // 'o'
    CHECK(requite::getIsValid(0x70)); // 'p'
    CHECK(requite::getIsValid(0x71)); // 'q'
    CHECK(requite::getIsValid(0x72)); // 'r'
    CHECK(requite::getIsValid(0x73)); // 's'
    CHECK(requite::getIsValid(0x74)); // 't'
    CHECK(requite::getIsValid(0x75)); // 'u'
    CHECK(requite::getIsValid(0x76)); // 'v'
    CHECK(requite::getIsValid(0x77)); // 'w'
    CHECK(requite::getIsValid(0x78)); // 'x'
    CHECK(requite::getIsValid(0x79)); // 'y'
    CHECK(requite::getIsValid(0x7A)); // 'z'
    CHECK(requite::getIsValid(0x7B)); // '{'
    CHECK(requite::getIsValid(0x7C)); // '|'
    CHECK(requite::getIsValid(0x7D)); // '}'
    CHECK(requite::getIsValid(0x7E)); // '~'
  }

  SECTION("DEL Character (0x7F)") {
    CHECK_FALSE(requite::getIsValid(0x7F)); // DEL
  }

  SECTION("Extended Characters (0x80 - 0xFF)") {
    // Extended continue characters (0x80 - 0x8F)
    for (unsigned char codeunit = 0x80; codeunit <= 0x8F; ++codeunit) {
      CHECK(requite::getIsValid(codeunit));
    }

    // Extended 2 byte start characters (0xC0 - 0xDF)
    for (unsigned char codeunit = 0xC0; codeunit <= 0xDF; ++codeunit) {
      CHECK(requite::getIsValid(codeunit));
    }

    // Extended 3 byte start characters (0xE0 - 0xEF)
    for (unsigned char codeunit = 0xE0; codeunit <= 0xEF; ++codeunit) {
      CHECK(requite::getIsValid(codeunit));
    }

    // Extended four byte start characters (0xF0 - 0xF7)
    for (unsigned char codeunit = 0xF0; codeunit <= 0xF7; ++codeunit) {
      CHECK(requite::getIsValid(codeunit));
    }

    // Invalid Extended Characters (0xF8 - 0xFF)
    for (unsigned char codeunit = 0xF8; codeunit >= 0xF8; ++codeunit) {
      CHECK_FALSE(requite::getIsValid(codeunit));
    }
  }
}
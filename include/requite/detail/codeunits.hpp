// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>
#include <requite/unreachable.hpp>

#include <llvm/ADT/StringRef.h>

#include <array>
#include <bit>
#include <cstdint>

namespace requite {

constexpr char _CHAR_EXTENDED_BIT = 0x80;

constexpr char _CHAR_CASE_BIT = 0x20;

enum _CharFlags : std::uint16_t {
  _CHAR_FLAG_NONE = 0,

  _CHAR_FLAG_SPACE = (1 << 15),
  _CHAR_FLAG_VERTICAL_SPACE = (1 << 14),
  _CHAR_FLAG_DECIMAL_DIGIT = (1 << 13),
  _CHAR_FLAG_DIGIT = (1 << 12),
  _CHAR_FLAG_NUMERIC_LITERAL = (1 << 11),
  _CHAR_FLAG_UPPERCASE_LETTER = (1 << 10),
  _CHAR_FLAG_IDENTIFIER = (1 << 9),
  _CHAR_FLAG_IDENTIFIER_START = (1 << 8),
  _CHAR_FLAG_SYMBOL = (1 << 7),
  _CHAR_FLAG_EXTENDED = (1 << 6),
  _CHAR_MASK_VALUE = 0x3F,

  _CHAR_COMMON_FLAGS_LOWERCASE_LETTER =
      (_CHAR_FLAG_DIGIT | _CHAR_FLAG_IDENTIFIER | _CHAR_FLAG_IDENTIFIER_START |
       _CHAR_FLAG_DIGIT | _CHAR_FLAG_NUMERIC_LITERAL),
  _CHAR_COMMON_FLAGS_UPPERCASE_LETTER =
      (_CHAR_FLAG_UPPERCASE_LETTER | _CHAR_FLAG_DIGIT | _CHAR_FLAG_IDENTIFIER |
       _CHAR_FLAG_IDENTIFIER_START | _CHAR_FLAG_DIGIT |
       _CHAR_FLAG_NUMERIC_LITERAL),
  _CHAR_COMMON_FLAGS_DECIMAL_DIGIT =
      (_CHAR_FLAG_DECIMAL_DIGIT | _CHAR_FLAG_DIGIT | _CHAR_FLAG_IDENTIFIER |
       _CHAR_FLAG_NUMERIC_LITERAL),
  _CHAR_COMMON_FLAGS_DIGIT_SYMBOL =
      (_CHAR_FLAG_SYMBOL | _CHAR_FLAG_DIGIT | _CHAR_FLAG_NUMERIC_LITERAL),
  _CHAR_COMMON_FLAGS_NUMERIC_SYMBOL = 
      (_CHAR_FLAG_SYMBOL | _CHAR_FLAG_NUMERIC_LITERAL),
  _CHAR_COMMON_FLAGS_HORIZONTAL_SPACE = (_CHAR_FLAG_SPACE),
  _CHAR_COMMON_FLAGS_VERTICAL_SPACE =
      (_CHAR_FLAG_SPACE | _CHAR_FLAG_VERTICAL_SPACE),
  _CHAR_COMMON_FLAGS_CONTINUE =
      (_CHAR_FLAG_IDENTIFIER_START | _CHAR_FLAG_IDENTIFIER |
       _CHAR_FLAG_EXTENDED | 0),
  _CHAR_COMMON_FLAGS_TWO_START =
      (_CHAR_FLAG_IDENTIFIER_START | _CHAR_FLAG_IDENTIFIER |
       _CHAR_FLAG_EXTENDED | 1),
  _CHAR_COMMON_FLAGS_THREE_START =
      (_CHAR_FLAG_IDENTIFIER_START | _CHAR_FLAG_IDENTIFIER |
       _CHAR_FLAG_EXTENDED | 2),
  _CHAR_COMMON_FLAGS_FOUR_START =
      (_CHAR_FLAG_IDENTIFIER_START | _CHAR_FLAG_IDENTIFIER |
       _CHAR_FLAG_EXTENDED | 3),
  _CHAR_COMMON_FLAGS_INVALID = (_CHAR_FLAG_NONE),

  _CHAR_FLAGS_NUL = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_SOH = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_STX = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_ETX = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_EOT = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_ENQ = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_ACK = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_BEL = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_BS = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_HT = _CHAR_COMMON_FLAGS_HORIZONTAL_SPACE,
  _CHAR_FLAGS_LF = _CHAR_COMMON_FLAGS_VERTICAL_SPACE,
  _CHAR_FLAGS_VT = _CHAR_COMMON_FLAGS_VERTICAL_SPACE,
  _CHAR_FLAGS_FF = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_CR = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_SO = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_SI = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_DLE = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_DC1 = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_DC2 = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_DC3 = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_DC4 = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_NAK = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_SYN = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_ETB = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_CAN = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_EM = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_SUB = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_ESC = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_FS = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_GS = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_RS = _CHAR_COMMON_FLAGS_INVALID,
  _CHAR_FLAGS_US = _CHAR_COMMON_FLAGS_INVALID,

  _CHAR_FLAGS_SPACE = _CHAR_COMMON_FLAGS_HORIZONTAL_SPACE,
  _CHAR_FLAGS_BANG = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_DOUBLE_QUOTE = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_HASH = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_DOLLAR = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_MODULO = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_AMPERPAND = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_SINGLE_QUOTE = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_OPEN_PARENTHESIS = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_CLOSE_PARENTHESIS = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_ASTERISK = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_PLUS = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_COMMA = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_MINUS = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_DOT = _CHAR_COMMON_FLAGS_NUMERIC_SYMBOL,
  _CHAR_FLAGS_FORWARD_SLASH = _CHAR_FLAG_SYMBOL,

  _CHAR_FLAGS_DIGIT_0 = (_CHAR_COMMON_FLAGS_DECIMAL_DIGIT | 0),
  _CHAR_FLAGS_DIGIT_1 = (_CHAR_COMMON_FLAGS_DECIMAL_DIGIT | 1),
  _CHAR_FLAGS_DIGIT_2 = (_CHAR_COMMON_FLAGS_DECIMAL_DIGIT | 2),
  _CHAR_FLAGS_DIGIT_3 = (_CHAR_COMMON_FLAGS_DECIMAL_DIGIT | 3),
  _CHAR_FLAGS_DIGIT_4 = (_CHAR_COMMON_FLAGS_DECIMAL_DIGIT | 4),
  _CHAR_FLAGS_DIGIT_5 = (_CHAR_COMMON_FLAGS_DECIMAL_DIGIT | 5),
  _CHAR_FLAGS_DIGIT_6 = (_CHAR_COMMON_FLAGS_DECIMAL_DIGIT | 6),
  _CHAR_FLAGS_DIGIT_7 = (_CHAR_COMMON_FLAGS_DECIMAL_DIGIT | 7),
  _CHAR_FLAGS_DIGIT_8 = (_CHAR_COMMON_FLAGS_DECIMAL_DIGIT | 8),
  _CHAR_FLAGS_DIGIT_9 = (_CHAR_COMMON_FLAGS_DECIMAL_DIGIT | 9),

  _CHAR_FLAGS_COLON = (_CHAR_COMMON_FLAGS_DIGIT_SYMBOL | 62),
  _CHAR_FLAGS_SEMICOLON = (_CHAR_COMMON_FLAGS_DIGIT_SYMBOL | 63),
  _CHAR_FLAGS_OPEN_ANGLE = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_EQUAL = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_CLOSE_ANGLE = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_QUESTION = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_AT = _CHAR_FLAG_SYMBOL,

  _CHAR_FLAGS_UPPER_A = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 36),
  _CHAR_FLAGS_UPPER_B = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 37),
  _CHAR_FLAGS_UPPER_C = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 38),
  _CHAR_FLAGS_UPPER_D = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 39),
  _CHAR_FLAGS_UPPER_E = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 40),
  _CHAR_FLAGS_UPPER_F = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 41),
  _CHAR_FLAGS_UPPER_G = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 42),
  _CHAR_FLAGS_UPPER_H = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 43),
  _CHAR_FLAGS_UPPER_I = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 44),
  _CHAR_FLAGS_UPPER_J = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 45),
  _CHAR_FLAGS_UPPER_K = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 46),
  _CHAR_FLAGS_UPPER_L = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 47),
  _CHAR_FLAGS_UPPER_M = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 48),
  _CHAR_FLAGS_UPPER_N = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 49),
  _CHAR_FLAGS_UPPER_O = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 50),
  _CHAR_FLAGS_UPPER_P = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 51),
  _CHAR_FLAGS_UPPER_Q = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 52),
  _CHAR_FLAGS_UPPER_R = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 53),
  _CHAR_FLAGS_UPPER_S = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 54),
  _CHAR_FLAGS_UPPER_T = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 55),
  _CHAR_FLAGS_UPPER_U = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 56),
  _CHAR_FLAGS_UPPER_V = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 57),
  _CHAR_FLAGS_UPPER_W = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 58),
  _CHAR_FLAGS_UPPER_X = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 59),
  _CHAR_FLAGS_UPPER_Y = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 60),
  _CHAR_FLAGS_UPPER_Z = (_CHAR_COMMON_FLAGS_UPPERCASE_LETTER | 61),

  _CHAR_FLAGS_OPEN_BRACKET = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_BACK_SLASH = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_CLOSE_BRACKET = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_CARET = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_UNDERSCORE =
      (_CHAR_FLAG_IDENTIFIER | _CHAR_FLAG_IDENTIFIER_START |
       _CHAR_FLAG_NUMERIC_LITERAL),
  _CHAR_FLAGS_GRAVE = _CHAR_FLAG_SYMBOL,

  _CHAR_FLAGS_LOWER_A = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 10),
  _CHAR_FLAGS_LOWER_B = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 11),
  _CHAR_FLAGS_LOWER_C = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 12),
  _CHAR_FLAGS_LOWER_D = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 13),
  _CHAR_FLAGS_LOWER_E = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 14),
  _CHAR_FLAGS_LOWER_F = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 15),
  _CHAR_FLAGS_LOWER_G = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 16),
  _CHAR_FLAGS_LOWER_H = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 17),
  _CHAR_FLAGS_LOWER_I = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 18),
  _CHAR_FLAGS_LOWER_J = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 19),
  _CHAR_FLAGS_LOWER_K = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 20),
  _CHAR_FLAGS_LOWER_L = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 21),
  _CHAR_FLAGS_LOWER_M = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 22),
  _CHAR_FLAGS_LOWER_N = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 23),
  _CHAR_FLAGS_LOWER_O = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 24),
  _CHAR_FLAGS_LOWER_P = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 25),
  _CHAR_FLAGS_LOWER_Q = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 26),
  _CHAR_FLAGS_LOWER_R = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 27),
  _CHAR_FLAGS_LOWER_S = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 28),
  _CHAR_FLAGS_LOWER_T = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 29),
  _CHAR_FLAGS_LOWER_U = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 30),
  _CHAR_FLAGS_LOWER_V = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 31),
  _CHAR_FLAGS_LOWER_W = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 32),
  _CHAR_FLAGS_LOWER_X = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 33),
  _CHAR_FLAGS_LOWER_Y = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 34),
  _CHAR_FLAGS_LOWER_Z = (_CHAR_COMMON_FLAGS_LOWERCASE_LETTER | 35),

  _CHAR_FLAGS_OPEN_BRACE = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_PIPE = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_CLOSE_BRACE = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_TILDE = _CHAR_FLAG_SYMBOL,
  _CHAR_FLAGS_DEL = _CHAR_COMMON_FLAGS_INVALID,
};

constexpr llvm::StringRef
getUtf8Name(char codeunit) { // these names are used for log messages.
  switch (std::bit_cast<unsigned char>(codeunit)) {
  case 0x00:
    return "NUL";
  case 0x01:
    return "SOH";
  case 0x02:
    return "STX";
  case 0x03:
    return "ETX";
  case 0x04:
    return "EOT";
  case 0x05:
    return "ENQ";
  case 0x06:
    return "ACK";
  case 0x07:
    return "BEL";
  case 0x08:
    return "BS";
  case 0x09:
    return "HT";
  case 0x0A:
    return "LF";
  case 0x0B:
    return "VT";
  case 0x0C:
    return "FF";
  case 0x0D:
    return "CR";
  case 0x0E:
    return "SO";
  case 0x0F:
    return "SI";
  case 0x10:
    return "DLE";
  case 0x11:
    return "DC1";
  case 0x12:
    return "DC2";
  case 0x13:
    return "DC3";
  case 0x14:
    return "DC4";
  case 0x15:
    return "NAK";
  case 0x16:
    return "SYN";
  case 0x17:
    return "ETB";
  case 0x18:
    return "CAN";
  case 0x19:
    return "EM";
  case 0x1A:
    return "SUB";
  case 0x1B:
    return "ESC";
  case 0x1C:
    return "FS";
  case 0x1D:
    return "GS";
  case 0x1E:
    return "RS";
  case 0x1F:
    return "US";
  case 0x20:
    return "SP";
  case 0x21:
    return "!";
  case 0x22:
    return "\"";
  case 0x23:
    return "#";
  case 0x24:
    return "$";
  case 0x25:
    return "%";
  case 0x26:
    return "&";
  case 0x27:
    return "'";
  case 0x28:
    return "(";
  case 0x29:
    return ")";
  case 0x2A:
    return "*";
  case 0x2B:
    return "+";
  case 0x2C:
    return ",";
  case 0x2D:
    return "-";
  case 0x2E:
    return ".";
  case 0x2F:
    return "/";
  case 0x30:
    return "0";
  case 0x31:
    return "1";
  case 0x32:
    return "2";
  case 0x33:
    return "3";
  case 0x34:
    return "4";
  case 0x35:
    return "5";
  case 0x36:
    return "6";
  case 0x37:
    return "7";
  case 0x38:
    return "8";
  case 0x39:
    return "9";
  case 0x3A:
    return ":";
  case 0x3B:
    return ";";
  case 0x3C:
    return "<";
  case 0x3D:
    return "=";
  case 0x3E:
    return ">";
  case 0x3F:
    return "?";
  case 0x40:
    return "@";
  case 0x41:
    return "A";
  case 0x42:
    return "B";
  case 0x43:
    return "C";
  case 0x44:
    return "D";
  case 0x45:
    return "E";
  case 0x46:
    return "F";
  case 0x47:
    return "G";
  case 0x48:
    return "H";
  case 0x49:
    return "I";
  case 0x4A:
    return "J";
  case 0x4B:
    return "K";
  case 0x4C:
    return "L";
  case 0x4D:
    return "M";
  case 0x4E:
    return "N";
  case 0x4F:
    return "O";
  case 0x50:
    return "P";
  case 0x51:
    return "Q";
  case 0x52:
    return "R";
  case 0x53:
    return "S";
  case 0x54:
    return "T";
  case 0x55:
    return "U";
  case 0x56:
    return "V";
  case 0x57:
    return "W";
  case 0x58:
    return "X";
  case 0x59:
    return "Y";
  case 0x5A:
    return "Z";
  case 0x5B:
    return "[";
  case 0x5C:
    return "\\";
  case 0x5D:
    return "]";
  case 0x5E:
    return "^";
  case 0x5F:
    return "_";
  case 0x60:
    return "`";
  case 0x61:
    return "a";
  case 0x62:
    return "b";
  case 0x63:
    return "c";
  case 0x64:
    return "d";
  case 0x65:
    return "e";
  case 0x66:
    return "f";
  case 0x67:
    return "g";
  case 0x68:
    return "h";
  case 0x69:
    return "i";
  case 0x6A:
    return "j";
  case 0x6B:
    return "k";
  case 0x6C:
    return "l";
  case 0x6D:
    return "m";
  case 0x6E:
    return "n";
  case 0x6F:
    return "o";
  case 0x70:
    return "p";
  case 0x71:
    return "q";
  case 0x72:
    return "r";
  case 0x73:
    return "s";
  case 0x74:
    return "t";
  case 0x75:
    return "u";
  case 0x76:
    return "v";
  case 0x77:
    return "w";
  case 0x78:
    return "x";
  case 0x79:
    return "y";
  case 0x7A:
    return "z";
  case 0x7B:
    return "{";
  case 0x7C:
    return "|";
  case 0x7D:
    return "}";
  case 0x7E:
    return "~";
  case 0x7F:
    return "DEL";
  // The following are internal names for debugging purposes and are not
  // official for UTF-8.
  case 0x80:
    return "CONTINUE_000000";
  case 0x81:
    return "CONTINUE_000001";
  case 0x82:
    return "CONTINUE_000010";
  case 0x83:
    return "CONTINUE_000011";
  case 0x84:
    return "CONTINUE_000100";
  case 0x85:
    return "CONTINUE_000101";
  case 0x86:
    return "CONTINUE_000110";
  case 0x87:
    return "CONTINUE_000111";
  case 0x88:
    return "CONTINUE_001000";
  case 0x89:
    return "CONTINUE_001001";
  case 0x8A:
    return "CONTINUE_001010";
  case 0x8B:
    return "CONTINUE_001011";
  case 0x8C:
    return "CONTINUE_001100";
  case 0x8D:
    return "CONTINUE_001101";
  case 0x8E:
    return "CONTINUE_001110";
  case 0x8F:
    return "CONTINUE_001111";
  case 0x90:
    return "CONTINUE_010000";
  case 0x91:
    return "CONTINUE_010001";
  case 0x92:
    return "CONTINUE_010010";
  case 0x93:
    return "CONTINUE_010011";
  case 0x94:
    return "CONTINUE_010100";
  case 0x95:
    return "CONTINUE_010101";
  case 0x96:
    return "CONTINUE_010110";
  case 0x97:
    return "CONTINUE_010111";
  case 0x98:
    return "CONTINUE_011000";
  case 0x99:
    return "CONTINUE_011001";
  case 0x9A:
    return "CONTINUE_011010";
  case 0x9B:
    return "CONTINUE_011011";
  case 0x9C:
    return "CONTINUE_011100";
  case 0x9D:
    return "CONTINUE_011101";
  case 0x9E:
    return "CONTINUE_011110";
  case 0x9F:
    return "CONTINUE_011111";
  case 0xA0:
    return "CONTINUE_100000";
  case 0xA1:
    return "CONTINUE_100001";
  case 0xA2:
    return "CONTINUE_100010";
  case 0xA3:
    return "CONTINUE_100011";
  case 0xA4:
    return "CONTINUE_100100";
  case 0xA5:
    return "CONTINUE_100101";
  case 0xA6:
    return "CONTINUE_100110";
  case 0xA7:
    return "CONTINUE_100111";
  case 0xA8:
    return "CONTINUE_101000";
  case 0xA9:
    return "CONTINUE_101001";
  case 0xAA:
    return "CONTINUE_101010";
  case 0xAB:
    return "CONTINUE_101011";
  case 0xAC:
    return "CONTINUE_101100";
  case 0xAD:
    return "CONTINUE_101101";
  case 0xAE:
    return "CONTINUE_101110";
  case 0xAF:
    return "CONTINUE_101111";
  case 0xB0:
    return "CONTINUE_110000";
  case 0xB1:
    return "CONTINUE_110001";
  case 0xB2:
    return "CONTINUE_110010";
  case 0xB3:
    return "CONTINUE_110011";
  case 0xB4:
    return "CONTINUE_110100";
  case 0xB5:
    return "CONTINUE_110101";
  case 0xB6:
    return "CONTINUE_110110";
  case 0xB7:
    return "CONTINUE_110111";
  case 0xB8:
    return "CONTINUE_111000";
  case 0xB9:
    return "CONTINUE_111001";
  case 0xBA:
    return "CONTINUE_111010";
  case 0xBB:
    return "CONTINUE_111011";
  case 0xBC:
    return "CONTINUE_111100";
  case 0xBD:
    return "CONTINUE_111101";
  case 0xBE:
    return "CONTINUE_111110";
  case 0xBF:
    return "CONTINUE_111111";
  case 0xC0:
    return "TWO_START_00000";
  case 0xC1:
    return "TWO_START_00001";
  case 0xC2:
    return "TWO_START_00010";
  case 0xC3:
    return "TWO_START_00011";
  case 0xC4:
    return "TWO_START_00100";
  case 0xC5:
    return "TWO_START_00101";
  case 0xC6:
    return "TWO_START_00110";
  case 0xC7:
    return "TWO_START_00111";
  case 0xC8:
    return "TWO_START_01000";
  case 0xC9:
    return "TWO_START_01001";
  case 0xCA:
    return "TWO_START_01010";
  case 0xCB:
    return "TWO_START_01011";
  case 0xCC:
    return "TWO_START_01100";
  case 0xCD:
    return "TWO_START_01101";
  case 0xCE:
    return "TWO_START_01110";
  case 0xCF:
    return "TWO_START_01111";
  case 0xD0:
    return "TWO_START_10000";
  case 0xD1:
    return "TWO_START_10001";
  case 0xD2:
    return "TWO_START_10010";
  case 0xD3:
    return "TWO_START_10011";
  case 0xD4:
    return "TWO_START_10100";
  case 0xD5:
    return "TWO_START_10101";
  case 0xD6:
    return "TWO_START_10110";
  case 0xD7:
    return "TWO_START_10111";
  case 0xD8:
    return "TWO_START_11000";
  case 0xD9:
    return "TWO_START_11001";
  case 0xDA:
    return "TWO_START_11010";
  case 0xDB:
    return "TWO_START_11011";
  case 0xDC:
    return "TWO_START_11100";
  case 0xDD:
    return "TWO_START_11101";
  case 0xDE:
    return "TWO_START_11110";
  case 0xDF:
    return "TWO_START_11111";
  case 0xE0:
    return "THREE_START_0000";
  case 0xE1:
    return "THREE_START_0001";
  case 0xE2:
    return "THREE_START_0010";
  case 0xE3:
    return "THREE_START_0011";
  case 0xE4:
    return "THREE_START_0100";
  case 0xE5:
    return "THREE_START_0101";
  case 0xE6:
    return "THREE_START_0110";
  case 0xE7:
    return "THREE_START_0111";
  case 0xE8:
    return "THREE_START_1000";
  case 0xE9:
    return "THREE_START_1001";
  case 0xEA:
    return "THREE_START_1010";
  case 0xEB:
    return "THREE_START_1011";
  case 0xEC:
    return "THREE_START_1100";
  case 0xED:
    return "THREE_START_1101";
  case 0xEE:
    return "THREE_START_1110";
  case 0xEF:
    return "THREE_START_1111";
  case 0xF0:
    return "FOUR_START_000";
  case 0xF1:
    return "FOUR_START_001";
  case 0xF2:
    return "FOUR_START_010";
  case 0xF3:
    return "FOUR_START_011";
  case 0xF4:
    return "FOUR_START_100";
  case 0xF5:
    return "FOUR_START_101";
  case 0xF6:
    return "FOUR_START_110";
  case 0xF7:
    return "FOUR_START_111";
  case 0xF8:
    return "INVALID_0";
  case 0xF9:
    return "INVALID_1";
  case 0xFA:
    return "INVALID_2";
  case 0xFB:
    return "INVALID_3";
  case 0xFC:
    return "INVALID_4";
  case 0xFD:
    return "INVALID_5";
  case 0xFE:
    return "INVALID_6";
  case 0xFF:
    return "INVALID_7";
  }
  REQUITE_UNREACHABLE();
}

[[nodiscard]] constexpr requite::_CharFlags _getFlags(char codeunit) {
  switch (std::bit_cast<unsigned char>(codeunit)) {
  case 0x00:
    return requite::_CHAR_FLAGS_NUL;
  case 0x01:
    return requite::_CHAR_FLAGS_SOH;
  case 0x02:
    return requite::_CHAR_FLAGS_STX;
  case 0x03:
    return requite::_CHAR_FLAGS_ETX;
  case 0x04:
    return requite::_CHAR_FLAGS_EOT;
  case 0x05:
    return requite::_CHAR_FLAGS_ENQ;
  case 0x06:
    return requite::_CHAR_FLAGS_ACK;
  case 0x07:
    return requite::_CHAR_FLAGS_BEL;
  case 0x08:
    return requite::_CHAR_FLAGS_BS;
  case 0x09:
    return requite::_CHAR_FLAGS_HT;
  case 0x0A:
    return requite::_CHAR_FLAGS_LF;
  case 0x0B:
    return requite::_CHAR_FLAGS_VT;
  case 0x0C:
    return requite::_CHAR_FLAGS_FF;
  case 0x0D:
    return requite::_CHAR_FLAGS_CR;
  case 0x0E:
    return requite::_CHAR_FLAGS_SO;
  case 0x0F:
    return requite::_CHAR_FLAGS_SI;
  case 0x10:
    return requite::_CHAR_FLAGS_DLE;
  case 0x11:
    return requite::_CHAR_FLAGS_DC1;
  case 0x12:
    return requite::_CHAR_FLAGS_DC2;
  case 0x13:
    return requite::_CHAR_FLAGS_DC3;
  case 0x14:
    return requite::_CHAR_FLAGS_DC4;
  case 0x15:
    return requite::_CHAR_FLAGS_NAK;
  case 0x16:
    return requite::_CHAR_FLAGS_SYN;
  case 0x17:
    return requite::_CHAR_FLAGS_ETB;
  case 0x18:
    return requite::_CHAR_FLAGS_CAN;
  case 0x19:
    return requite::_CHAR_FLAGS_SUB;
  case 0x1A:
    return requite::_CHAR_FLAGS_EM;
  case 0x1B:
    return requite::_CHAR_FLAGS_ESC;
  case 0x1C:
    return requite::_CHAR_FLAGS_FS;
  case 0x1D:
    return requite::_CHAR_FLAGS_GS;
  case 0x1E:
    return requite::_CHAR_FLAGS_RS;
  case 0x1F:
    return requite::_CHAR_FLAGS_US;
  case 0x20:
    return requite::_CHAR_FLAGS_SPACE;
  case 0x21:
    return requite::_CHAR_FLAGS_BANG;
  case 0x22:
    return requite::_CHAR_FLAGS_DOUBLE_QUOTE;
  case 0x23:
    return requite::_CHAR_FLAGS_HASH;
  case 0x24:
    return requite::_CHAR_FLAGS_DOLLAR;
  case 0x25:
    return requite::_CHAR_FLAGS_MODULO;
  case 0x26:
    return requite::_CHAR_FLAGS_AMPERPAND;
  case 0x27:
    return requite::_CHAR_FLAGS_SINGLE_QUOTE;
  case 0x28:
    return requite::_CHAR_FLAGS_OPEN_PARENTHESIS;
  case 0x29:
    return requite::_CHAR_FLAGS_CLOSE_PARENTHESIS;
  case 0x2A:
    return requite::_CHAR_FLAGS_ASTERISK;
  case 0x2B:
    return requite::_CHAR_FLAGS_PLUS;
  case 0x2C:
    return requite::_CHAR_FLAGS_COMMA;
  case 0x2D:
    return requite::_CHAR_FLAGS_MINUS;
  case 0x2E:
    return requite::_CHAR_FLAGS_DOT;
  case 0x2F:
    return requite::_CHAR_FLAGS_FORWARD_SLASH;
  case 0x30:
    return requite::_CHAR_FLAGS_DIGIT_0;
  case 0x31:
    return requite::_CHAR_FLAGS_DIGIT_1;
  case 0x32:
    return requite::_CHAR_FLAGS_DIGIT_2;
  case 0x33:
    return requite::_CHAR_FLAGS_DIGIT_3;
  case 0x34:
    return requite::_CHAR_FLAGS_DIGIT_4;
  case 0x35:
    return requite::_CHAR_FLAGS_DIGIT_5;
  case 0x36:
    return requite::_CHAR_FLAGS_DIGIT_6;
  case 0x37:
    return requite::_CHAR_FLAGS_DIGIT_7;
  case 0x38:
    return requite::_CHAR_FLAGS_DIGIT_8;
  case 0x39:
    return requite::_CHAR_FLAGS_DIGIT_9;
  case 0x3A:
    return requite::_CHAR_FLAGS_COLON;
  case 0x3B:
    return requite::_CHAR_FLAGS_SEMICOLON;
  case 0x3C:
    return requite::_CHAR_FLAGS_OPEN_ANGLE;
  case 0x3D:
    return requite::_CHAR_FLAGS_EQUAL;
  case 0x3E:
    return requite::_CHAR_FLAGS_CLOSE_ANGLE;
  case 0x3F:
    return requite::_CHAR_FLAGS_QUESTION;
  case 0x40:
    return requite::_CHAR_FLAGS_AT;
  case 0x41:
    return requite::_CHAR_FLAGS_UPPER_A;
  case 0x42:
    return requite::_CHAR_FLAGS_UPPER_B;
  case 0x43:
    return requite::_CHAR_FLAGS_UPPER_C;
  case 0x44:
    return requite::_CHAR_FLAGS_UPPER_D;
  case 0x45:
    return requite::_CHAR_FLAGS_UPPER_E;
  case 0x46:
    return requite::_CHAR_FLAGS_UPPER_F;
  case 0x47:
    return requite::_CHAR_FLAGS_UPPER_G;
  case 0x48:
    return requite::_CHAR_FLAGS_UPPER_H;
  case 0x49:
    return requite::_CHAR_FLAGS_UPPER_I;
  case 0x4A:
    return requite::_CHAR_FLAGS_UPPER_J;
  case 0x4B:
    return requite::_CHAR_FLAGS_UPPER_K;
  case 0x4C:
    return requite::_CHAR_FLAGS_UPPER_L;
  case 0x4D:
    return requite::_CHAR_FLAGS_UPPER_M;
  case 0x4E:
    return requite::_CHAR_FLAGS_UPPER_N;
  case 0x4F:
    return requite::_CHAR_FLAGS_UPPER_O;
  case 0x50:
    return requite::_CHAR_FLAGS_UPPER_P;
  case 0x51:
    return requite::_CHAR_FLAGS_UPPER_Q;
  case 0x52:
    return requite::_CHAR_FLAGS_UPPER_R;
  case 0x53:
    return requite::_CHAR_FLAGS_UPPER_S;
  case 0x54:
    return requite::_CHAR_FLAGS_UPPER_T;
  case 0x55:
    return requite::_CHAR_FLAGS_UPPER_U;
  case 0x56:
    return requite::_CHAR_FLAGS_UPPER_V;
  case 0x57:
    return requite::_CHAR_FLAGS_UPPER_W;
  case 0x58:
    return requite::_CHAR_FLAGS_UPPER_X;
  case 0x59:
    return requite::_CHAR_FLAGS_UPPER_Y;
  case 0x5A:
    return requite::_CHAR_FLAGS_UPPER_Z;
  case 0x5B:
    return requite::_CHAR_FLAGS_OPEN_BRACKET;
  case 0x5C:
    return requite::_CHAR_FLAGS_BACK_SLASH;
  case 0x5D:
    return requite::_CHAR_FLAGS_CLOSE_BRACKET;
  case 0x5E:
    return requite::_CHAR_FLAGS_CARET;
  case 0x5F:
    return requite::_CHAR_FLAGS_UNDERSCORE;
  case 0x60:
    return requite::_CHAR_FLAGS_GRAVE;
  case 0x61:
    return requite::_CHAR_FLAGS_LOWER_A;
  case 0x62:
    return requite::_CHAR_FLAGS_LOWER_B;
  case 0x63:
    return requite::_CHAR_FLAGS_LOWER_C;
  case 0x64:
    return requite::_CHAR_FLAGS_LOWER_D;
  case 0x65:
    return requite::_CHAR_FLAGS_LOWER_E;
  case 0x66:
    return requite::_CHAR_FLAGS_LOWER_F;
  case 0x67:
    return requite::_CHAR_FLAGS_LOWER_G;
  case 0x68:
    return requite::_CHAR_FLAGS_LOWER_H;
  case 0x69:
    return requite::_CHAR_FLAGS_LOWER_I;
  case 0x6A:
    return requite::_CHAR_FLAGS_LOWER_J;
  case 0x6B:
    return requite::_CHAR_FLAGS_LOWER_K;
  case 0x6C:
    return requite::_CHAR_FLAGS_LOWER_L;
  case 0x6D:
    return requite::_CHAR_FLAGS_LOWER_M;
  case 0x6E:
    return requite::_CHAR_FLAGS_LOWER_N;
  case 0x6F:
    return requite::_CHAR_FLAGS_LOWER_O;
  case 0x70:
    return requite::_CHAR_FLAGS_LOWER_P;
  case 0x71:
    return requite::_CHAR_FLAGS_LOWER_Q;
  case 0x72:
    return requite::_CHAR_FLAGS_LOWER_R;
  case 0x73:
    return requite::_CHAR_FLAGS_LOWER_S;
  case 0x74:
    return requite::_CHAR_FLAGS_LOWER_T;
  case 0x75:
    return requite::_CHAR_FLAGS_LOWER_U;
  case 0x76:
    return requite::_CHAR_FLAGS_LOWER_V;
  case 0x77:
    return requite::_CHAR_FLAGS_LOWER_W;
  case 0x78:
    return requite::_CHAR_FLAGS_LOWER_X;
  case 0x79:
    return requite::_CHAR_FLAGS_LOWER_Y;
  case 0x7A:
    return requite::_CHAR_FLAGS_LOWER_Z;
  case 0x7B:
    return requite::_CHAR_FLAGS_OPEN_BRACE;
  case 0x7C:
    return requite::_CHAR_FLAGS_PIPE;
  case 0x7D:
    return requite::_CHAR_FLAGS_CLOSE_BRACE;
  case 0x7E:
    return requite::_CHAR_FLAGS_TILDE;
  case 0x7F:
    return requite::_CHAR_FLAGS_DEL;
  case 0x80:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x81:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x82:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x83:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x84:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x85:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x86:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x87:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x88:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x89:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x8A:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x8B:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x8C:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x8D:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x8E:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x8F:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x90:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x91:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x92:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x93:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x94:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x95:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x96:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x97:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x98:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x99:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x9A:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x9B:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x9C:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x9D:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x9E:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0x9F:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xA0:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xA1:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xA2:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xA3:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xA4:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xA5:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xA6:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xA7:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xA8:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xA9:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xAA:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xAB:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xAC:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xAD:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xAE:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xAF:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xB0:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xB1:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xB2:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xB3:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xB4:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xB5:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xB6:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xB7:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xB8:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xB9:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xBA:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xBB:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xBC:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xBD:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xBE:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xBF:
    return requite::_CHAR_COMMON_FLAGS_CONTINUE;
  case 0xC0:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xC1:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xC2:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xC3:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xC4:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xC5:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xC6:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xC7:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xC8:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xC9:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xCA:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xCB:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xCC:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xCD:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xCE:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xCF:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xD0:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xD1:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xD2:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xD3:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xD4:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xD5:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xD6:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xD7:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xD8:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xD9:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xDA:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xDB:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xDC:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xDD:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xDE:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xDF:
    return requite::_CHAR_COMMON_FLAGS_TWO_START;
  case 0xE0:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xE1:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xE2:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xE3:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xE4:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xE5:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xE6:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xE7:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xE8:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xE9:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xEA:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xEB:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xEC:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xED:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xEE:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xEF:
    return requite::_CHAR_COMMON_FLAGS_THREE_START;
  case 0xF0:
    return requite::_CHAR_COMMON_FLAGS_FOUR_START;
  case 0xF1:
    return requite::_CHAR_COMMON_FLAGS_FOUR_START;
  case 0xF2:
    return requite::_CHAR_COMMON_FLAGS_FOUR_START;
  case 0xF3:
    return requite::_CHAR_COMMON_FLAGS_FOUR_START;
  case 0xF4:
    return requite::_CHAR_COMMON_FLAGS_FOUR_START;
  case 0xF5:
    return requite::_CHAR_COMMON_FLAGS_FOUR_START;
  case 0xF6:
    return requite::_CHAR_COMMON_FLAGS_FOUR_START;
  case 0xF7:
    return requite::_CHAR_COMMON_FLAGS_FOUR_START;
  case 0xF8:
    return requite::_CHAR_COMMON_FLAGS_INVALID;
  case 0xF9:
    return requite::_CHAR_COMMON_FLAGS_INVALID;
  case 0xFA:
    return requite::_CHAR_COMMON_FLAGS_INVALID;
  case 0xFB:
    return requite::_CHAR_COMMON_FLAGS_INVALID;
  case 0xFC:
    return requite::_CHAR_COMMON_FLAGS_INVALID;
  case 0xFD:
    return requite::_CHAR_COMMON_FLAGS_INVALID;
  case 0xFE:
    return requite::_CHAR_COMMON_FLAGS_INVALID;
  case 0xFF:
    return requite::_CHAR_COMMON_FLAGS_INVALID;
  }
  REQUITE_UNREACHABLE();
}

[[nodiscard]] constexpr bool _getHasFlags(char codeunit,
                                          requite::_CharFlags flags) {
  const requite::_CharFlags codeunit_flags = requite::_getFlags(codeunit);
  const bool has_flags = (codeunit_flags & flags) == flags;
  return has_flags;
}

[[nodiscard]] constexpr unsigned char _getMaskValue(char codeunit) {
  const requite::_CharFlags codeunit_flags = requite::_getFlags(codeunit);
  const unsigned char value = (codeunit_flags & requite::_CHAR_MASK_VALUE);
  return value;
}

constexpr bool getIsWhitespace(char codeunit) {
  const bool has_flags =
      requite::_getHasFlags(codeunit, requite::_CHAR_FLAGS_SPACE);
  return has_flags;
}

constexpr bool getIsVerticalSpace(char codeunit) {
  const bool has_flags =
      requite::_getHasFlags(codeunit, requite::_CHAR_FLAG_VERTICAL_SPACE);
  return has_flags;
}

constexpr bool getIsDecimalDigit(char codeunit) {
  const bool has_flags =
      requite::_getHasFlags(codeunit, requite::_CHAR_FLAG_DECIMAL_DIGIT);
  return has_flags;
}

constexpr bool getIsDigit(char codeunit) {
  const bool has_flags =
      requite::_getHasFlags(codeunit, requite::_CHAR_FLAG_DIGIT);
  return has_flags;
}

constexpr bool getIsNumericLiteral(char codeunit) {
  const bool has_flags =
      requite::_getHasFlags(codeunit, requite::_CHAR_FLAG_NUMERIC_LITERAL);
  return has_flags;
}

constexpr unsigned char getDigitBaseMultiplier(char codeunit) {
  REQUITE_ASSERT(requite::getIsDigit(codeunit));
  unsigned char base_multiplier = requite::_getMaskValue(codeunit);
  return base_multiplier;
}

constexpr bool getIsUppercase(char codeunit) {
  const bool has_flags =
      requite::_getHasFlags(codeunit, requite::_CHAR_FLAG_UPPERCASE_LETTER);
  return has_flags;
}

constexpr char getLowercase(char codeunit) {
  if (requite::getIsUppercase(codeunit)) {
    return codeunit ^ requite::_CHAR_CASE_BIT;
  }
  return codeunit;
}

constexpr bool getIsIdentifier(char codeunit) {
  const bool has_flags =
      requite::_getHasFlags(codeunit, requite::_CHAR_FLAG_IDENTIFIER);
  return has_flags;
}

constexpr bool getIsIdentifierStart(char codeunit) {
  const bool has_flags =
      requite::_getHasFlags(codeunit, requite::_CHAR_FLAG_IDENTIFIER_START);
  return has_flags;
}

constexpr bool getIsSymbol(char codeunit) {
  const bool has_flags =
      requite::_getHasFlags(codeunit, requite::_CHAR_FLAG_SYMBOL);
  return has_flags;
}

constexpr bool getIsExtended(char codeunit) {
  const bool has_flags =
      requite::_getHasFlags(codeunit, requite::_CHAR_FLAG_EXTENDED);
  return has_flags;
}

constexpr unsigned char getExtendedStartCount(char codeunit) {
  REQUITE_ASSERT(requite::getIsExtended(codeunit));
  const unsigned char value = requite::_getMaskValue(codeunit);
  return value;
}

constexpr bool getIsValid(char codeunit) {
  const requite::_CharFlags codeunit_flags = requite::_getFlags(codeunit);
  const bool is_valid = codeunit_flags != requite::_CHAR_COMMON_FLAGS_INVALID;
  return is_valid;
}

constexpr llvm::StringRef getIntermediateFileEscapeSequence(
    char codeunit) { // escape sequences used for output to intermediate files.
  switch (std::bit_cast<unsigned char>(codeunit)) {
  case 0x00:
    return "\\00\\";
  case 0x01:
    return "\\01\\";
  case 0x02:
    return "\\02\\";
  case 0x03:
    return "\\03\\";
  case 0x04:
    return "\\04\\";
  case 0x05:
    return "\\05\\";
  case 0x06:
    return "\\06\\";
  case 0x07:
    return "\\a";
  case 0x08:
    return "\\b";
  case 0x09:
    return "\\t";
  case 0x0A:
    return "\\0A\\";
  case 0x0B:
    return "\\v";
  case 0x0C:
    return "\\f";
  case 0x0D:
    return "\\r";
  case 0x0E:
    return "\\0E\\";
  case 0x0F:
    return "\\0F\\";
  case 0x10:
    return "\\10\\";
  case 0x11:
    return "\\11\\";
  case 0x12:
    return "\\12\\";
  case 0x13:
    return "\\13\\";
  case 0x14:
    return "\\14\\";
  case 0x15:
    return "\\15\\";
  case 0x16:
    return "\\16\\";
  case 0x17:
    return "\\17\\";
  case 0x18:
    return "\\18\\";
  case 0x19:
    return "\\19\\";
  case 0x1A:
    return "\\1A\\";
  case 0x1B:
    return "\\e";
  case 0x1C:
    return "\\1C\\";
  case 0x1D:
    return "\\1D\\";
  case 0x1E:
    return "\\1E\\";
  case 0x1F:
    return "\\1F\\";
  case 0x20:
    return " ";
  case 0x21:
    return "!";
  case 0x22:
    return "\\\"";
  case 0x23:
    return "#";
  case 0x24:
    return "$";
  case 0x25:
    return "%";
  case 0x26:
    return "&";
  case 0x27:
    return "'";
  case 0x28:
    return "(";
  case 0x29:
    return ")";
  case 0x2A:
    return "*";
  case 0x2B:
    return "+";
  case 0x2C:
    return ",";
  case 0x2D:
    return "-";
  case 0x2E:
    return ".";
  case 0x2F:
    return "/";
  case 0x30:
    return "0";
  case 0x31:
    return "1";
  case 0x32:
    return "2";
  case 0x33:
    return "3";
  case 0x34:
    return "4";
  case 0x35:
    return "5";
  case 0x36:
    return "6";
  case 0x37:
    return "7";
  case 0x38:
    return "8";
  case 0x39:
    return "9";
  case 0x3A:
    return ":";
  case 0x3B:
    return ";";
  case 0x3C:
    return "<";
  case 0x3D:
    return "=";
  case 0x3E:
    return ">";
  case 0x3F:
    return "?";
  case 0x40:
    return "@";
  case 0x41:
    return "A";
  case 0x42:
    return "B";
  case 0x43:
    return "C";
  case 0x44:
    return "D";
  case 0x45:
    return "E";
  case 0x46:
    return "F";
  case 0x47:
    return "G";
  case 0x48:
    return "H";
  case 0x49:
    return "I";
  case 0x4A:
    return "J";
  case 0x4B:
    return "K";
  case 0x4C:
    return "L";
  case 0x4D:
    return "M";
  case 0x4E:
    return "N";
  case 0x4F:
    return "O";
  case 0x50:
    return "P";
  case 0x51:
    return "Q";
  case 0x52:
    return "R";
  case 0x53:
    return "S";
  case 0x54:
    return "T";
  case 0x55:
    return "U";
  case 0x56:
    return "V";
  case 0x57:
    return "W";
  case 0x58:
    return "X";
  case 0x59:
    return "Y";
  case 0x5A:
    return "Z";
  case 0x5B:
    return "[";
  case 0x5C:
    return "\\\\";
  case 0x5D:
    return "]";
  case 0x5E:
    return "^";
  case 0x5F:
    return "_";
  case 0x60:
    return "\\`";
  case 0x61:
    return "a";
  case 0x62:
    return "b";
  case 0x63:
    return "c";
  case 0x64:
    return "d";
  case 0x65:
    return "e";
  case 0x66:
    return "f";
  case 0x67:
    return "g";
  case 0x68:
    return "h";
  case 0x69:
    return "i";
  case 0x6A:
    return "j";
  case 0x6B:
    return "k";
  case 0x6C:
    return "l";
  case 0x6D:
    return "m";
  case 0x6E:
    return "n";
  case 0x6F:
    return "o";
  case 0x70:
    return "p";
  case 0x71:
    return "q";
  case 0x72:
    return "r";
  case 0x73:
    return "s";
  case 0x74:
    return "t";
  case 0x75:
    return "u";
  case 0x76:
    return "v";
  case 0x77:
    return "w";
  case 0x78:
    return "x";
  case 0x79:
    return "y";
  case 0x7A:
    return "z";
  case 0x7B:
    return "{";
  case 0x7C:
    return "|";
  case 0x7D:
    return "}";
  case 0x7E:
    return "~";
  case 0x7F:
    return "\\7F\\";
  case 0x80:
    return "\\80\\";
  case 0x81:
    return "\\81\\";
  case 0x82:
    return "\\82\\";
  case 0x83:
    return "\\83\\";
  case 0x84:
    return "\\84\\";
  case 0x85:
    return "\\85\\";
  case 0x86:
    return "\\86\\";
  case 0x87:
    return "\\87\\";
  case 0x88:
    return "\\88\\";
  case 0x89:
    return "\\89\\";
  case 0x8A:
    return "\\8A\\";
  case 0x8B:
    return "\\8B\\";
  case 0x8C:
    return "\\8C\\";
  case 0x8D:
    return "\\8D\\";
  case 0x8E:
    return "\\8E\\";
  case 0x8F:
    return "\\8F\\";
  case 0x90:
    return "\\90\\";
  case 0x91:
    return "\\91\\";
  case 0x92:
    return "\\92\\";
  case 0x93:
    return "\\93\\";
  case 0x94:
    return "\\94\\";
  case 0x95:
    return "\\95\\";
  case 0x96:
    return "\\96\\";
  case 0x97:
    return "\\97\\";
  case 0x98:
    return "\\98\\";
  case 0x99:
    return "\\99\\";
  case 0x9A:
    return "\\9A\\";
  case 0x9B:
    return "\\9B\\";
  case 0x9C:
    return "\\9C\\";
  case 0x9D:
    return "\\9D\\";
  case 0x9E:
    return "\\9E\\";
  case 0x9F:
    return "\\9F\\";
  case 0xA0:
    return "\\A0\\";
  case 0xA1:
    return "\\A1\\";
  case 0xA2:
    return "\\A2\\";
  case 0xA3:
    return "\\A3\\";
  case 0xA4:
    return "\\A4\\";
  case 0xA5:
    return "\\A5\\";
  case 0xA6:
    return "\\A6\\";
  case 0xA7:
    return "\\A7\\";
  case 0xA8:
    return "\\A8\\";
  case 0xA9:
    return "\\A9\\";
  case 0xAA:
    return "\\AA\\";
  case 0xAB:
    return "\\AB\\";
  case 0xAC:
    return "\\AC\\";
  case 0xAD:
    return "\\AD\\";
  case 0xAE:
    return "\\AE\\";
  case 0xAF:
    return "\\AF\\";
  case 0xB0:
    return "\\B0\\";
  case 0xB1:
    return "\\B1\\";
  case 0xB2:
    return "\\B2\\";
  case 0xB3:
    return "\\B3\\";
  case 0xB4:
    return "\\B4\\";
  case 0xB5:
    return "\\B5\\";
  case 0xB6:
    return "\\B6\\";
  case 0xB7:
    return "\\B7\\";
  case 0xB8:
    return "\\B8\\";
  case 0xB9:
    return "\\B9\\";
  case 0xBA:
    return "\\BA\\";
  case 0xBB:
    return "\\BB\\";
  case 0xBC:
    return "\\BC\\";
  case 0xBD:
    return "\\BD\\";
  case 0xBE:
    return "\\BE\\";
  case 0xBF:
    return "\\BF\\";
  case 0xC0:
    return "\\C0\\";
  case 0xC1:
    return "\\C1\\";
  case 0xC2:
    return "\\C2\\";
  case 0xC3:
    return "\\C3\\";
  case 0xC4:
    return "\\C4\\";
  case 0xC5:
    return "\\C5\\";
  case 0xC6:
    return "\\C6\\";
  case 0xC7:
    return "\\C7\\";
  case 0xC8:
    return "\\C8\\";
  case 0xC9:
    return "\\C9\\";
  case 0xCA:
    return "\\CA\\";
  case 0xCB:
    return "\\CB\\";
  case 0xCC:
    return "\\CC\\";
  case 0xCD:
    return "\\CD\\";
  case 0xCE:
    return "\\CE\\";
  case 0xCF:
    return "\\CF\\";
  case 0xD0:
    return "\\D0\\";
  case 0xD1:
    return "\\D1\\";
  case 0xD2:
    return "\\D2\\";
  case 0xD3:
    return "\\D3\\";
  case 0xD4:
    return "\\D4\\";
  case 0xD5:
    return "\\D5\\";
  case 0xD6:
    return "\\D6\\";
  case 0xD7:
    return "\\D7\\";
  case 0xD8:
    return "\\D8\\";
  case 0xD9:
    return "\\D9\\";
  case 0xDA:
    return "\\DA\\";
  case 0xDB:
    return "\\DB\\";
  case 0xDC:
    return "\\DC\\";
  case 0xDD:
    return "\\DD\\";
  case 0xDE:
    return "\\DE\\";
  case 0xDF:
    return "\\DF\\";
  case 0xE0:
    return "\\E0\\";
  case 0xE1:
    return "\\E1\\";
  case 0xE2:
    return "\\E2\\";
  case 0xE3:
    return "\\E3\\";
  case 0xE4:
    return "\\E4\\";
  case 0xE5:
    return "\\E5\\";
  case 0xE6:
    return "\\E6\\";
  case 0xE7:
    return "\\E7\\";
  case 0xE8:
    return "\\E8\\";
  case 0xE9:
    return "\\E9\\";
  case 0xEA:
    return "\\EA\\";
  case 0xEB:
    return "\\EB\\";
  case 0xEC:
    return "\\EC\\";
  case 0xED:
    return "\\ED\\";
  case 0xEE:
    return "\\EE\\";
  case 0xEF:
    return "\\EF\\";
  case 0xF0:
    return "\\F0\\";
  case 0xF1:
    return "\\F1\\";
  case 0xF2:
    return "\\F2\\";
  case 0xF3:
    return "\\F3\\";
  case 0xF4:
    return "\\F4\\";
  case 0xF5:
    return "\\F5\\";
  case 0xF6:
    return "\\F6\\";
  case 0xF7:
    return "\\F7\\";
  case 0xF8:
    return "\\F8\\";
  case 0xF9:
    return "\\F9\\";
  case 0xFA:
    return "\\FA\\";
  case 0xFB:
    return "\\FB\\";
  case 0xFC:
    return "\\FC\\";
  case 0xFD:
    return "\\FD\\";
  case 0xFE:
    return "\\FE\\";
  case 0xFF:
    return "\\FF\\";
  }
  REQUITE_UNREACHABLE();
}

} // namespace requite
#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>

#include <bit>
#include <cstdint>

namespace rq {

enum class CodeunitFlags : std::uint16_t {
  NONE = 0,
  HORIZONTAL_WHITESPACE = rq::getBit(15),
  VERTICAL_WHITESPACE = rq::getBit(14),
  DECIMAL_DIGIT = rq::getBit(13),
  EXTENDED_DIGIT = rq::getBit(12),
  NUMERIC = rq::getBit(11),
  UPPERCASE_LETTER = rq::getBit(10),
  LOWERCASE_LETTER = rq::getBit(9),
  IDENTIFIER_FRONT = rq::getBit(8),
  IDENTIFIER_BODY = rq::getBit(7),
  SYMBOL = rq::getBit(6),
  MASK = 0x3F
};

template <> struct is_flags<rq::CodeunitFlags> : std::true_type {};

constexpr char CHAR_EXTENDED_BIT = 0x80;

constexpr char CHAR_CASE_BIT = 0x20;

[[nodiscard]] RQ_ALWAYS_INLINE constexpr llvm::StringRef
getUtf8Name(char codeunit) {
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
  return "unknown";
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr rq::CodeunitFlags
getFlags(char codeunit) {
  using namespace rq;
  using CF = CodeunitFlags;
  constexpr CF INVALID = CF::NONE;
  constexpr CF HORIZONTAL_WHITESPACE = CF::HORIZONTAL_WHITESPACE;
  constexpr CF VERTICAL_WHITESPACE = CF::VERTICAL_WHITESPACE;
  constexpr CF DECIMAL_DIGIT =
      CF::NUMERIC | CF::DECIMAL_DIGIT | CF::IDENTIFIER_BODY;
  constexpr CF UPPERCASE_LETTER = CF::NUMERIC | CF::EXTENDED_DIGIT |
                                  CF::UPPERCASE_LETTER | CF::IDENTIFIER_FRONT |
                                  CF::IDENTIFIER_BODY;
  constexpr CF LOWERCASE_LETTER = CF::NUMERIC | CF::EXTENDED_DIGIT |
                                  CF::LOWERCASE_LETTER | CF::IDENTIFIER_FRONT |
                                  CF::IDENTIFIER_BODY;
  constexpr CF SYMBOL = CF::SYMBOL;
  constexpr CF UNDERSCORE =
      CF::NUMERIC | CF::IDENTIFIER_FRONT | CF::IDENTIFIER_BODY;
  constexpr CF DOT = CF::NUMERIC | CF::SYMBOL;
  constexpr CF DIGIT_SYMBOL = CF::SYMBOL | CF::EXTENDED_DIGIT;
  constexpr CF CONTINUE = CF::NONE | 0;
  constexpr CF TWO_START = CF::NONE | 1;
  constexpr CF THREE_START = CF::NONE | 2;
  constexpr CF FOUR_START = CF::NONE | 3;
  switch (std::bit_cast<unsigned char>(codeunit)) {
  case 0x00: // NUL
    return INVALID;
  case 0x01: // SOH
    return INVALID;
  case 0x02: // STX
    return INVALID;
  case 0x03: // ETX
    return INVALID;
  case 0x04: // EOT
    return INVALID;
  case 0x05: // ENQ
    return INVALID;
  case 0x06: // ACK
    return INVALID;
  case 0x07: // BEL
    return INVALID;
  case 0x08: // BS
    return INVALID;
  case 0x09: // HT
    return HORIZONTAL_WHITESPACE;
  case 0x0A: // LF
    return HORIZONTAL_WHITESPACE;
  case 0x0B: // VT
    return VERTICAL_WHITESPACE;
  case 0x0C: // FF
    return INVALID;
  case 0x0D: // CR
    return VERTICAL_WHITESPACE;
  case 0x0E: // SO
    return INVALID;
  case 0x0F: // SI
    return INVALID;
  case 0x10: // DLE
    return INVALID;
  case 0x11: // DC1
    return INVALID;
  case 0x12: // DC2
    return INVALID;
  case 0x13: // DC3
    return INVALID;
  case 0x14: // DC4
    return INVALID;
  case 0x15: // NAK
    return INVALID;
  case 0x16: // SYN
    return INVALID;
  case 0x17: // ETB
    return INVALID;
  case 0x18: // CAN
    return INVALID;
  case 0x19: // EM
    return INVALID;
  case 0x1A: // SUB
    return INVALID;
  case 0x1B: // ESC
    return INVALID;
  case 0x1C: // FS
    return INVALID;
  case 0x1D: // GS
    return INVALID;
  case 0x1E: // RS
    return INVALID;
  case 0x1F: // US
    return INVALID;
  case 0x20: // ' '
    return HORIZONTAL_WHITESPACE;
  case 0x21: // '!'
    return DIGIT_SYMBOL | 63;
  case 0x22: // '\"'
    return SYMBOL;
  case 0x23: // '#'
    return SYMBOL;
  case 0x24: // '$'
    return SYMBOL;
  case 0x25: // '%'
    return SYMBOL;
  case 0x26: // '&'
    return SYMBOL;
  case 0x27: // '\''
    return SYMBOL;
  case 0x28: // '('
    return SYMBOL;
  case 0x29: // ')'
    return SYMBOL;
  case 0x2A: // '*'
    return SYMBOL;
  case 0x2B: // '+'
    return SYMBOL;
  case 0x2C: // ','
    return SYMBOL;
  case 0x2D: // '-'
    return SYMBOL;
  case 0x2E: // '.'
    return DOT;
  case 0x2F: // '/"
    return SYMBOL;
  case 0x30: // '0'
    return DECIMAL_DIGIT | 0;
  case 0x31: // '1'
    return DECIMAL_DIGIT | 1;
  case 0x32: // '2'
    return DECIMAL_DIGIT | 2;
  case 0x33: // '3'
    return DECIMAL_DIGIT | 3;
  case 0x34: // '4'
    return DECIMAL_DIGIT | 4;
  case 0x35: // '5'
    return DECIMAL_DIGIT | 5;
  case 0x36: // '6'
    return DECIMAL_DIGIT | 6;
  case 0x37: // '7'
    return DECIMAL_DIGIT | 7;
  case 0x38: // '8'
    return DECIMAL_DIGIT | 8;
  case 0x39: // '9'
    return DECIMAL_DIGIT | 9;
  case 0x3A: // ':'
    return SYMBOL;
  case 0x3B: // ';'
    return SYMBOL;
  case 0x3C: // '<'
    return SYMBOL;
  case 0x3D: // '='
    return SYMBOL;
  case 0x3E: // '>'
    return SYMBOL;
  case 0x3F: // '?'
    return DIGIT_SYMBOL | 62;
  case 0x40: // '@'
    return SYMBOL;
  case 0x41: // 'A'
    return UPPERCASE_LETTER | 36;
  case 0x42: // 'B'
    return UPPERCASE_LETTER | 37;
  case 0x43: // 'C'
    return UPPERCASE_LETTER | 38;
  case 0x44: // 'D'
    return UPPERCASE_LETTER | 39;
  case 0x45: // 'E'
    return UPPERCASE_LETTER | 40;
  case 0x46: // 'F'
    return UPPERCASE_LETTER | 41;
  case 0x47: // 'G'
    return UPPERCASE_LETTER | 42;
  case 0x48: // 'H'
    return UPPERCASE_LETTER | 43;
  case 0x49: // 'I'
    return UPPERCASE_LETTER | 44;
  case 0x4A: // 'J'
    return UPPERCASE_LETTER | 45;
  case 0x4B: // 'K'
    return UPPERCASE_LETTER | 46;
  case 0x4C: // 'L'
    return UPPERCASE_LETTER | 47;
  case 0x4D: // 'M'
    return UPPERCASE_LETTER | 48;
  case 0x4E: // 'N'
    return UPPERCASE_LETTER | 49;
  case 0x4F: // 'O'
    return UPPERCASE_LETTER | 50;
  case 0x50: // 'P'
    return UPPERCASE_LETTER | 51;
  case 0x51: // 'Q'
    return UPPERCASE_LETTER | 52;
  case 0x52: // 'R'
    return UPPERCASE_LETTER | 53;
  case 0x53: // 'S'
    return UPPERCASE_LETTER | 54;
  case 0x54: // 'T'
    return UPPERCASE_LETTER | 55;
  case 0x55: // 'U'
    return UPPERCASE_LETTER | 56;
  case 0x56: // 'V'
    return UPPERCASE_LETTER | 57;
  case 0x57: // 'W'
    return UPPERCASE_LETTER | 58;
  case 0x58: // 'X'
    return UPPERCASE_LETTER | 59;
  case 0x59: // 'Y'
    return UPPERCASE_LETTER | 60;
  case 0x5A: // 'Z'
    return UPPERCASE_LETTER | 61;
  case 0x5B: // '['
    return SYMBOL;
  case 0x5C: // '\\'
    return SYMBOL;
  case 0x5D: // ']'
    return SYMBOL;
  case 0x5E: // '^'
    return SYMBOL;
  case 0x5F: // '_'
    return UNDERSCORE;
  case 0x60: // '\''
    return SYMBOL;
  case 0x61: // 'a'
    return LOWERCASE_LETTER | 10;
  case 0x62: // 'b'
    return LOWERCASE_LETTER | 11;
  case 0x63: // 'c'
    return LOWERCASE_LETTER | 12;
  case 0x64: // 'd'
    return LOWERCASE_LETTER | 13;
  case 0x65: // 'e'
    return LOWERCASE_LETTER | 14;
  case 0x66: // 'f'
    return LOWERCASE_LETTER | 15;
  case 0x67: // 'g'
    return LOWERCASE_LETTER | 16;
  case 0x68: // 'h'
    return LOWERCASE_LETTER | 17;
  case 0x69: // 'i'
    return LOWERCASE_LETTER | 18;
  case 0x6A: // 'j'
    return LOWERCASE_LETTER | 19;
  case 0x6B: // 'k'
    return LOWERCASE_LETTER | 20;
  case 0x6C: // 'l'
    return LOWERCASE_LETTER | 21;
  case 0x6D: // 'm'
    return LOWERCASE_LETTER | 22;
  case 0x6E: // 'n'
    return LOWERCASE_LETTER | 23;
  case 0x6F: // 'o'
    return LOWERCASE_LETTER | 24;
  case 0x70: // 'p'
    return LOWERCASE_LETTER | 25;
  case 0x71: // 'q'
    return LOWERCASE_LETTER | 26;
  case 0x72: // 'r'
    return LOWERCASE_LETTER | 27;
  case 0x73: // 's'
    return LOWERCASE_LETTER | 28;
  case 0x74: // 's'
    return LOWERCASE_LETTER | 29;
  case 0x75: // 'u'
    return LOWERCASE_LETTER | 30;
  case 0x76: // 'v'
    return LOWERCASE_LETTER | 31;
  case 0x77: // 'w'
    return LOWERCASE_LETTER | 32;
  case 0x78: // 'x'
    return LOWERCASE_LETTER | 33;
  case 0x79: // 'y'
    return LOWERCASE_LETTER | 34;
  case 0x7A: // 'z'
    return LOWERCASE_LETTER | 35;
  case 0x7B: // '{'
    return SYMBOL;
  case 0x7C: // '|'
    return SYMBOL;
  case 0x7D: // '}'
    return SYMBOL;
  case 0x7E: // '~'
    return SYMBOL;
  case 0x7F: // DEL
    return INVALID;
  case 0x80:
    return CONTINUE;
  case 0x81:
    return CONTINUE;
  case 0x82:
    return CONTINUE;
  case 0x83:
    return CONTINUE;
  case 0x84:
    return CONTINUE;
  case 0x85:
    return CONTINUE;
  case 0x86:
    return CONTINUE;
  case 0x87:
    return CONTINUE;
  case 0x88:
    return CONTINUE;
  case 0x89:
    return CONTINUE;
  case 0x8A:
    return CONTINUE;
  case 0x8B:
    return CONTINUE;
  case 0x8C:
    return CONTINUE;
  case 0x8D:
    return CONTINUE;
  case 0x8E:
    return CONTINUE;
  case 0x8F:
    return CONTINUE;
  case 0x90:
    return CONTINUE;
  case 0x91:
    return CONTINUE;
  case 0x92:
    return CONTINUE;
  case 0x93:
    return CONTINUE;
  case 0x94:
    return CONTINUE;
  case 0x95:
    return CONTINUE;
  case 0x96:
    return CONTINUE;
  case 0x97:
    return CONTINUE;
  case 0x98:
    return CONTINUE;
  case 0x99:
    return CONTINUE;
  case 0x9A:
    return CONTINUE;
  case 0x9B:
    return CONTINUE;
  case 0x9C:
    return CONTINUE;
  case 0x9D:
    return CONTINUE;
  case 0x9E:
    return CONTINUE;
  case 0x9F:
    return CONTINUE;
  case 0xA0:
    return CONTINUE;
  case 0xA1:
    return CONTINUE;
  case 0xA2:
    return CONTINUE;
  case 0xA3:
    return CONTINUE;
  case 0xA4:
    return CONTINUE;
  case 0xA5:
    return CONTINUE;
  case 0xA6:
    return CONTINUE;
  case 0xA7:
    return CONTINUE;
  case 0xA8:
    return CONTINUE;
  case 0xA9:
    return CONTINUE;
  case 0xAA:
    return CONTINUE;
  case 0xAB:
    return CONTINUE;
  case 0xAC:
    return CONTINUE;
  case 0xAD:
    return CONTINUE;
  case 0xAE:
    return CONTINUE;
  case 0xAF:
    return CONTINUE;
  case 0xB0:
    return CONTINUE;
  case 0xB1:
    return CONTINUE;
  case 0xB2:
    return CONTINUE;
  case 0xB3:
    return CONTINUE;
  case 0xB4:
    return CONTINUE;
  case 0xB5:
    return CONTINUE;
  case 0xB6:
    return CONTINUE;
  case 0xB7:
    return CONTINUE;
  case 0xB8:
    return CONTINUE;
  case 0xB9:
    return CONTINUE;
  case 0xBA:
    return CONTINUE;
  case 0xBB:
    return CONTINUE;
  case 0xBC:
    return CONTINUE;
  case 0xBD:
    return CONTINUE;
  case 0xBE:
    return CONTINUE;
  case 0xBF:
    return CONTINUE;
  case 0xC0:
    return TWO_START;
  case 0xC1:
    return TWO_START;
  case 0xC2:
    return TWO_START;
  case 0xC3:
    return TWO_START;
  case 0xC4:
    return TWO_START;
  case 0xC5:
    return TWO_START;
  case 0xC6:
    return TWO_START;
  case 0xC7:
    return TWO_START;
  case 0xC8:
    return TWO_START;
  case 0xC9:
    return TWO_START;
  case 0xCA:
    return TWO_START;
  case 0xCB:
    return TWO_START;
  case 0xCC:
    return TWO_START;
  case 0xCD:
    return TWO_START;
  case 0xCE:
    return TWO_START;
  case 0xCF:
    return TWO_START;
  case 0xD0:
    return TWO_START;
  case 0xD1:
    return TWO_START;
  case 0xD2:
    return TWO_START;
  case 0xD3:
    return TWO_START;
  case 0xD4:
    return TWO_START;
  case 0xD5:
    return TWO_START;
  case 0xD6:
    return TWO_START;
  case 0xD7:
    return TWO_START;
  case 0xD8:
    return TWO_START;
  case 0xD9:
    return TWO_START;
  case 0xDA:
    return TWO_START;
  case 0xDB:
    return TWO_START;
  case 0xDC:
    return TWO_START;
  case 0xDD:
    return TWO_START;
  case 0xDE:
    return TWO_START;
  case 0xDF:
    return TWO_START;
  case 0xE0:
    return THREE_START;
  case 0xE1:
    return THREE_START;
  case 0xE2:
    return THREE_START;
  case 0xE3:
    return THREE_START;
  case 0xE4:
    return THREE_START;
  case 0xE5:
    return THREE_START;
  case 0xE6:
    return THREE_START;
  case 0xE7:
    return THREE_START;
  case 0xE8:
    return THREE_START;
  case 0xE9:
    return THREE_START;
  case 0xEA:
    return THREE_START;
  case 0xEB:
    return THREE_START;
  case 0xEC:
    return THREE_START;
  case 0xED:
    return THREE_START;
  case 0xEE:
    return THREE_START;
  case 0xEF:
    return THREE_START;
  case 0xF0:
    return FOUR_START;
  case 0xF1:
    return FOUR_START;
  case 0xF2:
    return FOUR_START;
  case 0xF3:
    return FOUR_START;
  case 0xF4:
    return FOUR_START;
  case 0xF5:
    return FOUR_START;
  case 0xF6:
    return FOUR_START;
  case 0xF7:
    return FOUR_START;
  case 0xF8:
    return INVALID;
  case 0xF9:
    return INVALID;
  case 0xFA:
    return INVALID;
  case 0xFB:
    return INVALID;
  case 0xFC:
    return INVALID;
  case 0xFD:
    return INVALID;
  case 0xFE:
    return INVALID;
  case 0xFF:
    return INVALID;
  }
  return CF::NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr llvm::StringRef
getCsvQuotedValueText(char codeunit) {
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
    return "\"\"";
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
  return "\\ERROR\\";
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsWhitespace(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasSome(flags, rq::CodeunitFlags::HORIZONTAL_WHITESPACE |
                                   rq::CodeunitFlags::VERTICAL_WHITESPACE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsHorizontalWhitespace(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasAll(flags, rq::CodeunitFlags::HORIZONTAL_WHITESPACE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsVerticalWhitespace(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasAll(flags, rq::CodeunitFlags::VERTICAL_WHITESPACE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsDigit(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasSome(flags, rq::CodeunitFlags::DECIMAL_DIGIT |
                                   rq::CodeunitFlags::EXTENDED_DIGIT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsDecimalDigit(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasAll(flags, rq::CodeunitFlags::DECIMAL_DIGIT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsExtendedDigit(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasAll(flags, rq::CodeunitFlags::EXTENDED_DIGIT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsNumeric(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasAll(flags, rq::CodeunitFlags::NUMERIC);
}

[[nodiscard]] RQ_ALWAYS_INLINE std::underlying_type_t<rq::CodeunitFlags>
getDigitBaseMultiplier(char codeunit) {
  RQ_ASSERT(rq::getIsDigit(codeunit), "codeunit not digit");
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getMaskValue(flags, rq::CodeunitFlags::MASK);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsLetter(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasSome(flags, rq::CodeunitFlags::LOWERCASE_LETTER |
                                   rq::CodeunitFlags::UPPERCASE_LETTER);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr char
getIsUppercaseLetter(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasAll(flags, rq::CodeunitFlags::UPPERCASE_LETTER);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr char
getIsLowercaseLetter(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasAll(flags, rq::CodeunitFlags::LOWERCASE_LETTER);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr char
getUppercaseLetter(char codeunit) {
  if (rq::getIsLowercaseLetter(codeunit)) {
    return codeunit ^ rq::CHAR_CASE_BIT;
  }
  return codeunit;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr char
getLowercaseLetter(char codeunit) {
  if (rq::getIsUppercaseLetter(codeunit)) {
    return codeunit ^ rq::CHAR_CASE_BIT;
  }
  return codeunit;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsIdentifier(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasSome(flags, rq::CodeunitFlags::IDENTIFIER_FRONT |
                                   rq::CodeunitFlags::IDENTIFIER_BODY);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsIdentifierStart(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasAll(flags, rq::CodeunitFlags::IDENTIFIER_FRONT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsIdentifierBody(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasAll(flags, rq::CodeunitFlags::IDENTIFIER_BODY);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsSymbol(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getHasAll(flags, rq::CodeunitFlags::SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsExtended(char codeunit) {
  return (codeunit & rq::CHAR_EXTENDED_BIT) == rq::CHAR_EXTENDED_BIT;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr std::underlying_type_t<rq::CodeunitFlags>
getExtendedStartCount(char codeunit) {
  RQ_ASSERT(rq::getIsExtended(codeunit), "codeunit not extended");
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return rq::getMaskValue(flags, rq::CodeunitFlags::MASK);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsValid(char codeunit) {
  const rq::CodeunitFlags flags = rq::getFlags(codeunit);
  return flags != rq::CodeunitFlags::NONE;
}

} // namespace rq

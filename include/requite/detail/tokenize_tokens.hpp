// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

template <bool CAN_HAVE_INTERPOLATION_PARAM,
          char END_QUOTE_PARAM,
          requite::TokenType TYPE_PARAM,
          requite::TokenType ERROR_UNTERMINATED_PARAM>
void Tokenizer::tokenizeQuotedLiteral() {
  this->getRanger().startSubToken();
  this->getRanger().incrementChar(1);
  this->getRanger().addColumns(1);
  while (true) {
    switch (const char sub_c0 = this->getRanger().getChar(0)) {
    case '\\':
      switch (const char sub_c1 = this->getRanger().getChar(1)) {
      case '\v':
        [[fallthrough]];
      case '\n':
        this->getRanger().incrementChar(2);
        this->getRanger().addLines(1);
        break;
      case '\r':
        switch (const char sub_c2 = this->getRanger().getChar(2)) {
        case '\n':
          this->getRanger().incrementChar(3);
          this->getRanger().addLines(1);
          break;
        default:
          this->getRanger().incrementChar(2);
          break;
        }
        this->getRanger().addLines(1);
        break;
      case 'a':
        [[fallthrough]];
      case 'b':
        [[fallthrough]];
      case 'e':
        [[fallthrough]];
      case 'f':
        [[fallthrough]];
      case 'n':
        [[fallthrough]];
      case 'r':
        [[fallthrough]];
      case 't':
        [[fallthrough]];
      case 'v':
        [[fallthrough]];
      case '\\':
        [[fallthrough]];
      case '\"':
        [[fallthrough]];
      case '\'':
        this->getRanger().incrementChar(2);
        this->getRanger().addColumns(2);
        break;
      default:
        this->getRanger().incrementChar(1);
        this->getRanger().addColumns(1);
        while (true) {
          const char escape_c = this->getRanger().getChar(0);
          if (this->getRanger().getIsDone() || escape_c == '\\' ||
              escape_c == END_QUOTE_PARAM) {
            break;
          }
          switch (escape_c) {
          case '\v':
            [[fallthrough]];
          case '\n':
            this->getRanger().incrementChar(2);
            this->getRanger().addLines(1);
            break;
          case '\r':
            switch (const char escape_c1 = this->getRanger().getChar(1)) {
            case '\n':
              this->getRanger().incrementChar(3);
              this->getRanger().addLines(1);
              break;
            default:
              this->getRanger().incrementChar(2);
              break;
            }
            this->getRanger().addLines(1);
            break;
          default:
            this->getRanger().incrementChar(1);
            this->getRanger().addColumns(1);
          }
        }
      }
      break;
    case END_QUOTE_PARAM:
      this->getRanger().incrementChar(1);
      this->getTokens().push_back(
          this->getRanger().getSubToken(TYPE_PARAM));
      return;
    case '\n':
      this->getRanger().incrementChar(1);
      this->getRanger().addLines(1);
      break;
    case '\r':
      switch (const char sub_c1 = this->getRanger().getChar(1)) {
      case '\n':
        this->getRanger().incrementChar(2);
        break;
      default:
        this->getRanger().incrementChar(1);
        break;
      }
      this->getRanger().addLines(1);
      break;
    case '\x00':
    this->getTokens().push_back(this->getRanger().getSubToken(
          ERROR_UNTERMINATED_PARAM));
      return;
    case '{':
      if constexpr (CAN_HAVE_INTERPOLATION_PARAM) {
        this->getTokens().push_back(this->getRanger().getSubToken(
            requite::TokenType::LEFT_INTERPOLATED_STRING_LITERAL));
        this->tokenizeLengthToken(requite::TokenType::LEFT_TRIP_GROUPING, 1);
        this->pushGrouping(requite::GroupingType::VALUE_INTERPOLATION);
        return;
      }
      [[fallthrough]];
    default:
      this->getRanger().incrementChar(1);
      this->getRanger().addColumns(1);
      break;
    }
  }
}

} // namespace requite
// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/codeunits.hpp>
#include <requite/context.hpp>
#include <requite/file.hpp>
#include <requite/source_ranger.hpp>
#include <requite/tokenizer.hpp>
#include <requite/unreachable.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <cstddef>

namespace requite {

bool Context::tokenizeTokens(requite::Module &module,
                             std::vector<requite::Token> &tokens) {
  requite::Tokenizer tokenizer(*this, module.getFile(), tokens);
  return tokenizer.tokenizeTokens();
}

Tokenizer::Tokenizer(requite::Context &context, requite::File &file,
                     std::vector<requite::Token> &tokens)
    : _context_ref(context), _grouping_stack(), _ranger(file.getText()),
      _tokens_ref(tokens) {}

bool Tokenizer::getIsOk() const { return this->_is_ok; }

void Tokenizer::setNotOk() { this->_is_ok = false; }

void Tokenizer::logErrorUnmatchedRightToken(const requite::Token &token) {
  if (this->getHasGrouping()) {
    const requite::Grouping &grouping = this->getTopGrouping();
    REQUITE_ASSERT(this->getTokens().size() > grouping.token_i);
    const requite::Token &left_token = this->getTokens().at(grouping.token_i);
    this->getContext().logSourceMessage(
        token, requite::LogType::ERROR,
        llvm::Twine("right grouping token of type \"") +
            requite::getName(token.getType()) +
            "\" does not match previous left grouping token");
    return;
  }
  this->getContext().logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine("right grouping token of type \"") +
          requite::getName(token.getType()) +
          "\" does not follow a left grouping token");
}

requite::Context &Tokenizer::getContext() { return this->_context_ref.get(); }

const requite::Context &Tokenizer::getContext() const {
  return this->_context_ref.get();
}

requite::SourceRanger &Tokenizer::getRanger() { return this->_ranger; }

const requite::SourceRanger &Tokenizer::getRanger() const {
  return this->_ranger;
}

std::vector<requite::Token> &Tokenizer::getTokens() {
  return this->_tokens_ref.get();
}

const std::vector<requite::Token> &Tokenizer::getTokens() const {
  return this->_tokens_ref.get();
}

bool Tokenizer::getHasGrouping() const {
  return !this->_grouping_stack.empty();
}

const requite::Grouping &Tokenizer::getTopGrouping() const {
  REQUITE_ASSERT(!this->_grouping_stack.empty());
  return this->_grouping_stack.back();
}

void Tokenizer::pushGrouping(requite::GroupingType grouping) {
  this->_grouping_stack.emplace_back(grouping, this->getTokens().size() - 1);
}

void Tokenizer::popGrouping() {
  REQUITE_ASSERT(!this->_grouping_stack.empty());
  this->_grouping_stack.pop_back();
}

void Tokenizer::_tokenizeTokens() {
  this->getTokens().clear();
  if (this->getRanger().getIsDone()) {
    return;
  }
  while (true) {
    switch (const char c0 = this->getRanger().getChar(0)) {
    case '\x00':
      return;
    case '\x01':
      REQUITE_UNREACHABLE();
    case '\x02':
      REQUITE_UNREACHABLE();
    case '\x03':
      REQUITE_UNREACHABLE();
    case '\x04':
      REQUITE_UNREACHABLE();
    case '\x05':
      REQUITE_UNREACHABLE();
    case '\x06':
      REQUITE_UNREACHABLE();
    case '\a':
      REQUITE_UNREACHABLE();
    case '\b':
      REQUITE_UNREACHABLE();
    case '\t':
      this->getRanger().incrementChar(1);
      this->getRanger().addColumns(1);
      continue;
    case '\n':
      [[fallthrough]];
    case '\v':
      this->getRanger().incrementChar(1);
      this->getRanger().addLines(1);
      continue;
    case '\x0C':
      REQUITE_UNREACHABLE();
    case '\r':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '\n':
        this->getRanger().incrementChar(2);
        continue;
      default:
        this->getRanger().incrementChar(1);
        continue;
      }
      this->getRanger().addLines(1);
      continue;
    case '\x0E':
      REQUITE_UNREACHABLE();
    case '\x0F':
      REQUITE_UNREACHABLE();
    case '\x10':
      REQUITE_UNREACHABLE();
    case '\x11':
      REQUITE_UNREACHABLE();
    case '\x12':
      REQUITE_UNREACHABLE();
    case '\x13':
      REQUITE_UNREACHABLE();
    case '\x14':
      REQUITE_UNREACHABLE();
    case '\x15':
      REQUITE_UNREACHABLE();
    case '\x16':
      REQUITE_UNREACHABLE();
    case '\x17':
      REQUITE_UNREACHABLE();
    case '\x18':
      REQUITE_UNREACHABLE();
    case '\x19':
      REQUITE_UNREACHABLE();
    case '\x1A':
      REQUITE_UNREACHABLE();
    case '\x1B':
      REQUITE_UNREACHABLE();
    case '\x1C':
      REQUITE_UNREACHABLE();
    case '\x1D':
      REQUITE_UNREACHABLE();
    case '\x1E':
      REQUITE_UNREACHABLE();
    case '\x1F':
      REQUITE_UNREACHABLE();
    case ' ':
      this->getRanger().incrementChar(1);
      this->getRanger().addColumns(1);
      continue;
    case '!':
      this->tokenizeLengthToken(requite::TokenType::BANG_OPERATOR, 1);
      continue;
    case '\"':
      this->tokenizeQuotedLiteral<
          true, '\"', requite::TokenType::STRING_LITERAL,
          requite::TokenType::ERROR_UNTERMINATED_STRING_LITERAL>();
      continue;
    case '#':
      this->tokenizeLengthToken(requite::TokenType::HASH_OPERATOR, 1);
      continue;
    case '$':
      this->tokenizeLengthToken(requite::TokenType::DOLLAR_OPERATOR, 1);
      continue;
    case '%':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '=':
        this->tokenizeLengthToken(requite::TokenType::PERCENT_EQUAL_OPERATOR,
                                  2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::PERCENT_OPERATOR, 1);
      }
      continue;
    case '&':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '&':
        this->tokenizeLengthToken(requite::TokenType::DOUBLE_AMPERSAND_OPERATOR,
                                  2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::AMBERSAND_OPERATOR, 1);
      }
      continue;
    case '\'':
      this->tokenizeQuotedLiteral<
          false, '\'', requite::TokenType::CODEUNIT_LITERAL,
          requite::TokenType::ERROR_UNTERMINATED_CODEUNIT_LITERAL>();
      continue;
    case '(':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '>':
        this->tokenizeLengthToken(requite::TokenType::LEFT_COMPAS_GROUPING, 2);
        this->pushGrouping(requite::GroupingType::COMPAS);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::LEFT_PARENTHESIS_GROUPING,
                                  1);
        this->pushGrouping(requite::GroupingType::PARENTHESIS);
      }
      continue;
    case ')':
      this->tokenizeRightGrouping(
          requite::GroupingType::PARENTHESIS,
          requite::TokenType::RIGHT_PARENTHESIS_GROUPING, 1);
      continue;
    case '*':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '=':
        this->tokenizeLengthToken(requite::TokenType::STAR_EQUAL_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::STAR_OPERATOR, 1);
      }
      continue;
    case '+':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '=':
        this->tokenizeLengthToken(requite::TokenType::PLUS_EQUAL_OPERATOR, 2);
        break;
      case '>':
        this->tokenizeLengthToken(requite::TokenType::CONCATENATE_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::PLUS_OPERATOR, 1);
      }
      continue;
    case ',':
      this->tokenizeLengthToken(requite::TokenType::COMMA_OPERATOR, 1);
      continue;
    case '-':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '=':
        this->tokenizeLengthToken(requite::TokenType::DASH_EQUAL_OPERATOR, 2);
        break;
      case '>':
        this->tokenizeLengthToken(requite::TokenType::ARROW_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::DASH_OPERATOR, 1);
        break;
      }
      continue;
    case '.':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '.':
        this->tokenizeLengthToken(requite::TokenType::DOUBLE_DOT_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::DOT_OPERATOR, 1);
      }
      continue;
    case '/':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '/':
        this->getRanger().incrementChar(2);
        this->getRanger().addColumns(2);
        {
          bool found_newline = false;
          while (!found_newline) {
            switch (const char c2 = this->getRanger().getChar(0)) {
            case '\x00':
              return;
            case '\n':
              [[fallthrough]];
            case '\v':
              this->getRanger().incrementChar(1);
              this->getRanger().addLines(1);
              found_newline = true;
              break;
            case '\r':
              switch (const char c3 = this->getRanger().getChar(1)) {
              case '\n':
                this->getRanger().incrementChar(2);
                break;
              default:
                this->getRanger().incrementChar(1);
              }
              this->getRanger().addLines(1);
              found_newline = true;
              break;
            default:
              this->getRanger().incrementChar(1);
              this->getRanger().addColumns(1);
            }
          }
        }
        break;
      case '*':
        this->getRanger().incrementChar(2);
        this->getRanger().addColumns(2);
        while (true) {
          switch (const char c2 = this->getRanger().getChar(0)) {
          case '\x00':
            return;
          case '*':
            switch (const char c3 = this->getRanger().getChar(1)) {
            case '/':
              this->getRanger().incrementChar(2);
              break;
            default:
              this->getRanger().incrementChar(1);
            }
            break;
          case '\n':
            [[fallthrough]];
          case '\v':
            this->getRanger().incrementChar(1);
            this->getRanger().addLines(1);
            break;
          case '\r':
            switch (const char c3 = this->getRanger().getChar(1)) {
            case '\n':
              this->getRanger().incrementChar(2);
              break;
            default:
              this->getRanger().incrementChar(1);
            }
            this->getRanger().addLines(1);
            break;
          default:
            this->getRanger().incrementChar(1);
            this->getRanger().addColumns(1);
          }
        }
      case '=':
        this->tokenizeLengthToken(requite::TokenType::SLASH_EQUAL_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::SLASH_OPERATOR, 1);
      }
      continue;
    case '0':
      [[fallthrough]];
    case '1':
      [[fallthrough]];
    case '2':
      [[fallthrough]];
    case '3':
      [[fallthrough]];
    case '4':
      [[fallthrough]];
    case '5':
      [[fallthrough]];
    case '6':
      [[fallthrough]];
    case '7':
      [[fallthrough]];
    case '8':
      [[fallthrough]];
    case '9': {
      this->getRanger().startSubToken();
      this->getRanger().incrementChar(1);
      this->getRanger().addColumns(1);
      bool is_real = false;
      while (this->getRanger().getIsNumeric()) {
        const char sub_c0 = this->getRanger().getChar(0);
        if (sub_c0 == '.') {
          const char sub_c1 = this->getRanger().getChar(1);
          if (requite::getIsDecimalDigit(sub_c1)) {
            is_real = true;
          } else {
            break;
          }
        }
        this->getRanger().incrementChar(1);
        this->getRanger().addColumns(1);
      }
      requite::TokenType type = (is_real) ? requite::TokenType::REAL_LITERAL
                                          : requite::TokenType::INTEGER_LITERAL;
      this->getTokens().push_back(this->getRanger().getSubToken(type));
      continue;
    }
    case ':':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case ':':
        this->tokenizeLengthToken(requite::TokenType::DOUBLE_COLON_OPERATOR, 2);
        break;
      case '=':
        this->tokenizeLengthToken(requite::TokenType::WALRUS_OPERATOR, 2);
        break;
      case '}':
        this->tokenizeRightGrouping(
            requite::GroupingType::CAP,
            requite::TokenType::RIGHT_CAP_GROUPING, 2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::COLON_OPERATOR, 1);
      }
      continue;
    case ';':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case ';':
        this->tokenizeLengthToken(requite::TokenType::DOUBLE_SEMICOLON_OPERATOR,
                                  2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::SEMICOLON_OPERATOR, 1);
      }
      continue;
    case '<':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '>':
        this->tokenizeLengthToken(requite::TokenType::SWAP_OPERATOR, 3);
        break;
      case '<':
        this->tokenizeLengthToken(requite::TokenType::DOUBLE_LESS_OPERATOR, 2);
        break;
      case ')':
        this->tokenizeRightGrouping(requite::GroupingType::COMPAS, requite::TokenType::RIGHT_COMPAS_GROUPING, 2);
        break;
      case '=':
        this->tokenizeLengthToken(requite::TokenType::LESS_EQUAL_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::LESS_OPERATOR, 1);
      }
      continue;
    case '=':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '=':
        this->tokenizeLengthToken(requite::TokenType::DOUBLE_EQUAL_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::EQUAL_OPERATOR, 1);
      }
      continue;
    case '>':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '>':
        this->tokenizeLengthToken(requite::TokenType::DOUBLE_GREATER_OPERATOR,
                                  2);
        break;
      case '^':
        this->tokenizeLengthToken(requite::TokenType::GREATER_CAROT_OPERATOR,
                                  2);
        break;
      case '=':
        this->tokenizeLengthToken(requite::TokenType::GREATER_EQUAL_OPERATOR,
                                  2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::LESS_OPERATOR, 1);
      }
      continue;
    case '?':
      this->tokenizeLengthToken(requite::TokenType::QUESTION_OPERATOR, 1);
      continue;
    case '@':
      this->tokenizeLengthToken(requite::TokenType::AT_OPERATOR, 1);
      continue;
    case 'A':
      break;
    case 'B':
      break;
    case 'C':
      break;
    case 'D':
      break;
    case 'E':
      break;
    case 'F':
      break;
    case 'G':
      break;
    case 'H':
      break;
    case 'I':
      break;
    case 'J':
      break;
    case 'K':
      break;
    case 'L':
      break;
    case 'M':
      break;
    case 'N':
      break;
    case 'O':
      break;
    case 'P':
      break;
    case 'Q':
      break;
    case 'R':
      break;
    case 'S':
      break;
    case 'T':
      break;
    case 'U':
      break;
    case 'V':
      break;
    case 'W':
      break;
    case 'X':
      break;
    case 'Y':
      break;
    case 'Z':
      break;
    case '[':
      this->tokenizeLengthToken(requite::TokenType::LEFT_BRACKET_GROUPING, 1);
      this->pushGrouping(requite::GroupingType::BRACKET);
      continue;
    case '\\':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '\\':
        this->tokenizeLengthToken(requite::TokenType::DOUBLE_BACKSLASH_OPERATOR,
                                  2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::BACKSLASH_OPERATOR, 1);
      }
      continue;
    case ']':
      this->tokenizeRightGrouping(requite::GroupingType::BRACKET,
                                  requite::TokenType::RIGHT_BRACKET_GROUPING,
                                  1);
      continue;
    case '^':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '<':
        this->tokenizeLengthToken(requite::TokenType::CAROT_LESS_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(requite::TokenType::CAROT_OPERATOR, 1);
      }
      continue;
    case '_':
      break;
    case '`':
      this->tokenizeLengthToken(requite::TokenType::GRAVE_OPERATOR, 1);
      continue;
    case 'a':
      break;
    case 'b':
      break;
    case 'c':
      break;
    case 'd':
      break;
    case 'e':
      break;
    case 'f':
      break;
    case 'g':
      break;
    case 'h':
      break;
    case 'i':
      break;
    case 'j':
      break;
    case 'k':
      break;
    case 'l':
      break;
    case 'm':
      break;
    case 'n':
      break;
    case 'o':
      break;
    case 'p':
      break;
    case 'q':
      break;
    case 'r':
      break;
    case 's':
      break;
    case 't':
      break;
    case 'u':
      break;
    case 'v':
      break;
    case 'w':
      break;
    case 'x':
      break;
    case 'y':
      break;
    case 'z':
      break;
    case '{':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case ':':
        this->tokenizeLengthToken(
            requite::TokenType::LEFT_CAP_GROUPING, 2);
        this->pushGrouping(requite::GroupingType::CAP);
        break;
      default:
        this->tokenizeLengthToken(
            requite::TokenType::LEFT_TRIP_GROUPING, 1);
        this->pushGrouping(requite::GroupingType::TRIP);
      }
      continue;
    case '|':
      switch (const char c1 = this->getRanger().getChar(1)) {
      case '|':
        this->tokenizeLengthToken(requite::TokenType::DOUBLE_PIPE_OPERATOR, 2);
        continue;
      default:
        this->tokenizeLengthToken(requite::TokenType::PIPE_OPERATOR, 1);
      }
      continue;
    case '}':
      if (!this->getHasGrouping()) {
        this->tokenizeUnmatchedLengthToken(
            requite::TokenType::RIGHT_TRIP_GROUPING, 1);
      } else if (this->getTopGrouping().type ==
                 requite::GroupingType::TRIP) {
        this->tokenizeLengthToken(
            requite::TokenType::RIGHT_TRIP_GROUPING, 1);
        this->popGrouping();
      } else if (this->getTopGrouping().type ==
                 requite::GroupingType::VALUE_INTERPOLATION) {
        this->tokenizeLengthToken(
            requite::TokenType::RIGHT_TRIP_GROUPING, 1);
        this->popGrouping();
        this->getRanger().startSubToken();
        while (true) {
          const char sub_c0 = this->getRanger().getChar(0);
          if (sub_c0 == '\\') {
            switch (const char sub_c1 = this->getRanger().getChar(1)) {
            case '\n':
              this->getRanger().incrementChar(2);
              this->getRanger().addLines(1);
              break;
            case '\r':
              switch (const char sub_c2 = this->getRanger().getChar(1)) {
              case '\n':
                this->getRanger().incrementChar(3);
                break;
              default:
                this->getRanger().incrementChar(2);
              }
              this->getRanger().addLines(1);
              break;
            default:
              this->getRanger().incrementChar(2);
              this->getRanger().addColumns(2);
            }
          } else if (sub_c0 == '{') {
            this->getTokens().push_back(this->getRanger().getSubToken(
                requite::TokenType::MIDDLE_INTERPOLATED_STRING_LITERAL));
            this->tokenizeLengthToken(
                requite::TokenType::LEFT_TRIP_GROUPING, 1);
            this->pushGrouping(requite::GroupingType::VALUE_INTERPOLATION);
            break;
          } else if (sub_c0 == '\n') {
            this->getRanger().incrementChar(1);
            this->getRanger().addLines(1);
          } else if (sub_c0 == '\r') {
            switch (const char sub_c1 = this->getRanger().getChar(1)) {
            case '\n':
              this->getRanger().incrementChar(2);
              break;
            default:
              this->getRanger().incrementChar(1);
            }
            this->getRanger().addLines(1);
          } else if (sub_c0 == '\"') {
            this->getRanger().incrementChar(1);
            this->getRanger().addColumns(1);
            this->getTokens().push_back(this->getRanger().getSubToken(
                requite::TokenType::RIGHT_INTERPOLATED_STRING_LITERAL));
            break;
          } else if (sub_c0 == '\0') {
            this->getTokens().push_back(this->getRanger().getSubToken(
                requite::TokenType::ERROR_UNTERMINATED_STRING_LITERAL));
            this->getContext().logSourceMessage(this->getTokens().back(),
                                                requite::LogType::ERROR,
                                                "unterminated string");
            this->setNotOk();
            break;
          } else {
            this->getRanger().incrementChar(1);
            this->getRanger().addColumns(1);
          }
        }
      } else {
        this->tokenizeUnmatchedLengthToken(
            requite::TokenType::RIGHT_TRIP_GROUPING, 1);
      }
      continue;
    case '~':
      this->tokenizeLengthToken(requite::TokenType::TILDE_OPERATOR, 1);
      continue;
    case '\x7F':
      break;
    case '\x80':
      break;
    case '\x81':
      break;
    case '\x82':
      break;
    case '\x83':
      break;
    case '\x84':
      break;
    case '\x85':
      break;
    case '\x86':
      break;
    case '\x87':
      break;
    case '\x88':
      break;
    case '\x89':
      break;
    case '\x8A':
      break;
    case '\x8B':
      break;
    case '\x8C':
      break;
    case '\x8D':
      break;
    case '\x8E':
      break;
    case '\x8F':
      break;
    case '\x90':
      break;
    case '\x91':
      break;
    case '\x92':
      break;
    case '\x93':
      break;
    case '\x94':
      break;
    case '\x95':
      break;
    case '\x96':
      break;
    case '\x97':
      break;
    case '\x98':
      break;
    case '\x99':
      break;
    case '\x9A':
      break;
    case '\x9B':
      break;
    case '\x9C':
      break;
    case '\x9D':
      break;
    case '\x9E':
      break;
    case '\x9F':
      break;
    case '\xA0':
      break;
    case '\xA1':
      break;
    case '\xA2':
      break;
    case '\xA3':
      break;
    case '\xA4':
      break;
    case '\xA5':
      break;
    case '\xA6':
      break;
    case '\xA7':
      break;
    case '\xA8':
      break;
    case '\xA9':
      break;
    case '\xAA':
      break;
    case '\xAB':
      break;
    case '\xAC':
      break;
    case '\xAD':
      break;
    case '\xAE':
      break;
    case '\xAF':
      break;
    case '\xB0':
      break;
    case '\xB1':
      break;
    case '\xB2':
      break;
    case '\xB3':
      break;
    case '\xB4':
      break;
    case '\xB5':
      break;
    case '\xB6':
      break;
    case '\xB7':
      break;
    case '\xB8':
      break;
    case '\xB9':
      break;
    case '\xBA':
      break;
    case '\xBB':
      break;
    case '\xBC':
      break;
    case '\xBD':
      break;
    case '\xBE':
      break;
    case '\xBF':
      break;
    case '\xC0':
      break;
    case '\xC1':
      break;
    case '\xC2':
      break;
    case '\xC3':
      break;
    case '\xC4':
      break;
    case '\xC5':
      break;
    case '\xC6':
      break;
    case '\xC7':
      break;
    case '\xC8':
      break;
    case '\xC9':
      break;
    case '\xCA':
      break;
    case '\xCB':
      break;
    case '\xCC':
      break;
    case '\xCD':
      break;
    case '\xCE':
      break;
    case '\xCF':
      break;
    case '\xD0':
      break;
    case '\xD1':
      break;
    case '\xD2':
      break;
    case '\xD3':
      break;
    case '\xD4':
      break;
    case '\xD5':
      break;
    case '\xD6':
      break;
    case '\xD7':
      break;
    case '\xD8':
      break;
    case '\xD9':
      break;
    case '\xDA':
      break;
    case '\xDB':
      break;
    case '\xDC':
      break;
    case '\xDD':
      break;
    case '\xDE':
      break;
    case '\xDF':
      break;
    case '\xE0':
      break;
    case '\xE1':
      break;
    case '\xE2':
      break;
    case '\xE3':
      break;
    case '\xE4':
      break;
    case '\xE5':
      break;
    case '\xE6':
      break;
    case '\xE7':
      break;
    case '\xE8':
      break;
    case '\xE9':
      break;
    case '\xEA':
      break;
    case '\xEB':
      break;
    case '\xEC':
      break;
    case '\xED':
      break;
    case '\xEE':
      break;
    case '\xEF':
      break;
    case '\xF0':
      break;
    case '\xF1':
      break;
    case '\xF2':
      break;
    case '\xF3':
      break;
    case '\xF4':
      break;
    case '\xF5':
      break;
    case '\xF6':
      break;
    case '\xF7':
      break;
    case '\xF8':
      REQUITE_UNREACHABLE();
    case '\xF9':
      REQUITE_UNREACHABLE();
    case '\xFA':
      REQUITE_UNREACHABLE();
    case '\xFB':
      REQUITE_UNREACHABLE();
    case '\xFC':
      REQUITE_UNREACHABLE();
    case '\xFD':
      REQUITE_UNREACHABLE();
    case '\xFE':
      REQUITE_UNREACHABLE();
    case '\xFF':
      REQUITE_UNREACHABLE();
    }
    REQUITE_ASSERT(this->getRanger().getIsIdentifier());
    this->getRanger().startSubToken();
    this->getRanger().incrementChar(1);
    this->getRanger().addColumns(1);
    while (this->getRanger().getIsIdentifier()) {
      this->getRanger().incrementChar(1);
      this->getRanger().addColumns(1);
    }
    this->getTokens().push_back(
        this->getRanger().getSubToken(requite::TokenType::IDENTIFIER_LITERAL));
  }
  REQUITE_UNREACHABLE();
}

bool Tokenizer::tokenizeTokens() {
  this->_tokenizeTokens();
  this->checkFinalGroupings();
  return this->getIsOk();
}

void Tokenizer::checkFinalGroupings() {
  if (!this->getHasGrouping()) {
    return;
  }
  this->setNotOk();
  while (this->getHasGrouping()) {
    const requite::Grouping &grouping = this->getTopGrouping();
    REQUITE_ASSERT(this->getTokens().size() > grouping.token_i);
    requite::Token &token = this->getTokens().at(grouping.token_i);
    this->getContext().logSourceMessage(
        token, requite::LogType::ERROR,
        llvm::Twine("grouping token of type \"") +
            requite::getName(token.getType()) + "\" has no right match");
    token.setUnmatched();
    this->popGrouping();
  }
}

void Tokenizer::tokenizeUnmatchedLengthToken(requite::TokenType type,
                                             unsigned length) {
  requite::Token token = this->getRanger().getLengthToken(type, length);
  this->logErrorUnmatchedRightToken(token);
  token.setUnmatched();
  this->getTokens().push_back(token);
  this->setNotOk();
}

void Tokenizer::tokenizeLengthToken(requite::TokenType type, unsigned length) {
  this->getTokens().push_back(this->getRanger().getLengthToken(type, length));
}

void Tokenizer::tokenizeRightGrouping(requite::GroupingType grouping,
                                      requite::TokenType type,
                                      unsigned length) {
  requite::Token token = this->getRanger().getLengthToken(type, length);
  if (!this->getHasGrouping() || this->getTopGrouping().type != grouping) {
    this->logErrorUnmatchedRightToken(token);
    token.setUnmatched();
    this->setNotOk();
    return;
  }
  this->getTokens().push_back(token);
  this->popGrouping();
}

} // namespace requite
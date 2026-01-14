#include <rq/context.hpp>
#include <rq/tokenize.hpp>
#include <rq/tokens.hpp>

namespace rq {

void Tokenizer::_tokenizeSourceText() {
  using namespace rq;
  using T = TokenKind;
  using G = GroupingKind;
  this->getTokens().clear();
  if (this->getRanger().getIsDone()) {
    return;
  }
  while (true) {
    switch (this->getRanger().getChar(0)) {
    case '\x00':
      return;
    case '\x01':
      RQ_UNREACHABLE();
    case '\x02':
      RQ_UNREACHABLE();
    case '\x03':
      RQ_UNREACHABLE();
    case '\x04':
      RQ_UNREACHABLE();
    case '\x05':
      RQ_UNREACHABLE();
    case '\x06':
      RQ_UNREACHABLE();
    case '\a':
      RQ_UNREACHABLE();
    case '\b':
      RQ_UNREACHABLE();
    case '\t':
      [[fallthrough]];
    case '\n':
      [[fallthrough]];
    case '\v':
      this->getRanger().incrementChar(1);
      continue;
    case '\x0C':
      RQ_UNREACHABLE();
    case '\r':
      this->getRanger().incrementChar(1);
      continue;
    case '\x0E':
      RQ_UNREACHABLE();
    case '\x0F':
      RQ_UNREACHABLE();
    case '\x10':
      RQ_UNREACHABLE();
    case '\x11':
      RQ_UNREACHABLE();
    case '\x12':
      RQ_UNREACHABLE();
    case '\x13':
      RQ_UNREACHABLE();
    case '\x14':
      RQ_UNREACHABLE();
    case '\x15':
      RQ_UNREACHABLE();
    case '\x16':
      RQ_UNREACHABLE();
    case '\x17':
      RQ_UNREACHABLE();
    case '\x18':
      RQ_UNREACHABLE();
    case '\x19':
      RQ_UNREACHABLE();
    case '\x1A':
      RQ_UNREACHABLE();
    case '\x1B':
      RQ_UNREACHABLE();
    case '\x1C':
      RQ_UNREACHABLE();
    case '\x1D':
      RQ_UNREACHABLE();
    case '\x1E':
      RQ_UNREACHABLE();
    case '\x1F':
      RQ_UNREACHABLE();
    case ' ':
      this->getRanger().incrementChar(1);
      continue;
    case '!':
      this->tokenizeLengthToken(T::BANG_OPERATOR, 1);
      continue;
    case '\"':
      this->tokenizeQuotedLiteral<true, '\"', T::STRING_LITERAL,
                                  T::ERROR_UNTERMINATED_STRING_LITERAL>();
      continue;
    case '#':
      this->tokenizeLengthToken(T::HASH_OPERATOR, 1);
      continue;
    case '$':
      this->tokenizeLengthToken(T::DOLLAR_SIGIL, 1);
      continue;
    case '%':
      switch (this->getRanger().getChar(1)) {
      case '%':
        this->tokenizeLengthToken(T::TRAILER_SEPARATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(T::PERCENT_OPERATOR, 1);
      }
      continue;
    case '&':
      this->tokenizeLengthToken(T::AMPERSAND_OPERATOR, 1);
      continue;
    case '\'':
      this->tokenizeQuotedLiteral<false, '\'', T::CODEUNIT_LITERAL,
                                  T::ERROR_UNTERMINATED_CODEUNIT_LITERAL>();
      continue;
    case '(':
      this->tokenizeLeftGrouping(G::PARENTHESIS, T::LEFT_PARENTHESIS_GROUPING,
                                 1);
      continue;
    case ')':
      this->tokenizeRightGrouping(rq::GroupingKind::PARENTHESIS,
                                  T::RIGHT_PARENTHESIS_GROUPING, 1);
      continue;
    case '*':
      this->tokenizeLengthToken(T::STAR_OPERATOR, 1);
      continue;
    case '+':
      switch (this->getRanger().getChar(1)) {
      case '>':
        this->tokenizeLengthToken(T::CONCATENATE_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(T::PLUS_OPERATOR, 1);
      }
      continue;
    case ',':
      this->tokenizeLengthToken(T::COMMA_SEPARATOR, 1);
      continue;
    case '-':
      switch (this->getRanger().getChar(1)) {
      case '>':
        this->tokenizeLengthToken(T::ARROW_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(T::DASH_OPERATOR, 1);
        break;
      }
      continue;
    case '.':
      switch (this->getRanger().getChar(1)) {
      case '.':
        this->tokenizeLengthToken(T::DOUBLE_DOT_OPERATOR, 2);
        break;
      case '+':
        this->tokenizeLengthToken(T::DOT_PLUS_OPERATOR, 2);
        break;
      case '-':
        this->tokenizeLengthToken(T::DOT_DASH_OPERATOR, 2);
        break;
      case '*':
        this->tokenizeLengthToken(T::DOT_STAR_OPERATOR, 2);
        break;
      case '/':
        this->tokenizeLengthToken(T::DOT_SLASH_OPERATOR, 2);
        break;
      case '%':
        this->tokenizeLengthToken(T::DOT_PERCENT_OPERATOR, 2);
        break;
      case '<':
        switch (this->getRanger().getChar(2)) {
        case '=':
          switch (this->getRanger().getChar(3)) {
          case '+':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_LESS_EQUAL_PLUS_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          case '-':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_LESS_EQUAL_DASH_DOT_OPERATOR, 5);
              continue;
            default:
              break;
              ;
            }
            break;
            ;
          case '*':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_LESS_EQUAL_STAR_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          case '/':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_LESS_EQUAL_SLASH_DOT_OPERATOR,
                                        5);
              continue;
            default:
              break;
            }
            break;
            ;
          case '%':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_LESS_EQUAL_PERCENT_DOT_OPERATOR,
                                        5);
              continue;
            default:
              break;
            }
            break;
          default:
            break;
          }
          this->tokenizeLengthToken(T::DOT_LESS_EQUAL_OPERATOR, 3);
          continue;
        case '+':
          switch (this->getRanger().getChar(3)) {
          case '.':
            this->tokenizeLengthToken(T::DOT_LESS_PLUS_DOT_OPERATOR, 4);
            break;
          default:
            this->tokenizeLengthToken(T::DOT_LESS_OPERATOR, 2);
          }
          continue;
        case '-':
          switch (this->getRanger().getChar(3)) {
          case '.':
            this->tokenizeLengthToken(T::DOT_LESS_DASH_DOT_OPERATOR, 4);
            break;
          default:
            this->tokenizeLengthToken(T::DOT_LESS_OPERATOR, 2);
          }
          continue;
        case '*':
          switch (this->getRanger().getChar(3)) {
          case '.':
            this->tokenizeLengthToken(T::DOT_LESS_STAR_DOT_OPERATOR, 4);
            break;
          default:
            this->tokenizeLengthToken(T::DOT_LESS_OPERATOR, 2);
          }
          continue;
        case '/':
          switch (this->getRanger().getChar(3)) {
          case '.':
            this->tokenizeLengthToken(T::DOT_LESS_SLASH_DOT_OPERATOR, 4);
            break;
          default:
            this->tokenizeLengthToken(T::DOT_LESS_OPERATOR, 2);
          }
          continue;
        case '%':
          switch (this->getRanger().getChar(3)) {
          case '.':
            this->tokenizeLengthToken(T::DOT_LESS_PERCENT_DOT_OPERATOR, 4);
            break;
          default:
            this->tokenizeLengthToken(T::DOT_LESS_OPERATOR, 2);
          }
          continue;
        default:
          this->tokenizeLengthToken(T::DOT_LESS_OPERATOR, 2);
        }
        continue;
      case '>':
        switch (this->getRanger().getChar(2)) {
        case '=':
          switch (this->getRanger().getChar(3)) {
          case '+':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_GREATER_EQUAL_PLUS_DOT_OPERATOR,
                                        5);
              continue;
            default:
              break;
            }
            break;
          case '-':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_GREATER_EQUAL_DASH_DOT_OPERATOR,
                                        5);
              continue;
            default:
              break;
            }
            break;
          case '*':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_GREATER_EQUAL_STAR_DOT_OPERATOR,
                                        5);
              continue;
            default:
              break;
            }
            break;
          case '/':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_GREATER_EQUAL_SLASH_DOT_OPERATOR,
                                        5);
              continue;
            default:
              break;
            }
            break;
          case '%':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(
                  T::DOT_GREATER_EQUAL_PERCENT_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          default:
            break;
          }
          break;
        case '+':
          switch (this->getRanger().getChar(3)) {
          case '.':
            this->tokenizeLengthToken(T::DOT_GREATER_PLUS_DOT_OPERATOR, 4);
            continue;
          default:
            break;
          }
          break;
        case '-':
          switch (this->getRanger().getChar(3)) {
          case '.':
            this->tokenizeLengthToken(T::DOT_GREATER_DASH_DOT_OPERATOR, 4);
            continue;
          default:
            break;
          }
          break;
        case '*':
          switch (this->getRanger().getChar(3)) {
          case '.':
            this->tokenizeLengthToken(T::DOT_GREATER_STAR_DOT_OPERATOR, 4);
            continue;
          default:
            break;
          }
          break;
        case '/':
          switch (this->getRanger().getChar(3)) {
          case '.':
            this->tokenizeLengthToken(T::DOT_GREATER_SLASH_DOT_OPERATOR, 4);
            continue;
          default:
            break;
          }
          break;
        case '%':
          switch (this->getRanger().getChar(3)) {
          case '.':
            this->tokenizeLengthToken(T::DOT_GREATER_PERCENT_DOT_OPERATOR, 4);
            continue;
          default:
            break;
          }
          break;
        default:
          break;
        }
        this->tokenizeLengthToken(T::DOT_GREATER_OPERATOR, 2);
        continue;
      case '=':
        switch (this->getRanger().getChar(2)) {
        case '=':
          switch (this->getRanger().getChar(3)) {
          case '+':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_EQUAL_PLUS_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          case '-':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_EQUAL_DASH_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          case '*':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_EQUAL_STAR_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          case '/':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_EQUAL_SLASH_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          case '%':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_EQUAL_PERCENT_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          default:
            break;
          }
          this->tokenizeLengthToken(T::DOT_DOUBLE_EQUAL_OPERATOR, 3);
          continue;
        default:
          this->tokenizeLengthToken(T::DOT_OPERATOR, 1);
        }
        continue;
      case '!':
        switch (this->getRanger().getChar(2)) {
        case '=':
          switch (this->getRanger().getChar(3)) {
          case '+':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_BANG_EQUAL_PLUS_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          case '-':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_BANG_EQUAL_DASH_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          case '*':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_BANG_EQUAL_STAR_DOT_OPERATOR, 5);
              continue;
            default:
              break;
            }
            break;
          case '/':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_BANG_EQUAL_SLASH_DOT_OPERATOR,
                                        5);
              continue;
            default:
              break;
            }
            break;
          case '%':
            switch (this->getRanger().getChar(4)) {
            case '.':
              this->tokenizeLengthToken(T::DOT_BANG_EQUAL_PERCENT_DOT_OPERATOR,
                                        5);
              continue;
            default:
              break;
            }
            break;
          default:
            break;
          }
          this->tokenizeLengthToken(T::DOT_BANG_EQUAL_OPERATOR, 3);
          continue;
        default:
          this->tokenizeLengthToken(T::DOT_OPERATOR, 1);
        }
        continue;
      default:
        this->tokenizeLengthToken(T::DOT_OPERATOR, 1);
      }
      continue;
    case '/':
      switch (this->getRanger().getChar(1)) {
      case '/':
        this->getRanger().incrementChar(2);
        {
          bool found_newline = false;
          while (!found_newline) {
            switch (this->getRanger().getChar(0)) {
            case '\x00':
              return;
            case '\n':
              [[fallthrough]];
            case '\v':
              this->getRanger().incrementChar(1);
              found_newline = true;
              break;
            case '\r':
              switch (this->getRanger().getChar(1)) {
              case '\n':
                this->getRanger().incrementChar(2);
                break;
              default:
                this->getRanger().incrementChar(1);
              }
              found_newline = true;
              break;
            default:
              this->getRanger().incrementChar(1);
            }
          }
        }
        break;
      case '*':
        this->getRanger().incrementChar(2);
        while (true) {
          switch (this->getRanger().getChar(0)) {
          case '\x00':
            return;
          case '*':
            switch (this->getRanger().getChar(1)) {
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
            break;
          case '\r':
            switch (this->getRanger().getChar(1)) {
            case '\n':
              this->getRanger().incrementChar(2);
              break;
            default:
              this->getRanger().incrementChar(1);
            }
            break;
          default:
            this->getRanger().incrementChar(1);
          }
        }
      case '\\':
        this->tokenizeLengthToken(T::UP_ARROW_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(T::SLASH_OPERATOR, 1);
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
      bool is_float = false;
      while (this->getRanger().getIsNumeric()) {
        const char sub_c0 = this->getRanger().getChar(0);
        if (sub_c0 == '.') {
          const char sub_c1 = this->getRanger().getChar(1);
          if (getIsDecimalDigit(sub_c1)) {
            is_float = true;
          } else {
            break;
          }
        }
        this->getRanger().incrementChar(1);
      }
      T type = (is_float) ? T::FLOAT_LITERAL : T::INTEGER_LITERAL;
      this->getTokens().push_back(this->getRanger().getSubToken(type));
      continue;
    }
    case ':':
      switch (this->getRanger().getChar(1)) {
      case ':':
        this->tokenizeLengthToken(T::DOUBLE_COLON_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(T::COLON_OPERATOR, 1);
      }
      continue;
    case ';':
      this->tokenizeLengthToken(T::SEMICOLON_SEPARATOR, 1);
      continue;
    case '<':
      switch (this->getRanger().getChar(1)) {
      case '<':
        this->tokenizeLengthToken(T::DOUBLE_LESS_OPERATOR, 2);
        break;
      case '=':
        this->tokenizeLengthToken(T::LESS_EQUAL_OPERATOR, 2);
        break;
      case ':':
        switch (this->getRanger().getChar(2)) {
        case '>':
          this->tokenizeLengthToken(T::TRAILER_SEPARATOR, 3);
          break;
        default:
          break;
        }
        break;
      default:
        this->tokenizeLengthToken(T::LESS_OPERATOR, 1);
      }
      continue;
    case '=':
      switch (this->getRanger().getChar(1)) {
      case '=':
        this->tokenizeLengthToken(T::DOUBLE_EQUAL_OPERATOR, 2);
        break;
      case '>':
        this->tokenizeLengthToken(T::THICK_ARROW_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(T::EQUAL_OPERATOR, 1);
      }
      continue;
    case '>':
      switch (this->getRanger().getChar(1)) {
      case '>':
        this->tokenizeLengthToken(T::DOUBLE_GREATER_OPERATOR, 2);
        break;
      case '=':
        this->tokenizeLengthToken(T::GREATER_EQUAL_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(T::GREATER_OPERATOR, 1);
      }
      continue;
    case '?':
      this->tokenizeLengthToken(T::WHAT_OPERATOR, 1);
      continue;
    case '@':
      this->tokenizeLengthToken(T::AT_SIGIL, 1);
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
      switch (this->getRanger().getChar(1)) {
      case '[':
        this->tokenizeLeftGrouping(G::DOUBLE_BRACKET,
                                   T::LEFT_DOUBLE_BRACKET_GROUPING, 2);
        break;
      default:
        this->tokenizeLeftGrouping(G::BRACKET, T::LEFT_BRACKET_GROUPING, 1);
      }
      continue;
    case '\\':
      switch (this->getRanger().getChar(1)) {
      case '/':
        this->tokenizeLengthToken(T::DOWN_ARROW_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(T::BACKSLASH_OPERATOR, 1);
      }
      continue;
    case ']':
      switch (this->getRanger().getChar(1)) {
      case ']':
        if (!this->getHasGrouping() ||
            this->getTopGrouping().getKind() == G::DOUBLE_BRACKET) {
          this->tokenizeRightGrouping(G::DOUBLE_BRACKET,
                                      T::RIGHT_DOUBLE_BRACKET_GROUPING, 2);
          continue;
        }
        break;
      default:
        break;
      }
      this->tokenizeRightGrouping(G::BRACKET, T::RIGHT_BRACKET_GROUPING, 1);
      continue;
    case '^':
      this->tokenizeLengthToken(T::CAROT_OPERATOR, 1);
      continue;
    case '_':
      break;
    case '`':
      switch (this->getRanger().getChar(1)) {
      case '`':
        this->tokenizeLengthToken(T::DOUBLE_GRAVE_OPERATOR, 2);
        break;
      default:
        this->tokenizeLengthToken(T::GRAVE_OPERATOR, 1);
      }
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
      this->tokenizeLeftGrouping(G::BRACE, T::LEFT_BRACE_GROUPING, 1);
      continue;
    case '|':
      this->tokenizeLengthToken(rq::TokenKind::PIPE_OPERATOR, 1);
      continue;
    case '}':
      if (!this->getHasGrouping()) {
        this->tokenizeUnmatchedLengthToken(T::RIGHT_BRACE_GROUPING, 1);
      } else if (this->getTopGrouping().getKind() == G::BRACE) {
        this->tokenizeLengthToken(T::RIGHT_BRACE_GROUPING, 1);
        this->popGrouping();
      } else if (this->getTopGrouping().getKind() == G::INTERPOLATION) {
        this->tokenizeRightGrouping(G::INTERPOLATION, T::RIGHT_BRACE_GROUPING,
                                    1);
        this->getRanger().startSubToken();
        while (true) {
          const char sub_c0 = this->getRanger().getChar(0);
          if (sub_c0 == '\\') {
            switch (this->getRanger().getChar(1)) {
            case '\n':
              this->getRanger().incrementChar(2);
              break;
            case '\r':
              switch (this->getRanger().getChar(1)) {
              case '\n':
                this->getRanger().incrementChar(3);
                break;
              default:
                this->getRanger().incrementChar(2);
              }
              break;
            default:
              this->getRanger().incrementChar(2);
            }
          } else if (sub_c0 == '{') {
            this->getTokens().push_back(
                this->getRanger().getSubToken(T::MIDDLE_INTERPOLATION_LITERAL));
            this->tokenizeLeftGrouping(G::INTERPOLATION, T::LEFT_BRACE_GROUPING,
                                       1);
            break;
          } else if (sub_c0 == '\n') {
            this->getRanger().incrementChar(1);
          } else if (sub_c0 == '\r') {
            switch (this->getRanger().getChar(1)) {
            case '\n':
              this->getRanger().incrementChar(2);
              break;
            default:
              this->getRanger().incrementChar(1);
            }
          } else if (sub_c0 == '\"') {
            this->getRanger().incrementChar(1);
            this->getTokens().push_back(
                this->getRanger().getSubToken(T::RIGHT_INTERPOLATION_LITERAL));
            break;
          } else if (sub_c0 == '\0') {
            this->getTokens().push_back(this->getRanger().getSubToken(
                T::ERROR_UNTERMINATED_STRING_LITERAL));
            this->getContext().logErrorUnterminatedStringLiteral(
                this->getTokens().back());
            this->setNotOk();
            break;
          } else {
            this->getRanger().incrementChar(1);
          }
        }
      } else {
        this->tokenizeUnmatchedLengthToken(T::RIGHT_BRACE_GROUPING, 1);
      }
      continue;
    case '~':
      this->tokenizeLengthToken(T::TILDE_OPERATOR, 1);
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
      RQ_UNREACHABLE();
    case '\xF9':
      RQ_UNREACHABLE();
    case '\xFA':
      RQ_UNREACHABLE();
    case '\xFB':
      RQ_UNREACHABLE();
    case '\xFC':
      RQ_UNREACHABLE();
    case '\xFD':
      RQ_UNREACHABLE();
    case '\xFE':
      RQ_UNREACHABLE();
    case '\xFF':
      RQ_UNREACHABLE();
    }
    RQ_ASSERT(this->getRanger().getIsIdentifier(), "codeunit not identifier");
    this->getRanger().startSubToken();
    this->getRanger().incrementChar(1);
    while (this->getRanger().getIsIdentifier()) {
      this->getRanger().incrementChar(1);
    }
    this->getTokens().push_back(
        this->getRanger().getSubToken(T::IDENTIFIER_LITERAL));
  }
  RQ_UNREACHABLE();
}

bool Tokenizer::tokenizeSourceText() {
  this->_tokenizeSourceText();
  this->checkFinalGroupings();
  return this->getIsOk();
}

void Tokenizer::checkFinalGroupings() {
  if (!this->getHasGrouping()) {
    return;
  }
  this->setNotOk();
  while (this->getHasGrouping()) {
    const rq::Grouping &grouping = this->getTopGrouping();
    RQ_ASSERT(this->getTokens().size() > grouping.getTokenI(),
              "token out of range");
    rq::Token &token = this->getTokens().at(grouping.getTokenI());
    this->getContext().logErrorUnmatchedLeftToken(token);
    token.setUnmatched();
    this->popGrouping();
  }
}

void Tokenizer::tokenizeUnmatchedLengthToken(rq::TokenKind type,
                                             unsigned length) {
  rq::Token token = this->getRanger().getLengthToken(type, length);
  this->getContext().logErrorSoloRightToken(token);
  token.setUnmatched();
  this->getTokens().push_back(token);
  this->setNotOk();
}

void Tokenizer::tokenizeLengthToken(rq::TokenKind type, unsigned length) {
  this->getTokens().push_back(this->getRanger().getLengthToken(type, length));
}

void Tokenizer::tokenizeLeftGrouping(rq::GroupingKind grouping_kind,
                                     rq::TokenKind token_kind,
                                     unsigned length) {
  this->tokenizeLengthToken(token_kind, length);
  this->pushGrouping(grouping_kind);
}

void Tokenizer::tokenizeRightGrouping(rq::GroupingKind grouping_kind,
                                      rq::TokenKind token_kind,
                                      unsigned length) {
  rq::Token right_token = this->getRanger().getLengthToken(token_kind, length);
  this->getTokens().push_back(right_token);
  if (this->getHasGrouping()) {
    const rq::Grouping &top_grouping = this->getTopGrouping();
    if (top_grouping.getKind() != grouping_kind) {
      const rq::Token &left_token = this->getLeftToken(top_grouping);
      this->getContext().logErrorUnmatchedRightToken(left_token, right_token);
      right_token.setUnmatched();
      this->setNotOk();
      return;
    }
  }
  this->popGrouping();
}

} // namespace rq

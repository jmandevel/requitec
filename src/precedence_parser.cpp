#include <requite/precedence_parser.hpp>

namespace requite {

PrecedenceParser::PrecedenceParser(requite::Parser &parser)
    : _parser_ref(parser) {}

requite::Parser &PrecedenceParser::getParser() {
  return this->_parser_ref.get();
}

const requite::Parser &PrecedenceParser::getParser() const {
  return this->_parser_ref.get();
}

} // namespace requite
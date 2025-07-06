#include <requite/parser.hpp>

namespace requite {

Parser::Parser(requite::Context &context, requite::Module &module,
               std::vector<requite::Token> &tokens)
    : _context_ref(context), _module_ref(module), _is_ok(true),
      _it(tokens.cbegin()), _end(tokens.cend()) {}

requite::Module &Parser::getModule() { return this->_module_ref.get(); }

const requite::Module &Parser::getModule() const {
  return this->_module_ref.get();
}

requite::Context &Parser::getContext() { return this->_context_ref.get(); }

const requite::Context &Parser::getContext() const {
  return this->_context_ref.get();
}

bool Parser::getIsOk() { return this->_is_ok; }

void Parser::setNotOk() { this->_is_ok = false; }

} // namespace requite
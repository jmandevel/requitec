#include <requite/tokenizer.hpp>
#include <requite/file.hpp>

namespace requite {

Tokenizer::Tokenizer(requite::Context &context, requite::File &file,
                     std::vector<requite::Token> &tokens)
    : _context_ref(context), _grouping_stack(), _ranger(file.getText()),
      _tokens_ref(tokens) {}

bool Tokenizer::getIsOk() const { return this->_is_ok; }

void Tokenizer::setNotOk() { this->_is_ok = false; }

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

} // namespace requite
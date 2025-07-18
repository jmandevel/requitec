#include <requite/context.hpp>
#include <requite/source_location.hpp>
#include <requite/source_range.hpp>

namespace requite {

requite::SourceLocation Context::getSourceLocation(llvm::SMLoc loc) const {
  unsigned buffer_i = this->_source_mgr.FindBufferContainingLoc(loc);
  auto line_and_column = this->_source_mgr.getLineAndColumn(loc, buffer_i);
  requite::SourceLocation source_location = {};
  source_location.file =
      this->_source_mgr.getMemoryBuffer(buffer_i)->getBufferIdentifier();
  source_location.line = line_and_column.first;
  source_location.column = line_and_column.second;
  return source_location;
}

requite::SourceLocation
Context::getSourceStartLocation(const requite::Expression &expression) const {
  return this->getSourceLocation(expression.getSourceStartLlvmLocation());
}

requite::SourceLocation
Context::getSourceEndLocation(const requite::Expression &expression) const {
  return this->getSourceLocation(expression.getSourceEndLlvmLocation());
}

requite::SourceRange
Context::getSourceRange(const requite::Expression &expression) const {
  requite::SourceRange source_range = {};
  source_range.start =
      this->getSourceLocation(expression.getSourceStartLlvmLocation());
  source_range.end = source_range.start;
  llvm::StringRef source_text = expression.getSourceText();
  bool had_carriage_return = false;
  for (const char &cur_c : source_text) {
    if (had_carriage_return) {
      had_carriage_return = false;
      switch (cur_c) {
      case '\n':
        continue;
      default:
        break;
      }
    }
    switch (cur_c) {
    case '\r':
      had_carriage_return = true;
      [[fallthrough]];
    case '\v':
      [[fallthrough]];
    case '\n':
      source_range.end.column = 0;
      source_range.end.line++;
      break;
    default:
      source_range.end.column++;
    }
  }
  return source_range;
}

} // namespace requite
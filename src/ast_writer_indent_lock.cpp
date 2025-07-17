#include <requite/ast_writer.hpp>

namespace requite {

AstWriterIndentLock::AstWriterIndentLock(requite::AstWriter &writer)
    : _writer(writer) {
  _writer.get().addIndentation();
}

AstWriterIndentLock::~AstWriterIndentLock() {
  _writer.get().removeIndentation();
}

} // namespace requite
#include <requite/ast_writer.hpp>

namespace requite {

AstWriter::AstWriter(requite::Context &context)
    : _buffer(), _ostream(_buffer), _context_ref(context), _indentation(0) {}

requite::Context &AstWriter::getContext() { return _context_ref.get(); }

const requite::Context &AstWriter::getContext() const {
  return _context_ref.get();
}

llvm::raw_string_ostream &AstWriter::getOstream() { return _ostream; }

}
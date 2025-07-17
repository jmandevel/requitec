#include <requite/user_symbol_writer.hpp>

namespace requite {

UserSymbolWriter::UserSymbolWriter(requite::Context &context)
    : _buffer(), _ostream(_buffer), _context_ref(context), _indentation(0) {}

requite::Context &UserSymbolWriter::getContext() { return _context_ref.get(); }

const requite::Context &UserSymbolWriter::getContext() const {
  return _context_ref.get();
}

llvm::raw_string_ostream &UserSymbolWriter::getOstream() { return _ostream; }

}
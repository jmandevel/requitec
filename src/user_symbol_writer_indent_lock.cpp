#include <requite/user_symbol_writer.hpp>

namespace requite {

UserSymbolWriterIndentLock::UserSymbolWriterIndentLock(
    requite::UserSymbolWriter &writer)
    : _writer(writer) {
  _writer.get().addIndentation();
}

UserSymbolWriterIndentLock::~UserSymbolWriterIndentLock() {
  _writer.get().removeIndentation();
}

} // namespace requite
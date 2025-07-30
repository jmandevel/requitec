#include <requite/context.hpp>
#include <requite/literal_text.hpp>
#include <requite/module.hpp>
#include <requite/user_symbol_writer.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/FormatVariadic.h>
#include <llvm/Support/raw_ostream.h>

namespace requite {

bool Context::writeUserSymbols(llvm::StringRef output_path) {
  requite::UserSymbolWriter writer(*this);
  return writer.writeUserSymbols(output_path);
}

bool UserSymbolWriter::writeUserSymbols(llvm::StringRef out_path) {
  this->_buffer.clear();
  const requite::Scope &outer_scope = this->getContext().getOuterScope();
  this->writeScope(outer_scope);
  std::error_code ec;
  llvm::raw_fd_ostream fout(out_path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->getContext().logMessage(
        llvm::Twine("error: failed to open output file for writing\n\tpath: ") +
        llvm::Twine(out_path) + llvm::Twine("\n\treason: ") +
        llvm::Twine(ec.message()) + "\n");
    return false;
  }
  fout << this->_buffer;
  return true;
}

void UserSymbolWriter::writeStringProperty(llvm::StringRef name,
                                           llvm::StringRef value) {
  this->writeTextProperty(name, requite::getLiteralValue(value));
}

void UserSymbolWriter::writeTextProperty(llvm::StringRef name,
                                         llvm::StringRef value) {
  this->writeNewLine();
  this->getOstream() << name;
  this->getOstream() << ":";
  this->getOstream() << value;
}

void UserSymbolWriter::writeBoolProperty(llvm::StringRef name, bool value) {
  this->writeNewLine();
  this->getOstream() << name;
  this->getOstream() << ":";
  if (value) {
    this->getOstream() << "true";
  } else {
    this->getOstream() << "false";
  }
}

void UserSymbolWriter::writeEntry(llvm::StringRef name,
                                  const requite::LookupTableEntry &entry) {
  this->writeStringProperty("name", name);
  if (entry.getHasTable()) {
    this->writeNewLine();
    this->getOstream() << "table:";
    this->writeScope(entry.getTable().getScope());
  }
  if (entry.getUserSymbols().empty()) {
    return;
  }
  this->writeNewLine();
  this->getOstream() << "user_symbols:{";
  {
    requite::UserSymbolWriterIndentLock lock0(*this);
    for (const requite::UserSymbol &user : entry.getUserSymbols()) {
      this->writeNewLine();
      this->getOstream() << "{";
      {
        requite::UserSymbolWriterIndentLock lock1(*this);
        this->writeUserSymbol(user);
      }
      this->writeNewLine();
      this->getOstream() << "}";
    }
  }
  this->writeNewLine();
  this->getOstream() << "}";
}

void UserSymbolWriter::writeUserSymbol(const requite::UserSymbol &user) {
  this->writeStringProperty("type", requite::getName(user.getType()));
  this->writeStringProperty("module", user.getContainingModule().getName());
  this->writeBoolProperty("is_exported", user.getIsExported());
  switch (const requite::UserSymbolType type = user.getType()) {
  case requite::UserSymbolType::OBJECT:
    this->writeNewLine();
    this->getOstream() << "scope:";
    this->writeScope(user.getObject().getScope());
    break;
  case requite::UserSymbolType::PROCEDURE:
    this->writeNewLine();
    this->getOstream() << "scope:";
    this->writeScope(user.getProcedure().getScope());
    break;
  default:
    break;
  }
}

void UserSymbolWriter::writeScope(const requite::Scope &scope) {
  this->getOstream() << "{";
  if (scope.getIsEmpty()) {
    this->getOstream() << "}";
    return;
  }
  {
    requite::UserSymbolWriterIndentLock lock0(*this);
    for (const llvm::StringMapEntry<requite::LookupTableEntry> &entry :
         scope.getLookupTable()) {
      this->writeNewLine();
      this->getOstream() << "{";
      {
        requite::UserSymbolWriterIndentLock lock1(*this);
        this->writeEntry(entry.first(), entry.second);
      }
      this->writeNewLine();
      this->getOstream() << "}";
    }
  }
  this->writeNewLine();
  this->getOstream() << "}";
}

void UserSymbolWriter::addIndentation() { this->_indentation++; }

void UserSymbolWriter::removeIndentation() { this->_indentation--; }

void UserSymbolWriter::writeNewLine() {
  this->getOstream() << "\n";
  for (unsigned i = 0; i < this->_indentation; i++) {
    this->getOstream() << "    ";
  }
}

} // namespace requite
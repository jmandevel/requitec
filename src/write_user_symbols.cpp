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
  return writer.writeUserSymbols(this->getSourceModule(), output_path);
}

bool UserSymbolWriter::writeUserSymbols(const requite::Module &module,
                                        llvm::StringRef out_path) {
  this->_buffer.clear();
  const requite::Scope &module_scope = module.getScope();
  this->getOstream() << "module:{";
  {
    requite::UserSymbolWriterIndentLock lock0(*this);
    this->writeStringProperty("name", module.getName());
    this->writeScope(module.getScope());
  }
  this->writeNewLine();
  this->getOstream() << "}";
  std::error_code ec;
  llvm::raw_fd_ostream fout(out_path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->getContext().logMessage(
        llvm::Twine("error: failed to open output file for writing\n\tpath: ") +
        llvm::Twine(out_path) + llvm::Twine("\n\treason: ") +
        llvm::Twine(ec.message()));
    return false;
  }
  fout << this->_buffer;
  return true;
}

void UserSymbolWriter::writeStringProperty(llvm::StringRef name,
                                           llvm::StringRef value) {
  this->writeTextProperty(name,
                          requite::getLiteralValue(value));
}

void UserSymbolWriter::writeTextProperty(llvm::StringRef name,
                                         llvm::StringRef value) {
  this->writeNewLine();
  this->getOstream() << name;
  this->getOstream() << ":";
  this->getOstream() << value;
}

void UserSymbolWriter::writeEntry(llvm::StringRef name,
                                  const requite::LookupTableEntry &entry) {
  this->writeStringProperty("name", name);
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
    }
    this->writeNewLine();
    this->getOstream() << "}";
  }
  this->writeNewLine();
  this->getOstream() << "}";
}

void UserSymbolWriter::writeUserSymbol(const requite::UserSymbol &user) {
  this->writeTextProperty("origin", requite::getName(user.getOrigin()));
  this->writeNewLine();
  this->getOstream() << "root:{";
  {
    requite::UserSymbolWriterIndentLock lock0(*this);
    this->writeRootSymbol(user.getRoot());
  }
  this->writeNewLine();
  this->getOstream() << "}";
}

void UserSymbolWriter::writeRootSymbol(const requite::RootSymbol &root) {
  const requite::RootSymbolType type = root.getType();
  this->writeTextProperty("type", requite::getName(type));
  if (type == requite::RootSymbolType::PROCEDURE) {
    const requite::Procedure &procedure = root.getProcedure();
    this->writeTextProperty("procedure_type",
                            requite::getName(procedure.getType()));
  }
  switch (type) {
  case requite::RootSymbolType::OBJECT:
    this->writeScope(root.getObject().getScope());
    break;
  case requite::RootSymbolType::TABLE:
    this->writeScope(root.getTable().getScope());
    break;
  case requite::RootSymbolType::PROCEDURE:
    this->writeScope(root.getProcedure().getScope());
    break;
  default:
    break;
  }
}

void UserSymbolWriter::writeScope(const requite::Scope &scope) {
  this->writeNewLine();
  this->getOstream() << "scope:{";
  {
    requite::UserSymbolWriterIndentLock lock1(*this);
    for (const llvm::StringMapEntry<requite::LookupTableEntry> &entry :
         scope.getLookupTable()) {
      this->writeNewLine();
      this->getOstream() << "{";
      {
        requite::UserSymbolWriterIndentLock lock2(*this);
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
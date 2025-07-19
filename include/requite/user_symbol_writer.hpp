#pragma once

#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>

#include <functional>
#include <string>

namespace requite {

struct Module;
struct Context;
struct LookupTableEntry;
struct UserSymbol;
struct RootSymbol;
struct Scope;

struct UserSymbolWriter final {
  using Self = requite::UserSymbolWriter;

  std::reference_wrapper<requite::Context> _context_ref;
  std::string _buffer = {};
  llvm::raw_string_ostream _ostream;
  unsigned _indentation = 0;

  // user_symbol_writer.cpp
  UserSymbolWriter(requite::Context &constext);
  UserSymbolWriter(const Self &) = delete;
  UserSymbolWriter(Self &&) = delete;
  ~UserSymbolWriter() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Context &getContext();
  [[nodiscard]] const requite::Context &getContext() const;
  [[nodiscard]] llvm::raw_string_ostream &getOstream();

  // write_user_symbols.cpp
  [[nodiscard]] bool writeUserSymbols(llvm::StringRef out_path);
  void writeStringProperty(llvm::StringRef name, llvm::StringRef value);
  void writeTextProperty(llvm::StringRef name, llvm::StringRef value);
  void writeEntry(llvm::StringRef name, const requite::LookupTableEntry& entry);
  void writeUserSymbol(const requite::UserSymbol &user);
  void writeRootSymbol(const requite::RootSymbol &root);
  void writeScope(const requite::Scope& scope);
  void writeNewLine();
  void addIndentation();
  void removeIndentation();
};

struct UserSymbolWriterIndentLock final {
  using Self = requite::UserSymbolWriterIndentLock;

  std::reference_wrapper<requite::UserSymbolWriter> _writer;

  // user_symbol_writer_indent_lock.cpp
  UserSymbolWriterIndentLock(requite::UserSymbolWriter &writer);
  UserSymbolWriterIndentLock(const Self &) = delete;
  UserSymbolWriterIndentLock(Self &&) = delete;
  ~UserSymbolWriterIndentLock();
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

} // namespace requite
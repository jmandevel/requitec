#pragma once

#include <llvm/Support/raw_ostream.h>

#include <cstdint>
#include <string_view>
#include <concepts>

namespace rq {

struct JsonEmitter {
  using Self = rq::JsonEmitter;

  llvm::raw_ostream &_out;
  unsigned _indent;
  bool _needs_comma;

  explicit JsonEmitter(llvm::raw_ostream &out)
      : _out(out), _indent(0), _needs_comma(false) {}

  JsonEmitter(const Self &) = delete;
  JsonEmitter(Self &&) = delete;
  ~JsonEmitter() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  inline void emitIndent() {
    for (unsigned i = 0; i < this->_indent; i++) {
      this->_out << "  ";
    }
  }

  inline void emitCommaIfNeeded() {
    if (this->_needs_comma) {
      this->_out << ",";
    }
  }

  inline void emitNewline() { this->_out << "\n"; }

  inline void beginObject() {
    this->emitCommaIfNeeded();
    this->emitNewline();
    this->emitIndent();
    this->_out << "{";
    this->_indent++;
    this->_needs_comma = false;
  }

  inline void beginObject(std::string_view key) {
    this->emitCommaIfNeeded();
    this->emitNewline();
    this->emitIndent();
    this->_out << "\"" << key << "\": {";
    this->_indent++;
    this->_needs_comma = false;
  }

  inline void endObject() {
    this->_indent--;
    this->emitNewline();
    this->emitIndent();
    this->_out << "}";
    this->_needs_comma = true;
  }

  inline void beginArray(std::string_view key) {
    this->emitCommaIfNeeded();
    this->emitNewline();
    this->emitIndent();
    this->_out << "\"" << key << "\": [";
    this->_indent++;
    this->_needs_comma = false;
  }

  inline void endArray() {
    this->_indent--;
    this->emitNewline();
    this->emitIndent();
    this->_out << "]";
    this->_needs_comma = true;
  }

  inline void emitKey(std::string_view key) {
    this->emitCommaIfNeeded();
    this->emitNewline();
    this->emitIndent();
    this->_out << "\"" << key << "\": ";
    this->_needs_comma = false;
  }

  template <typename ValueParam>
  inline void emit(std::string_view key, ValueParam value) {
    this->emitCommaIfNeeded();
    this->emitNewline();
    this->emitIndent();
    this->_out << "\"" << key << "\": ";
    if constexpr (std::same_as<ValueParam, bool>) {
      this->_out << (value ? "true" : "false");
    } else if constexpr (std::same_as<ValueParam, std::string_view> ||
                         std::same_as<ValueParam, const char *> ||
                         std::same_as<ValueParam, llvm::StringRef>) {
      this->_out << "\"" << value << "\"";
    } else {
      this->_out << value;
    }
    this->_needs_comma = true;
  }

  inline void emitString(std::string_view key, std::string_view value) {
    this->emit(key, value);
  }

  inline void emitString(std::string_view value) {
    this->emitCommaIfNeeded();
    this->emitNewline();
    this->emitIndent();
    this->_out << "\"" << value << "\"";
    this->_needs_comma = true;
  }

  inline void emitNull(std::string_view key) {
    this->emitCommaIfNeeded();
    this->emitNewline();
    this->emitIndent();
    this->_out << "\"" << key << "\": null";
    this->_needs_comma = true;
  }
};

} // namespace rq

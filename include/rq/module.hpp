#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/MemoryBufferRef.h>

#include <cstdint>
#include <string>
#include <utility>

namespace rq {

struct Expression;

enum class ModuleType : std::uint_fast8_t { NONE, SOURCE, IMPORT };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleType type) {
  switch (type) {
  case rq::ModuleType::NONE:
    return "none";
  case rq::ModuleType::SOURCE:
    return "source";
  case rq::ModuleType::IMPORT:
    return "import";
  }
  RQ_UNREACHABLE();
}

enum class Language : std::uint_fast8_t {
  NONE,
  UNKNOWN,
  NORMATIVE_REQUITE,
  SYMBOLIC_REQUITE
};

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
getDescription(rq::Language language) {
  switch (language) {
  case rq::Language::NONE:
    return "none";
  case rq::Language::UNKNOWN:
    return "unknown language";
  case rq::Language::NORMATIVE_REQUITE:
    return "normative requite";
  case rq::Language::SYMBOLIC_REQUITE:
    return "symbolic requite";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Language
getLanguageOfExtension(llvm::StringRef extension) {
  if (extension == ".rq") {
    return rq::Language::NORMATIVE_REQUITE;
  } else if (extension == ".srq") {
    return rq::Language::SYMBOLIC_REQUITE;
  }
  return rq::Language::UNKNOWN;
}

struct Module final {
  using Self = rq::Module;

  rq::ModuleType _type;
  rq::Language _language;
  llvm::MemoryBufferRef _llvm_buffer_ref;
  std::string _path;
  rq::Expression *_expression_ptr = nullptr;

  Module(rq::ModuleType type, rq::Language language, llvm::StringRef path,
         llvm::MemoryBufferRef &&buffer)
      : _type(type), _language(language), _llvm_buffer_ref(std::move(buffer)),
        _path(path) {}
  Module(const Self &) = delete;
  Module(Self &&) = delete;
  ~Module() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  bool operator==(const Self &rhs) const { return this == &rhs; }
  bool operator!=(const Self &rhs) const { return this != &rhs; }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleType getType() const {
    return this->_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Language getLanguage() const {
    return this->_language;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInitialized() const {
    return this->getLanguage() != rq::Language::NONE &&
           this->getType() != rq::ModuleType::NONE && !this->_path.empty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const {
    return this->_path;
  }
  llvm::StringRef getSourceText() const {
    return this->_llvm_buffer_ref.getBuffer();
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression *expression_ptr) {
    rq::assignSingleValue(this->_expression_ptr, expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};

} // namespace rq
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

  rq::ModuleType _type = rq::ModuleType::NONE;
  rq::Language _language = rq::Language::NONE;
  llvm::MemoryBufferRef _llvm_buffer_ref{};
  bool _is_valid = true;
  std::string _path{};
  rq::Expression *_expression_ptr = nullptr;

  Module() = default;
  Module(const Self &) = delete;
  Module(Self &&) = delete;
  ~Module() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  bool operator==(const Self &rhs) const { return this == &rhs; }
  bool operator!=(const Self &rhs) const { return this != &rhs; }
  RQ_ALWAYS_INLINE void setType(rq::ModuleType type) {
    RQ_ASSERT(this->_type == rq::ModuleType::NONE, "module type already set");
    this->_type = type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleType getType() const {
    return this->_type;
  }
  RQ_ALWAYS_INLINE void setLangauge(rq::Language language) {
    RQ_ASSERT(this->_language == rq::Language::NONE, "language already set");
    this->_language = language;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Language getLanguage() const {
    return this->_language;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInitialized() const {
    return this->getLanguage() != rq::Language::NONE &&
           this->getType() != rq::ModuleType::NONE && !this->_path.empty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsValid() const {
    return this->_is_valid;
  }
  RQ_ALWAYS_INLINE void setIsNotValid() { this->_is_valid = false; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLlvmBuffer() const {
    return !this->_path.empty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const {
    return this->_path;
  }
  RQ_ALWAYS_INLINE void setPath(llvm::StringRef path) {
    RQ_ASSERT(this->getPath().empty(), "path already set");
    this->_path = path;
  }
  llvm::StringRef getSourceText() const {
    RQ_ASSERT(this->getHasLlvmBuffer(), "does not have llvm file buffer");
    return this->_llvm_buffer_ref.getBuffer();
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};

} // namespace rq
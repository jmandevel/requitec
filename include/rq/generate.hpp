#pragma once
#include <rq/utility.hpp>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <optional>

namespace rq {

struct Context;
struct Module;
struct Expression;
struct Table;
struct Instruction;

struct Generator final {
  using Self = rq::Generator;

  rq::Context *_context_ptr;
  bool _is_ok : 1 = true;

  Generator(rq::Context &context) : _context_ptr(&context) {}
  Generator(const Self &) = delete;
  Generator(Self &&) = delete;
  ~Generator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return rq::dereferencePtr(this->_context_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return rq::dereferencePtr(this->_context_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const { return this->_is_ok; }
  void setNotOk() { this->_is_ok = false; }
  void generateSourceModule();
  void generateGlobalForest(const rq::Expression &first,
                            rq::Table &hosting_table, rq::Module &module);
  rq::Instruction *generateProcedureForest(const rq::Expression &first,
                                       rq::Table &hosting_table,
                                       rq::Procedure &procedure);
  [[nodiscard]] std::optional<llvm::StringRef>
  evaluateName(const rq::Expression &expression,
               rq::Table &hosting_table);
  [[nodiscard]] std::optional<llvm::StringRef>
  evaluateUtf8Cstr(const rq::Expression &expression,
                   rq::Table &hosting_table);
  [[nodiscard]] rq::Table &
  determineContainingTable(const rq::Expression &unascribed,
                           rq::Table &hosting_table, rq::Module &module);
  [[nodiscard]] const rq::SymbolConstant *
  inferenceType(const rq::Expression &expression,
                rq::Table &hosting_table, rq::Module &module);
  [[nodiscard]] bool implementProcedure(rq::Procedure &procedure);
  [[nodiscard]] bool implementGlobal(rq::GlobalDynamicVariable &global);
};

} // namespace rq

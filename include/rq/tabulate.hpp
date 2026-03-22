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
struct SymbolTable;
struct Instruction;

struct ExecutionFactory final {
  using Self = rq::ExecutionFactory;

  rq::Instruction *_instruction_ptr{nullptr};
  rq::BinaryInstruction *_last_exec_ptr{nullptr};

  ExecutionFactory() = default;

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInstruction() const {
    return this->_instruction_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getInstruction() {
    return rq::dereferencePtr(this->_instruction_ptr);
  }
  void addInstruction(rq::Context &context, rq::Instruction &instruction);
};

struct Tabulator final {
  using Self = rq::Tabulator;

  rq::Context *_context_ptr;
  rq::Expression *_trunk_ptr{nullptr};
  llvm::DenseMap<rq::Expression *, llvm::SmallVector<rq::Symbol *>>
      _declaration_map{};
  bool _is_ok : 1 = true;

  Tabulator(rq::Context &context) : _context_ptr(&context) {}
  Tabulator(const Self &) = delete;
  Tabulator(Self &&) = delete;
  ~Tabulator() = default;
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
  void tabulateSourceModule();
  void tabulateGlobalForest(const rq::Expression &first,
                            rq::SymbolTable &hosting_table, rq::Module &module);
  rq::Instruction *tabulateLocalForest(const rq::Expression &first,
                                       rq::SymbolTable &hosting_table,
                                       rq::Procedure &procedure);
  [[nodiscard]] std::optional<llvm::StringRef>
  evaluateName(const rq::Expression &expression,
               rq::SymbolTable &hosting_table);
  [[nodiscard]] std::optional<llvm::StringRef>
  evaluateUtf8Cstr(const rq::Expression &expression,
                   rq::SymbolTable &hosting_table);
  [[nodiscard]] rq::SymbolTable &
  determineContainingTable(const rq::ExpressionFlagsFactory &factory,
                           const rq::Expression &unascribed,
                           rq::SymbolTable &hosting_table, rq::Module &module);
  [[nodiscard]] rq::Entity *evaluateValue(const rq::Expression &expression,
                                          rq::TypeConstant &type,
                                          rq::SymbolTable &hosting_table);
  [[nodiscard]] rq::Symbol *evaluateSymbol(const rq::Expression &path,
                                           rq::SymbolTable &hosting_table,
                                           rq::Module &module);
  [[nodiscard]] rq::TypeConstant *evaluateType(const rq::Expression &path,
                                               rq::SymbolTable &hosting_table,
                                               rq::Module &module);
  [[nodiscard]] rq::TypeConstant *
  inferenceType(const rq::Expression &type_expression,
                rq::SymbolTable &hosting_table, rq::Module &module);
  void implementProcedure(rq::Procedure &procedure);
  void implementGlobalVariable(rq::GlobalVariable &global);
  void tabulateGlobalVariable(
      rq::SymbolTable &containing_table, rq::SymbolTable &hosting_table,
      rq::Module &module, const rq::ExpressionFlagsFactory &factory,
      const rq::Expression &unascribed, const rq::Expression &name,
      const rq::Expression &type, const rq::Expression *value_ptr);
};

} // namespace rq

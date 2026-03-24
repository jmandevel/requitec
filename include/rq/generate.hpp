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

template <rq::Opcode OPCODE_PARAM> struct InstructionFactory final {
  static constexpr rq::Opcode OPCODE = OPCODE_PARAM;
  using Self = rq::InstructionFactory<OPCODE>;

  rq::Instruction *_instruction_ptr{nullptr};
  rq::BinaryInstruction *_last_exec_ptr{nullptr};

  InstructionFactory() = default;

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInstruction() const {
    return this->_instruction_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getInstruction() {
    return rq::dereferencePtr(this->_instruction_ptr);
  }
  inline void addInstruction(rq::Context &context, rq::Instruction &instruction) {
    if (this->_instruction_ptr == nullptr) {
      this->_instruction_ptr = &instruction;
      return;
    }
    rq::Instruction &previous_instruction =
        rq::dereferencePtr(this->_instruction_ptr);
    rq::BinaryInstruction &new_exec = context.acquireBinaryInstruction(OPCODE);
    new_exec.setAddress1(instruction);
    if (this->_last_exec_ptr == nullptr) {
      new_exec.setAddress0(previous_instruction);
      this->_instruction_ptr = &new_exec;
      this->_last_exec_ptr = &new_exec;
      return;
    }
    rq::BinaryInstruction &last_exec = rq::dereferencePtr(this->_last_exec_ptr);
    new_exec.setAddress0(last_exec.replaceAddress1(new_exec));
  }
};

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
                            rq::SymbolTable &hosting_table, rq::Module &module);
  rq::Instruction *generateLocalForest(const rq::Expression &first,
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
  void generateGlobalVariable(
      rq::SymbolTable &containing_table, rq::SymbolTable &hosting_table,
      rq::Module &module, const rq::ExpressionFlagsFactory &factory,
      const rq::Expression &unascribed, const rq::Expression &name,
      const rq::Expression &type, const rq::Expression *value_ptr);
};

} // namespace rq

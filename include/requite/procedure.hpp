// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_flags.hpp>
#include <requite/procedure_iterator.hpp>
#include <requite/procedure_type.hpp>
#include <requite/scope.hpp>
#include <requite/signature.hpp>

#include <llvm/ADT/SmallVector.h>

#include <string>

namespace llvm {

class FunctionType;
class Function;
class BasicBlock;

} // namespace llvm

namespace requite {

struct Expression;

struct Procedure final {
  using Self = requite::Procedure;

  std::string _name = {};
  std::string _mangled_name = {};
  requite::Module *_module_ptr = nullptr;
  requite::Expression *_expression_ptr = nullptr;
  requite::ProcedureType _type;
  requite::Scope _scope = {};
  requite::Signature _signature = {};
  requite::AttributeFlags _attributes = {};
  requite::Procedure *_next_ptr = nullptr;
  llvm::FunctionType *_llvm_function_type_ptr = nullptr;
  llvm::Function *_llvm_function_ptr = nullptr;
  llvm::BasicBlock *_llvm_block_ptr = nullptr;

  // procedure.cpp
  Procedure(requite::ProcedureType type);
  Procedure(Self &that) = delete;
  Procedure(Self &&that) = delete;
  ~Procedure() = default;
  Self &operator=(Self &rhs) = delete;
  Self &operator=(Self &&rhs) = delete;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]] bool getCanHaveName() const;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] bool getHasContainingScope() const;
  void setContainingScope(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContainingScope();
  [[nodiscard]] const requite::Scope &getContainingScope() const;
  void setMangledName(llvm::StringRef name);
  [[nodiscard]] bool getHasMangledName() const;
  [[nodiscard]] llvm::StringRef getMangledName() const;
  [[nodiscard]] requite::ProcedureType getType() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] requite::Signature &getSignature();
  [[nodiscard]] const requite::Signature &getSignature() const;
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
  [[nodiscard]] const requite::AttributeFlags &getAttributeFlags() const;
  void setNextProcedure(requite::Procedure &procedure);
  [[nodiscard]] bool getHasNextProcedure() const;
  [[nodiscard]] requite::Procedure &getNextProcedure();
  [[nodiscard]] const requite::Procedure &getNextProcedure() const;
  [[nodiscard]] bool getHasModule() const;
  void setModule(requite::Module &module);
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] const requite::Module &getModule() const;
  [[nodiscard]] bool getHasLlvmFunctionType() const;
  void setLlvmFunctionType(llvm::FunctionType &type);
  [[nodiscard]] llvm::FunctionType &getLlvmFunctionType();
  [[nodiscard]] const llvm::FunctionType &getLlvmFunctionType() const;
  [[nodiscard]] bool getHasLlvmFunction() const;
  void setLlvmFunction(llvm::Function &function);
  [[nodiscard]] llvm::Function &getLlvmFunction();
  [[nodiscard]] const llvm::Function &getLlvmFunction() const;
  [[nodiscard]] bool getHasLlvmBlock() const;
  void setLlvmBlock(llvm::BasicBlock &block);
  [[nodiscard]] llvm::BasicBlock &getLlvmBlock();
  [[nodiscard]] const llvm::BasicBlock &getLlvmBlock() const;

  // detail/procedure_subrange.hpp
  [[nodiscard]] inline std::ranges::subrange<
      requite::ProcedureIterator, requite::ProcedureIterator,
      std::ranges::subrange_kind::unsized>
  getOverloadSubrange();
};

} // namespace requite

#include <requite/detail/procedure_subrange.hpp>
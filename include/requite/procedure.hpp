// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_flags.hpp>
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
struct NamedProcedureGroup;
struct Node;

struct Procedure final {
  using Self = requite::Procedure;

  bool _dependent_name = false;
  std::string _mangled_name = {};
  requite::Expression* _expression_ptr = nullptr;
  requite::ProcedureType _type = requite::ProcedureType::NONE;
  requite::Scope _scope = {};
  requite::Signature _signature = {};
  requite::AttributeFlags _attributes = {};
  requite::NamedProcedureGroup *_group_ptr = nullptr;
  requite::Procedure *_next_ptr = nullptr;
  llvm::FunctionType *_llvm_function_type_ptr = nullptr;
  llvm::Function *_llvm_function_ptr = nullptr;
  llvm::BasicBlock *_llvm_block_ptr = nullptr;

  // procedure.cpp
  Procedure();
  Procedure(Self &that) = delete;
  Procedure(Self &&that) = delete;
  ~Procedure() = default;
  Self &operator=(Self &rhs) = delete;
  Self &operator=(Self &&rhs) = delete;
  [[nodiscard]]
  bool operator==(const Self &rhs) const;
  [[nodiscard]]
  bool operator!=(const Self &rhs) const;
  [[nodiscard]] bool getIsNamed() const;
  void setHasDependentName();
  [[nodiscard]] bool getHasDependentName() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
  void setMangledName(llvm::StringRef name);
  [[nodiscard]] bool getHasMangledName() const;
  [[nodiscard]] llvm::StringRef getMangledName() const;
  void setType(requite::ProcedureType type);
  [[nodiscard]] requite::ProcedureType getType() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] requite::Signature &getSignature();
  [[nodiscard]] const requite::Signature &getSignature() const;
  void setAttributeFlags(requite::AttributeFlags attributes);
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
  [[nodiscard]] const requite::AttributeFlags &getAttributeFlags() const;
  void
  setNamedProcedureGroup(requite::NamedProcedureGroup &group);
  [[nodiscard]] bool getHasNamedProcedureGroup() const;
  [[nodiscard]] requite::NamedProcedureGroup &getNamedProcedureGroup();
  [[nodiscard]] const requite::NamedProcedureGroup &
  getNamedProcedureGroup() const;
  [[nodiscard]] requite::NamedProcedureGroup *getNamedProcedureGroupPtr();
  [[nodiscard]] const requite::NamedProcedureGroup *
  getNamedProcedureGroupPtr() const;
  void setNextProcedure(requite::Procedure &procedure);
  [[nodiscard]] bool getHasNextProcedure() const;
  [[nodiscard]] requite::Procedure &getNextProcedure();
  [[nodiscard]] const requite::Procedure &getNextProcedure() const;
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
};

} // namespace requite
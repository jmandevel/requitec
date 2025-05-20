// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/alias.hpp>
#include <requite/attributes.hpp>
#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/label.hpp>
#include <requite/module.hpp>
#include <requite/node.hpp>
#include <requite/object.hpp>
#include <requite/scope.hpp>
#include <requite/strings.hpp>
#include <requite/symbol.hpp>
#include <requite/table.hpp>
#include <requite/tabulator.hpp>
#include <requite/variable.hpp>

#include <llvm/ADT/Twine.h>

#include <ranges>
#include <utility>

namespace requite {

bool Context::tabulateSymbols(requite::Module &module) {
  requite::Tabulator tabulator(*this, module);
  tabulator.tabulateModule();
  return tabulator.getIsOk();
}

void Tabulator::tabulateModule() {
  requite::Expression &root = this->getModule().getSource().getRoot();
  REQUITE_ASSERT(root.getOpcode() == requite::Opcode::MODULE);
  requite::Expression &module_name_expression = root.getBranch();
  llvm::StringRef module_name = module_name_expression.getDataText();
  this->getModule().getTable().setName(module_name);
  for (requite::Expression &branch :
       root.getBranchSubrange() | std::views::drop(1)) {
    this->searchForUnorderedSymbolExpression(
        this->getModule().getTable().getScope(), requite::Attributes{}, branch);
  }
}

void Tabulator::searchForUnorderedSymbolExpression(
    requite::Scope &scope, requite::Attributes attributes,
    requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::ASCRIBE:
    REQUITE_ASSERT(!attributes.getHasAnyAttribute());
    this->tabulateUnorderedAscribe(scope, expression);
    break;
  case requite::Opcode::ENTRY_POINT:
    this->tabulateUnorderedEntryPoint(scope, attributes, expression);
    break;
  case requite::Opcode::FUNCTION:
    [[fallthrough]];
  case requite::Opcode::METHOD:
    this->tabulateUnorderedNamedProcedure(scope, attributes, expression);
    break;
  case requite::Opcode::CONSTRUCTOR:
    this->tabulateUnorderedConstructor(scope, attributes, expression);
    break;
  case requite::Opcode::DESTRUCTOR:
    this->tabulateUnorderedDestructor(scope, attributes, expression);
    break;
  case requite::Opcode::OBJECT:
    this->tabulateUnorderedObject(scope, attributes, expression);
    break;
  case requite::Opcode::TABLE:
    if (attributes.getHasAnyAttribute()) {
      this->getContext().logSourceMessage(
          expression, requite::LogType::ERROR,
          llvm::Twine("table can not have attributes"));
      this->setNotOk();
      break;
    }
    this->tabulateUnorderedTable(scope, expression);
    break;
  case requite::Opcode::ALIAS:
    this->tabulateUnorderedAlias(scope, attributes, expression);
    break;
  case requite::Opcode::GLOBAL:
    this->tabulateUnorderedGlobal(scope, attributes, expression);
    break;
  case requite::Opcode::PROPERTY:
    this->tabulateUnorderedProperty(scope, attributes, expression);
    break;
  case requite::Opcode::SCOPE:
    if (attributes.getHasAnyAttribute()) {
      this->getContext().logSourceMessage(
          expression, requite::LogType::ERROR,
          llvm::Twine("scope can not have attributes"));
      this->setNotOk();
      break;
    }
    this->tabulateUnorderedScope(scope, expression);
    break;
  case requite::Opcode::IMPORT:
    this->tabulateUnorderedImport(scope, attributes, expression);
    break;
  case requite::Opcode::USE:
    this->tabulateUnorderedUse(scope, attributes, expression);
    break;
  default:
    for (requite::Expression &branch : expression.getBranchSubrange()) {
      this->searchForScopedValue(scope, branch);
    }
    break;
  }
}

void Tabulator::tabulateUnorderedAscribe(requite::Scope &scope,
                                         requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ASCRIBE);
  requite::MakeAttributesResult result =
      requite::Attributes::makeAttributes(this->getContext(), expression);
  if (result.has_error) {
    this->setNotOk();
    return;
  }
  for (std::string &label_name : result.label_names) {
    requite::Label &label = this->getModule().makeLabel();
    label.setName(label_name);
    label.setExpression(expression);
    scope.addSymbol(label);
  }
  this->searchForUnorderedSymbolExpression(
      scope, result.attributes, requite::getRef(result.last_expression_ptr));
}

void Tabulator::tabulateUnorderedEntryPoint(requite::Scope &scope,
                                            requite::Attributes attributes,
                                            requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ENTRY_POINT);
  if (this->getModule().getHasEntryPoint()) {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("duplicate entry point found in module ") +
            this->getModule().getTable().getName() + "");
    this->setNotOk();
    return;
  } else {
    this->getContext().logSourceMessage(
        expression, requite::LogType::REMARK,
        llvm::Twine("entry point found in module ") +
            this->getModule().getTable().getName() + "");
  }
  requite::Procedure &procedure = this->getModule().makeProcedure();
  procedure.setType(requite::ProcedureType::ENTRY_POINT);
  procedure.getAttributes() = attributes;
  procedure.getScope().setExpression(expression);
  procedure.getScope().setContainingScope(scope);
  this->getModule().setEntryPoint(procedure);
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    this->searchForLocalMatteStatement(procedure.getScope(),
                                       requite::Attributes{}, branch);
  }
}

void Tabulator::tabulateUnorderedNamedProcedure(
    requite::Scope &scope, requite::Attributes attributes,
    requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::FUNCTION ||
                 expression.getOpcode() == requite::Opcode::METHOD);
  requite::Procedure &procedure = this->getModule().makeProcedure();
  procedure.setType(expression.getOpcode() == requite::Opcode::FUNCTION
                        ? requite::ProcedureType::FUNCTION
                        : requite::ProcedureType::METHOD);
  procedure.getAttributes() = attributes;
  procedure.getScope().setExpression(expression);
  procedure.getScope().setContainingScope(scope);
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name = name_expression.getDataText();
  requite::Expression &signature_expression = name_expression.getNext();
  requite::RootSymbol symbol = scope.lookupInternalRootSymbol(name);
  requite::NamedProcedureGroup *procedure_group_ptr = nullptr;
  if (symbol.getIsNone()) {
    requite::NamedProcedureGroup &procedure_group =
        this->getModule().makeNamedProcedureGroup();
    procedure_group.setName(name);
    scope.addSymbol(procedure_group);
    procedure_group_ptr = &procedure_group;
  } else if (symbol.getIsNamedProcedureGroup()) {
    procedure_group_ptr = &symbol.getNamedProcedureGroup();
  } else {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("symbol '") + name +
            "' already exists and is not a procedure");
    this->setNotOk();
    return;
  }
  requite::NamedProcedureGroup &procedure_group =
      requite::getRef(procedure_group_ptr);
  procedure_group.addProcedure(procedure);
  for (requite::Expression &branch :
       signature_expression.getBranchSubrange() | std::views::drop(1)) {
    this->searchForLocalMatteStatement(procedure.getScope(),
                                       requite::Attributes{}, branch);
  }
}

void Tabulator::tabulateUnorderedConstructor(requite::Scope &scope,
                                             requite::Attributes attributes,
                                             requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::CONSTRUCTOR);
  if (!scope.getHasObject()) {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("constructor must be associated with an object"));
    this->setNotOk();
    return;
  }
  requite::Procedure &procedure = this->getModule().makeProcedure();
  procedure.setType(requite::ProcedureType::CONSTRUCTOR);
  procedure.getAttributes() = attributes;
  procedure.getScope().setExpression(expression);
  procedure.getScope().setContainingScope(scope);
  scope.getObject().addConstructor(procedure);
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    this->searchForLocalMatteStatement(procedure.getScope(),
                                       requite::Attributes{}, branch);
  }
}

void Tabulator::tabulateUnorderedDestructor(requite::Scope &scope,
                                            requite::Attributes attributes,
                                            requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::DESTRUCTOR);
  if (!scope.getHasObject()) {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("destructor must be associated with an object"));
    this->setNotOk();
    return;
  }
  requite::Procedure &procedure = this->getModule().makeProcedure();
  procedure.setType(requite::ProcedureType::DESTRUCTOR);
  procedure.getAttributes() = attributes;
  procedure.getScope().setExpression(expression);
  procedure.getScope().setContainingScope(scope);
  scope.getObject().setDestructor(procedure);
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    this->searchForLocalMatteStatement(procedure.getScope(),
                                       requite::Attributes{}, branch);
  }
}

void Tabulator::tabulateUnorderedObject(requite::Scope &scope,
                                        requite::Attributes attributes,
                                        requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::OBJECT);
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name = name_expression.getDataText();
  requite::RootSymbol existing = scope.lookupInternalRootSymbol(name);
  if (!existing.getIsNone()) {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("there is already a symbol with name \"") + name + "\"");
    this->setNotOk();
    return;
  }
  requite::Object &object = this->getModule().makeObject();
  object.setName(name);
  object.getTable().getScope().setContainingScope(scope);
  object.getTable().getScope().setObject(object);
  object.getAttributes() = attributes;
  object.getTable().getScope().setExpression(expression);
  if (scope.getHasObject()) {
    object.getTable().getScope().setObject(scope.getObject());
  }
  scope.addSymbol(object);
  for (requite::Expression &branch :
       expression.getBranchSubrange() | std::views::drop(1)) {
    this->searchForUnorderedSymbolExpression(object.getTable().getScope(),
                                             requite::Attributes{}, branch);
  }
}

void Tabulator::tabulateUnorderedTable(requite::Scope &scope,
                                       requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::TABLE);
  requite::Expression &path_expression = expression.getBranch();
  requite::Table *table_ptr =
      this->getOrMakeTableAtPath(scope, expression, path_expression);
  if (table_ptr == nullptr) {
    return;
  }
  requite::Table &table = requite::getRef(table_ptr);
  for (requite::Expression &branch :
       expression.getBranchSubrange() | std::views::drop(1)) {
    this->searchForUnorderedSymbolExpression(table.getScope(),
                                             requite::Attributes{}, branch);
  }
}

void Tabulator::tabulateUnorderedAlias(requite::Scope &scope,
                                       requite::Attributes attributes,
                                       requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ALIAS);
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name = name_expression.getDataText();
  requite::RootSymbol existing = scope.lookupInternalRootSymbol(name);
  if (!existing.getIsNone()) {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("there is already a symbol with name \"") + name + "\"");
    this->setNotOk();
    return;
  }
  requite::Alias &alias = this->getModule().makeAlias();
  alias.setName(name);
  alias.getAttributes() = attributes;
  alias.setExpression(expression);
  alias.setContainingScope(scope);
  scope.addSymbol(alias);
}

void Tabulator::tabulateUnorderedGlobal(requite::Scope &scope,
                                        requite::Attributes attributes,
                                        requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::GLOBAL);
  requite::Expression &binding_expression = expression.getBranch();
  requite::Expression &name_expression = binding_expression.getBranch();
  llvm::StringRef name = name_expression.getDataText();
  requite::RootSymbol existing = scope.lookupInternalRootSymbol(name);
  if (!existing.getIsNone()) {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("there is already a symbol with name \"") + name + "\"");
    this->setNotOk();
    return;
  }
  requite::Variable &variable = this->getModule().makeVariable();
  variable.setName(name);
  variable.setType(requite::VariableType::GLOBAL);
  variable.getAttributes() = attributes;
  variable.setExpression(expression);
  variable.setContainingScope(scope);
  scope.addSymbol(variable);
}

void Tabulator::tabulateUnorderedProperty(requite::Scope &scope,
                                          requite::Attributes attributes,
                                          requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::PROPERTY);
  if (!scope.getHasObject()) {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("property must be associated with an object"));
    this->setNotOk();
    return;
  }
  requite::Expression &binding_expression = expression.getBranch();
  requite::Expression &name_expression = binding_expression.getBranch();
  llvm::StringRef name = name_expression.getDataText();
  requite::RootSymbol existing = scope.lookupInternalRootSymbol(name);
  if (existing.getIsNone()) {
    requite::Variable &variable = this->getModule().makeVariable();
    variable.setName(name);
    variable.setType(requite::VariableType::PROPERTY);
    variable.getAttributes() = attributes;
    variable.setExpression(expression);
    variable.setContainingScope(scope);
    scope.addSymbol(variable);
    requite::Object &object = scope.getObject();
  } else {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("there is already a symbol with name \"") + name + "\"");
    this->setNotOk();
    return;
  }
}

void Tabulator::tabulateUnorderedScope(requite::Scope &scope,
                                       requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::SCOPE);
  requite::Scope &this_scope = this->getModule().makeScope();
  this_scope.setContainingScope(scope);
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    this->searchForUnorderedSymbolExpression(this_scope, requite::Attributes{},
                                             branch);
  }
}

void Tabulator::tabulateUnorderedImport(requite::Scope &scope,
                                        requite::Attributes attributes,
                                        requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::IMPORT);
  requite::Node &node = scope.getNodes().emplace_back();
  node.setType(requite::NodeType::IMPORT);
  node.setExpression(expression);
  node.getAttributes() = attributes;
}

void Tabulator::tabulateUnorderedUse(requite::Scope &scope,
                                     requite::Attributes attributes,
                                     requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::USE);
  requite::Node &node = scope.getNodes().emplace_back();
  node.setType(requite::NodeType::USE);
  node.setExpression(expression);
  node.getAttributes() = attributes;
}

void Tabulator::searchForLocalMatteStatement(requite::Scope &scope,
                                             requite::Attributes attributes,
                                             requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::ASCRIBE:
    REQUITE_ASSERT(!attributes.getHasAnyAttribute());
    this->tabulateLocalAscribe(scope, expression);
    break;
  case requite::Opcode::LOCAL:
    this->tabulateLocalLocal(scope, attributes, expression);
    break;
  case requite::Opcode::IF:
    [[fallthrough]];
  case requite::Opcode::ELSE_IF:
    [[fallthrough]];
  case requite::Opcode::ELSE:
    [[fallthrough]];
  case requite::Opcode::SWITCH:
    [[fallthrough]];
  case requite::Opcode::CASE:
    [[fallthrough]];
  case requite::Opcode::DEFAULT_CASE:
    [[fallthrough]];
  case requite::Opcode::FOR:
    [[fallthrough]];
  case requite::Opcode::WHILE:
    [[fallthrough]];
  case requite::Opcode::DO_WHILE:
    [[fallthrough]];
  case requite::Opcode::FOR_EACH:
    [[fallthrough]];
  case requite::Opcode::LOOP:
    [[fallthrough]];
  case requite::Opcode::SCOPE:
    [[fallthrough]];
  case requite::Opcode::TRY:
    this->tabulateLocalScopedStatement(scope, expression);
    break;
  case requite::Opcode::CATCH:
    this->tabulateLocalScopedStatementSkipFirst(scope, expression);
    break;
  default:
    this->searchForUnorderedSymbolExpression(scope, attributes, expression);
  }
}

void Tabulator::tabulateLocalAscribe(requite::Scope &scope,
                                     requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ASCRIBE);
  requite::MakeAttributesResult result =
      requite::Attributes::makeAttributes(this->getContext(), expression);
  if (result.has_error) {
    this->setNotOk();
    return;
  }
  requite::Expression &unascribed = requite::getRef(result.last_expression_ptr);
}

void Tabulator::tabulateLocalLocal(requite::Scope &scope,
                                   requite::Attributes attributes,
                                   requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::LOCAL);
  requite::Expression &binding_expression = expression.getBranch();
  requite::Expression &name_expression = binding_expression.getBranch();
  llvm::StringRef name = name_expression.getDataText();
  requite::RootSymbol symbol = scope.lookupInternalRootSymbol(name);
  if (!symbol.getIsNone()) {
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        llvm::Twine("there is already a symbol with name \"") + name + "\"");
    this->setNotOk();
    return;
  }
  requite::Variable &variable = this->getModule().makeVariable();
  variable.setName(name);
  variable.setType(requite::VariableType::LOCAL);
  variable.setExpression(expression);
  variable.setContainingScope(scope);
  variable.getAttributes() = requite::Attributes{};
  scope.addSymbol(variable);
}

void Tabulator::tabulateLocalScopedStatement(requite::Scope &scope,
                                             requite::Expression &expression) {
  requite::Scope &this_scope = this->getModule().makeScope();
  expression.setScope(this_scope);
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    this->searchForLocalMatteStatement(this_scope, requite::Attributes{},
                                       branch);
  }
}

void Tabulator::tabulateLocalScopedStatementSkipFirst(
    requite::Scope &scope, requite::Expression &expression) {
  requite::Scope &this_scope = this->getModule().makeScope();
  expression.setScope(this_scope);
  for (requite::Expression &branch :
       expression.getBranchSubrange() | std::views::drop(1)) {
    this->searchForLocalMatteStatement(this_scope, requite::Attributes{},
                                       branch);
  }
}

void Tabulator::searchForScopedValue(requite::Scope &scope,
                                     requite::Expression &expression) {
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    switch (const requite::Opcode opcode = branch.getOpcode()) {
    case requite::Opcode::CONDUIT:
      this->tabulateConduit(scope, branch);
      break;
    case requite::Opcode::ANONYMOUS_FUNCTION:
      this->tabulateScopedValueAnonymousFunction(scope, branch);
      break;
    default:
      this->searchForScopedValue(scope, branch);
    }
  }
}

void Tabulator::tabulateConduit(requite::Scope &scope,
                                requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::CONDUIT);
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    this->searchForLocalMatteStatement(scope, requite::Attributes{}, branch);
  }
}

void Tabulator::tabulateScopedValueAnonymousFunction(
    requite::Scope &scope, requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ANONYMOUS_FUNCTION);
  requite::AnonymousFunction &anonymous_function =
      this->getModule().makeAnonymousFunction();
  anonymous_function.setExpression(expression);
  expression.setAnonymousFunction(anonymous_function);
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    this->searchForLocalMatteStatement(anonymous_function.getScope(),
                                       requite::Attributes{}, branch);
  }
}

requite::Table *
Tabulator::getOrMakeTableOfName(requite::Scope &scope,
                                requite::Expression &expression,
                                requite::Expression &name_expression) {
  llvm::StringRef name = name_expression.getDataText();
  requite::RootSymbol symbol = scope.lookupInternalRootSymbol(name);
  if (symbol.getIsNone()) {
    requite::Table &table = this->getModule().makeTable();
    table.setName(name);
    table.getScope().setContainingScope(scope);
    scope.addSymbol(table);
    return &table;
  } else if (symbol.getIsTable()) {
    requite::Table &table = symbol.getTable();
    return &table;
  }
  this->getContext().logSourceMessage(
      expression, requite::LogType::ERROR,
      llvm::Twine("symbol \"") + name + "\" already exists and is not a table");
  this->setNotOk();
  return nullptr;
}

requite::Table *
Tabulator::getOrMakeTableAtPath(requite::Scope &scope,
                                requite::Expression &expression,
                                requite::Expression &path_expression) {
  if (path_expression.getHasDataText()) {
    return this->getOrMakeTableOfName(scope, expression, path_expression);
  }
  REQUITE_ASSERT(path_expression.getOpcode() ==
                 requite::Opcode::REFLECT_SYMBOL);
  requite::Table *current_table_ptr = this->getOrMakeTableOfName(
      scope, expression, path_expression.getBranch());
  if (current_table_ptr == nullptr) {
    return nullptr;
  }
  for (requite::Expression &branch :
       path_expression.getBranchSubrange() | std::views::drop(1)) {
    current_table_ptr = this->getOrMakeTableOfName(
        current_table_ptr->getScope(), expression, branch);
    if (current_table_ptr == nullptr) {
      return nullptr;
    }
  }
  return current_table_ptr;
}

} // namespace requite
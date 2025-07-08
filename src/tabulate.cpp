#include <requite/context.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/contextualizer1.hpp>
#include <requite/expression.hpp>
#include <requite/local.hpp>
#include <requite/log_mode.hpp>
#include <requite/module.hpp>
#include <requite/scope.hpp>
#include <requite/situator.hpp>

namespace requite {

void Contextualizer0::tabulateModule() {
  requite::Expression &root = this->getModule().getExpression();
  REQUITE_ASSERT(!root.getHasNext());
  requite::Expression &module_name = root.getBranch();
  for (requite::Expression &module_statement : module_name.getNextSubrange()) {
    this->tabulateStatement(module_statement,
                            requite::Situation::MODULE_STATEMENT, false);
  }
}

void Contextualizer0::tabulateStatement(requite::Expression &statement,
                                        requite::Situation situation,
                                        bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateStatement(statement.getBranch(), situation, true);
  case requite::Opcode::ENTRY_POINT:
    this->tabulateEntryPoint(statement, situation, has_attributes);
  case requite::Opcode::FUNCTION:
    this->tabulateFunction(statement, situation, has_attributes);
  case requite::Opcode::METHOD:
    this->tabulateMethod(statement, situation, has_attributes);
  case requite::Opcode::EXTENSION:
    this->tabulateExtension(statement, situation, has_attributes);
  case requite::Opcode::CONSTRUCTOR:
    this->tabulateConstructor(statement, situation, has_attributes);
  case requite::Opcode::DESTRUCTOR:
    this->tabulateDestructor(statement, situation, has_attributes);
  case requite::Opcode::OBJECT:
    this->tabulateObject(statement, situation, has_attributes);
  case requite::Opcode::ALIAS:
    this->tabulateAlias(statement, situation, has_attributes);
  case requite::Opcode::IMPORT:
    this->tabulateImport(statement, situation, has_attributes);
  case requite::Opcode::USE:
    this->tabulateUse(statement, situation, has_attributes);
  case requite::Opcode::GLOBAL:
    this->tabulateGlobal(statement, situation, has_attributes);
  case requite::Opcode::PROPERTY:
    this->tabulateProperty(statement, situation, has_attributes);
  case requite::Opcode::_EXPAND_VALUE:
    this->tabulate_ExpandValue(statement, situation, has_attributes);
  default:
    break;
  }
  REQUITE_UNREACHABLE();
}

void Contextualizer0::tabulateEntryPoint(requite::Expression &expression,
                                         requite::Situation situation,
                                         bool has_attributes) {
  requite::Procedure &procedure = this->getContext().makeProcedure();
  procedure.setType(requite::ProcedureType::ENTRY_POINT);
  procedure.setModule(this->getModule());
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  this->getModule().addEntryPoint(procedure);
  if (has_attributes) {
    this->getContext().logSourceMessage(expression, requite::LogType::ERROR,
                                        "entry_point must not have attributes");
    this->setNotOk();
  }
  if (!this->expandExpression(expression, requite::Situation::MODULE_STATEMENT,
                              requite::LogMode::QUIET)) [[unlikely]] {
    this->addYield(procedure, situation);
  }
}
void Contextualizer0::tabulateFunction(requite::Expression &expression,
                                       requite::Situation situation,
                                       bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::FUNCTION);
  requite::Procedure &procedure = this->getContext().makeProcedure();
  procedure.setType(requite::ProcedureType::FUNCTION);
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContaining(this->getScope());
  if (has_attributes) {
    // TODO tabulate attributes
  }
  if (!this->expandNameBranch(expression, situation, requite::LogMode::QUIET))
      [[unlikely]] {
    this->addYield(procedure, situation);
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateName(name, this->getScope(), name_expression,
                                       requite::LogMode::QUIET)) {
    this->addYield(procedure, situation);
  }
  requite::RootSymbol found = this->getScope().lookupUserSymbol(name);
  if (found.getIsNone()) {
    requite::NamedProcedureGroup &group =
        this->getContext().makeNamedProcedureGroup();
    group.setName(name);
    procedure.setNamedProcedureGroup(group);
    this->getScope().addUserSymbol(group);
  } else if (found.getIsNamedProcedureGroup()) {
    requite::NamedProcedureGroup &group = found.getNamedProcedureGroup();
    procedure.setNamedProcedureGroup(group);
  } else [[unlikely]] {
    this->getContext().logErrorAlreadySymbolOfName(expression);
    this->setNotOk();
  }
  if (!this->expandExpression(expression, situation, requite::LogMode::QUIET))
      [[unlikely]] {
    this->addYield(procedure, situation);
  }
  return;
}

void Contextualizer0::tabulateMethod(requite::Expression &expression,
                                     requite::Situation situation,
                                     bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::METHOD);
  REQUITE_ASSERT(this->getScope().getType() == requite::ScopeType::OBJECT);
  requite::Procedure &procedure = this->getContext().makeProcedure();
  procedure.setType(requite::ProcedureType::METHOD);
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContaining(this->getScope());
  if (has_attributes) {
    // TODO tabulate attributes
  }
  if (!this->expandNameBranch(expression, situation, requite::LogMode::QUIET))
      [[unlikely]] {
    this->addYield(procedure, situation);
    return;
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateName(name, this->getScope(), name_expression,
                                       requite::LogMode::QUIET)) [[unlikely]] {
    this->addYield(procedure, situation);
    return;
  }
  requite::RootSymbol found = this->getScope().lookupUserSymbol(name);
  if (found.getIsNone()) {
    requite::NamedProcedureGroup &group =
        this->getContext().makeNamedProcedureGroup();
    group.setName(name);
    procedure.setNamedProcedureGroup(group);
    this->getScope().addUserSymbol(group);
  } else if (found.getIsNamedProcedureGroup()) {
    requite::NamedProcedureGroup &group = found.getNamedProcedureGroup();
    procedure.setNamedProcedureGroup(group);
  } else [[unlikely]] {
    this->getContext().logErrorAlreadySymbolOfName(expression);
    this->setNotOk();
  }
  if (!this->expandExpression(expression, situation, requite::LogMode::QUIET))
      [[unlikely]] {
    this->addYield(procedure, situation);
  }
  return;
}

void Contextualizer0::tabulateExtension(requite::Expression &expression,
                                        requite::Situation situation,
                                        bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::EXTENSION);
  REQUITE_ASSERT(this->getScope().getType() == requite::ScopeType::MODULE ||
                 this->getScope().getType() == requite::ScopeType::TABLE);
  requite::Procedure &procedure = this->getContext().makeProcedure();
  procedure.setType(requite::ProcedureType::EXTENSION);
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContaining(this->getScope());
  if (has_attributes) {
    // TODO tabulate attributes
  }
  if (!this->expandNameBranch(expression, situation, requite::LogMode::QUIET))
      [[unlikely]] {
    this->addYield(procedure, situation, requite::YieldState::EXPAND_NAME);
    return;
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateName(name, this->getScope(), name_expression,
                                       requite::LogMode::QUIET)) [[unlikely]] {
    this->addYield(procedure, situation, requite::YieldState::EVALUATE_NAME);
    return;
  }
  requite::RootSymbol found = this->getScope().lookupUserSymbol(name);
  if (found.getIsNone()) {
    requite::NamedProcedureGroup &group =
        this->getContext().makeNamedProcedureGroup();
    group.setName(name);
    procedure.setNamedProcedureGroup(group);
    this->getScope().addUserSymbol(group);
  } else if (found.getIsNamedProcedureGroup()) {
    requite::NamedProcedureGroup &group = found.getNamedProcedureGroup();
    procedure.setNamedProcedureGroup(group);
  } else [[unlikely]] {
    this->getContext().logErrorAlreadySymbolOfName(expression);
    this->setNotOk();
  }
  if (!this->expandExpression(expression, situation, requite::LogMode::QUIET))
      [[unlikely]] {
    this->addYield(procedure, situation, requite::YieldState::EXPAND_ALL);
  }
  return;
}

void Contextualizer0::tabulateConstructor(requite::Expression &expression,
                                          requite::Situation situation,
                                          bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::CONSTRUCTOR);
  REQUITE_ASSERT(this->getScope().getType() == requite::ScopeType::OBJECT);
  requite::Procedure &procedure = this->getContext().makeProcedure();
  procedure.setType(requite::ProcedureType::CONSTRUCTOR);
  procedure.setModule(this->getModule());
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  this->getObject().addConstructor(procedure);
  if (has_attributes) {
    // TODO parse attributes
  }
  if (!this->expandExpression(expression, requite::Situation::MODULE_STATEMENT,
                              requite::LogMode::QUIET)) [[unlikely]] {
    this->addYield(procedure, situation, requite::YieldState::EXPAND_ALL);
  }
}

void Contextualizer0::tabulateDestructor(requite::Expression &expression,
                                         requite::Situation situation,
                                         bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::DESTRUCTOR);
  REQUITE_ASSERT(this->getScope().getType() == requite::ScopeType::OBJECT);
  requite::Procedure &procedure = this->getContext().makeProcedure();
  procedure.setType(requite::ProcedureType::DESTRUCTOR);
  procedure.setModule(this->getModule());
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  this->getObject().addDestructor(procedure);
  if (has_attributes) {
    // TODO parse attributes
  }
  if (!this->expandExpression(expression, requite::Situation::MODULE_STATEMENT,
                              requite::LogMode::QUIET)) [[unlikely]] {
    this->addYield(procedure, situation, requite::YieldState::EXPAND_ALL);
  }
}

void Contextualizer0::tabulateObject(requite::Expression &expression,
                                     requite::Situation situation,
                                     bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::DESTRUCTOR);
  REQUITE_ASSERT(this->getScope().getType() == requite::ScopeType::MODULE ||
                 this->getScope().getType() == requite::ScopeType::TABLE);
  requite::Object &object = this->getContext().makeObject();
  object.setExpression(expression);
  expression.setObject(object);
  if (has_attributes) {
    // TODO parse attributes
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateName(name, this->getScope(), name_expression,
                                       requite::LogMode::QUIET)) [[unlikely]] {
    this->addYield(object, situation, requite::YieldState::EVALUATE_NAME);
    return;
  }
  requite::RootSymbol found = this->getScope().lookupUserSymbol(name);
  if (found.getIsNone()) {
    this->getScope().addUserSymbol(object);
  } else [[unlikely]] {
    object.setContaining(this->getScope());
    this->getContext().logErrorAlreadySymbolOfName(expression);
    this->setNotOk();
  }
  this->enterScope(object.getScope());
  for (requite::Expression &branch : name_expression.getNextSubrange()) {
    this->tabulateStatement(branch, requite::Situation::OBJECT_STATEMENT,
                            false);
  }
  this->leaveScope();
}

bool Contextualizer0::tabulateAlias(requite::Expression &expression,
                                    bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ALIAS);
  // if (!this->expandMacroTreeNary<requite::Situation::OBJECT_STATEMENT, 2,
  //                                requite::Situation::SYMBOL_NAME,
  //                                requite::Situation::MATTE_SYMBOL,
  //                                requite::Situation::MATTE_LOCAL_STATEMENT>(
  //         expression)) {
  //   return requite::Contextualize0Result::YIELD;
  // }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateName(name, this->getScope(),
                                       name_expression)) {
    return false;
  }
  requite::RootSymbol found = this->getScope().lookupUserSymbol(name);
  if (!found.getIsNone()) {
    this->getContext().logErrorAlreadySymbolOfName(expression);
    return false;
  }
  requite::Alias &alias = this->getContext().makeAlias();
  alias.setName(name);
  alias.setExpression(expression);
  expression.setAlias(alias);
  this->getScope().addUserSymbol(alias);
  if (has_attributes) {
    // TODO tabulate attributes
  }
  return true;
}

bool Contextualizer0::tabulateImport(requite::Expression &expression,
                                     bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::IMPORT);
  // if (!this->expandMacroTreeNary<requite::Situation::OBJECT_STATEMENT, 2,
  //                                requite::Situation::SYMBOL_NAME,
  //                                requite::Situation::MATTE_SYMBOL,
  //                                requite::Situation::MATTE_LOCAL_STATEMENT>(
  //         expression)) {
  //   return requite::Contextualize0Result::YIELD;
  // }
  requite::Node &node = this->getScope().getNodes().emplace_back();
  node.setType(requite::NodeType::IMPORT);
  node.setExpression(expression);
  if (has_attributes) {
    // TODO tabulate attributes
  }
  return true;
}

bool Contextualizer0::tabulateUse(requite::Expression &expression,
                                  bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::USE);
  // if (!this->expandMacroTreeNary<requite::Situation::OBJECT_STATEMENT, 2,
  //                                requite::Situation::SYMBOL_NAME,
  //                                requite::Situation::MATTE_SYMBOL,
  //                                requite::Situation::MATTE_LOCAL_STATEMENT>(
  //         expression)) {
  //   return requite::Contextualize0Result::YIELD;
  // }
  requite::Node &node = this->getScope().getNodes().emplace_back();
  node.setType(requite::NodeType::USE);
  node.setExpression(expression);
  if (has_attributes) {
    // TODO tabulate attributes
  }
  return true;
}

bool Contextualizer0::tabulateGlobal(requite::Expression &expression,
                                     bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer0::tabulateProperty(requite::Expression &expression,
                                       bool has_attributes) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::tabulate_Local(requite::Local *&out_local_ptr,
                                     llvm::StringRef name,
                                     requite::Expression &statement) {
  REQUITE_ASSERT(statement.getOpcode() == requite::Opcode::_LOCAL);
  requite::Local &local = this->getContext().makeLocal();
  local.setExpression(statement);
  statement.setLocal(local);
  local.setName(name);
  this->getScope().addUserSymbol(local);
  out_local_ptr = &local;
  return true;
}

bool Contextualizer1::tabulateAlias(requite::Expression &expression) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::tabulateImport(requite::Expression &import) {
  REQUITE_UNREACHABLE(); // TODO
}

bool Contextualizer1::tabulateUse(requite::Expression &use) {
  REQUITE_UNREACHABLE(); // TODO
}

} // namespace requite
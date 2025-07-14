#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/local.hpp>
#include <requite/module.hpp>
#include <requite/scope.hpp>
#include <requite/situator.hpp>
#include <requite/tabulator.hpp>

namespace requite {

bool Context::tabulateModule(requite::Module &module) {
  requite::Tabulator tabulator(*this, module);
  if (!tabulator.tabulateModule()) {
    return false;
  }
  return true;
}

bool Tabulator::tabulateModule() {
  requite::Expression &root = this->getModule().getExpression();
  REQUITE_ASSERT(!root.getHasNext());
  requite::Expression &module_name = root.getBranch();
  for (requite::Expression &module_statement : module_name.getNextSubrange()) {
    this->tabulateStatement(module_statement, false);
  }
  return this->getIsOk();
}

void Tabulator::tabulateStatement(requite::Expression &statement,
                                  bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateStatement(statement.getBranch(), true);
    break;
  case requite::Opcode::ENTRY_POINT:
    this->tabulateEntryPoint(statement, has_attributes);
    break;
  case requite::Opcode::FUNCTION:
    this->tabulateFunction(statement, has_attributes);
    break;
  case requite::Opcode::METHOD:
    this->tabulateMethod(statement, has_attributes);
    break;
  case requite::Opcode::EXTENSION:
    this->tabulateExtension(statement, has_attributes);
    break;
  case requite::Opcode::CONSTRUCTOR:
    this->tabulateConstructor(statement, has_attributes);
    break;
  case requite::Opcode::DESTRUCTOR:
    this->tabulateDestructor(statement, has_attributes);
    break;
  case requite::Opcode::OBJECT:
    this->tabulateObject(statement, has_attributes);
    break;
  case requite::Opcode::ALIAS:
    this->tabulateAlias(statement, has_attributes);
    break;
  case requite::Opcode::IMPORT:
    this->tabulateImport(statement, has_attributes);
    break;
  case requite::Opcode::USE:
    this->tabulateUse(statement, has_attributes);
    break;
  case requite::Opcode::GLOBAL:
    this->tabulateGlobal(statement, has_attributes);
    break;
  case requite::Opcode::PROPERTY:
    this->tabulateProperty(statement, has_attributes);
    break;
  case requite::Opcode::_BLOCK:
    this->tabulateBlock(statement, has_attributes);
    break;
  default:
    REQUITE_UNREACHABLE();
  }
}

void Tabulator::tabulateEntryPoint(requite::Expression &expression,
                                   bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ENTRY_POINT);
  requite::Procedure &procedure = this->getContext().makeEntryPoint();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContaining(this->getScope());
  if (has_attributes) {
    this->getContext().logSourceMessage(expression, requite::LogType::ERROR,
                                        "entry point must not have attributes");
    this->setNotOk();
  }
  this->getModule().addEntryPoint(procedure);
}

void Tabulator::tabulateFunction(requite::Expression &expression,
                                 bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::FUNCTION);
  requite::Procedure &procedure = this->getContext().makeFunction();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContaining(this->getScope());
  if (has_attributes) {
    // TODO parse attributes
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, this->getScope(),
                                              name_expression)) [[unlikely]] {
    this->setNotOk();
    return;
  }
  procedure.setName(name);
  this->getScope().addUserSymbol(procedure);
}

void Tabulator::tabulateMethod(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::METHOD);
  requite::Procedure &procedure = this->getContext().makeMethod();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContaining(this->getScope());
  if (has_attributes) {
    // TODO parse attributes
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, this->getScope(),
                                              name_expression)) [[unlikely]] {
    this->setNotOk();
    return;
  }
  procedure.setName(name);
  this->getScope().addUserSymbol(procedure);
}

void Tabulator::tabulateExtension(requite::Expression &expression,
                                  bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::EXTENSION);
  requite::Procedure &procedure = this->getContext().makeExtension();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContaining(this->getScope());
  if (has_attributes) {
    // TODO parse attributes
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, this->getScope(),
                                              name_expression)) [[unlikely]] {
    this->setNotOk();
    return;
  }
  procedure.setName(name);
  this->getScope().addUserSymbol(procedure);
}

void Tabulator::tabulateConstructor(requite::Expression &expression,
                                    bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::CONSTRUCTOR);
  requite::Procedure &procedure = this->getContext().makeExtension();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContaining(this->getScope());
  if (has_attributes) {
    // TODO parse attributes
  }
  this->getObject().addConstructor(procedure);
}

void Tabulator::tabulateDestructor(requite::Expression &expression,
                                   bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::DESTRUCTOR);
  requite::Procedure &procedure = this->getContext().makeExtension();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContaining(this->getScope());
  if (has_attributes) {
    // TODO parse attributes
  }
  this->getObject().addDestructor(procedure);
}

void Tabulator::tabulateObject(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::OBJECT);
  requite::Object &object = this->getContext().makeObject();
  object.setExpression(expression);
  expression.setObject(object);
  object.setContaining(this->getScope());
  if (has_attributes) {
    // TODO parse attributes
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, this->getScope(),
                                              name_expression)) [[unlikely]] {
    this->setNotOk();
  } else {
    object.setName(name);
    this->getScope().addUserSymbol(object);
  }
  this->enterScope(object.getScope());
  for (requite::Expression &statement : name_expression.getNextSubrange()) {
    this->tabulateStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateAlias(requite::Expression &expression,
                              bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ALIAS);
  requite::Alias &alias = this->getContext().makeAlias();
  alias.setExpression(expression);
  expression.setAlias(alias);
  alias.setContaining(this->getScope());
  if (has_attributes) {
    // TODO parse attributes
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, this->getScope(),
                                              name_expression)) [[unlikely]] {
    this->setNotOk();
    return;
  }
  alias.setName(name);
  this->getScope().addUserSymbol(alias);
}

void Tabulator::tabulateImport(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::IMPORT);
  requite::Import &import = this->getContext().makeImport();
  import.setExpression(expression);
  expression.setImport(import);
  if (has_attributes) {
    // TODO parse attributes
  }
  this->getScope().addImport(import);
}

void Tabulator::tabulateUse(requite::Expression &expression,
                            bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::USE);
  requite::Use &use = this->getContext().makeUse();
  use.setExpression(expression);
  expression.setUse(use);
  if (has_attributes) {
    // TODO parse attributes
  }
  this->getScope().addUse(use);
}

void Tabulator::tabulateGlobal(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::GLOBAL);
  requite::Global &global = this->getContext().makeGlobal();
  global.setExpression(expression);
  expression.setGlobal(global);
  global.setContaining(this->getScope());
  if (has_attributes) {
    // TODO parse attributes
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, this->getScope(),
                                              name_expression)) [[unlikely]] {
    this->setNotOk();
    return;
  }
  global.setName(name);
  this->getScope().addUserSymbol(global);
}

void Tabulator::tabulateProperty(requite::Expression &expression,
                                 bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::PROPERTY);
  requite::Property &property = this->getContext().makeProperty();
  property.setExpression(expression);
  expression.setProperty(property);
  property.setContaining(this->getScope());
  if (has_attributes) {
    // TODO parse attributes
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, this->getScope(),
                                              name_expression)) [[unlikely]] {
    this->setNotOk();
    return;
  }
  property.setName(name);
  this->getScope().addUserSymbol(property);
}

void Tabulator::tabulateBlock(requite::Expression &expression,
                              bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::PROPERTY);
  requite::Block &block = this->getContext().makeBlock();
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContaining(this->getScope());
  if (has_attributes) {
    this->getContext().logErrorMustNotHaveAttributeFlags(expression);
    this->setNotOk();
  }
}

} // namespace requite
#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/label.hpp>
#include <requite/local.hpp>
#include <requite/module.hpp>
#include <requite/options.hpp>
#include <requite/scope.hpp>
#include <requite/situator.hpp>
#include <requite/table.hpp>
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
    this->tabulateModuleStatement(module_statement, false);
  }
  return this->getIsOk();
}

void Tabulator::tabulateModuleStatement(requite::Expression &statement,
                                        bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateModuleStatement(statement.getBranch(), true);
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
  case requite::Opcode::TABLE:
    this->tabulateTable(statement, has_attributes);
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
  default:
    break;
  }
}

void Tabulator::tabulateTableStatement(requite::Expression &statement,
                                       bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateTableStatement(statement.getBranch(), true);
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
  case requite::Opcode::OBJECT:
    this->tabulateObject(statement, has_attributes);
    break;
  case requite::Opcode::TABLE:
    this->tabulateTable(statement, has_attributes);
    break;
  case requite::Opcode::ALIAS:
    this->tabulateAlias(statement, has_attributes);
    break;
  case requite::Opcode::USE:
    this->tabulateUse(statement, has_attributes);
    break;
  case requite::Opcode::GLOBAL:
    this->tabulateGlobal(statement, has_attributes);
    break;
  default:
    break;
  }
}

void Tabulator::tabulateObjectStatement(requite::Expression &statement,
                                        bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateObjectStatement(statement.getBranch(), true);
    break;
  case requite::Opcode::FUNCTION:
    this->tabulateFunction(statement, has_attributes);
    break;
  case requite::Opcode::METHOD:
    this->tabulateMethod(statement, has_attributes);
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
  case requite::Opcode::USE:
    this->tabulateUse(statement, has_attributes);
    break;
  case requite::Opcode::GLOBAL:
    this->tabulateGlobal(statement, has_attributes);
    break;
  case requite::Opcode::PROPERTY:
    this->tabulateProperty(statement, has_attributes);
    break;
  default:
    break;
  }
}

void Tabulator::tabulateMatteLocalStatement(requite::Expression &statement,
                                            bool has_attributes) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH:
    REQUITE_ASSERT(!has_attributes);
    this->tabulateMatteLocalStatement(statement.getBranch(), true);
    break;
  case requite::Opcode::_LOCAL:
    this->tabulate_Local(statement, has_attributes);
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
  case requite::Opcode::OBJECT:
    this->tabulateObject(statement, has_attributes);
    break;
  case requite::Opcode::ALIAS:
    this->tabulateAlias(statement, has_attributes);
    break;
  case requite::Opcode::USE:
    this->tabulateUse(statement, has_attributes);
    break;
  case requite::Opcode::GLOBAL:
    this->tabulateGlobal(statement, has_attributes);
    break;
  case requite::Opcode::IF:
    this->tabulateIf(statement, has_attributes);
    break;
  case requite::Opcode::ELSE_IF:
    this->tabulateElseIf(statement, has_attributes);
    break;
  case requite::Opcode::ELSE:
    this->tabulateElse(statement, has_attributes);
    break;
  case requite::Opcode::SWITCH:
    this->tabulateSwitch(statement, has_attributes);
    break;
  case requite::Opcode::FOR:
    this->tabulateFor(statement, has_attributes);
    break;
  case requite::Opcode::WHILE:
    this->tabulateWhile(statement, has_attributes);
    break;
  case requite::Opcode::DO_WHILE:
    this->tabulateDoWhile(statement, has_attributes);
    break;
  case requite::Opcode::LOOP:
    this->tabulateLoop(statement, has_attributes);
    break;
  case requite::Opcode::SCOPE:
    this->tabulateScope(statement, has_attributes);
    break;
  default:
    break;
  }
}

void Tabulator::tabulateEntryPoint(requite::Expression &expression,
                                   bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ENTRY_POINT);
  requite::Procedure &procedure = this->getContext().makeEntryPoint();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContainingScope(this->getScope());
  if (has_attributes) {
    this->getContext().logErrorMustNotHaveAttributes(expression);
    this->setNotOk();
  }
  this->getModule().addEntryPoint(procedure);
  this->enterScope(procedure.getScope());
  for (requite::Expression &statement : expression.getBranchSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateFunction(requite::Expression &expression,
                                 bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::FUNCTION);
  requite::AttributeFlags attributes;
  if (has_attributes) {
    if (this->getScope().getType() == requite::ScopeType::OBJECT) {
      attributes =
          this->tabulateAttributes<requite::AttributeCategory::MEMBER_FUNCTION>(
              expression);
      if (this->getObject().getAttributeFlags().getHasAttribute(
              requite::AttributeType::EXPORT)) {
        attributes.addAttribute(requite::AttributeType::EXPORT);
      }
    } else {
      attributes =
          this->tabulateAttributes<requite::AttributeCategory::FUNCTION>(
              expression);
    }
  }
  if (attributes.getHasAttribute(requite::AttributeType::TEMPLATE)) {
    // TODO tabulate template function
    return;
  }
  requite::Procedure &procedure = this->getContext().makeFunction();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContainingScope(this->getScope());
  procedure.getAttributeFlags() = attributes;
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, name_expression))
      [[unlikely]] {
    this->setNotOk();
    return;
  }
  procedure.setName(name);
  this->getScope().addUserSymbol(procedure, this->getModule());
  requite::Expression &signature_expression = name_expression.getNext();
  this->enterScope(procedure.getScope());
  for (requite::Expression &statement :
       signature_expression.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateMethod(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::METHOD);
  requite::AttributeFlags attributes;
  if (has_attributes) {
    attributes =
        this->tabulateAttributes<requite::AttributeCategory::MEMBER_METHOD>(
            expression);
    if (this->getObject().getAttributeFlags().getHasAttribute(
            requite::AttributeType::EXPORT)) {
      attributes.addAttribute(requite::AttributeType::EXPORT);
    }
  }
  if (attributes.getHasAttribute(requite::AttributeType::TEMPLATE)) {
    // TODO tabulate template method
    return;
  }
  requite::Procedure &procedure = this->getContext().makeMethod();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContainingScope(this->getScope());
  procedure.getAttributeFlags() = attributes;
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, name_expression))
      [[unlikely]] {
    this->setNotOk();
    return;
  }
  procedure.setName(name);
  this->getScope().addUserSymbol(procedure, this->getModule());
  requite::Expression &signature_expression = name_expression.getNext();
  this->enterScope(procedure.getScope());
  for (requite::Expression &statement :
       signature_expression.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateExtension(requite::Expression &expression,
                                  bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::EXTENSION);
  requite::AttributeFlags attributes;
  if (has_attributes) {
    if (this->getScope().getType() == requite::ScopeType::OBJECT) {
      attributes = this->tabulateAttributes<
          requite::AttributeCategory::MEMBER_EXTENSION>(expression);
      if (this->getObject().getAttributeFlags().getHasAttribute(
              requite::AttributeType::EXPORT)) {
        attributes.addAttribute(requite::AttributeType::EXPORT);
      }
    } else {
      attributes =
          this->tabulateAttributes<requite::AttributeCategory::EXTENSION>(
              expression);
    }
  }
  if (attributes.getHasAttribute(requite::AttributeType::TEMPLATE)) {
    // TODO tabulate template extension
    return;
  }
  requite::Procedure &procedure = this->getContext().makeExtension();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContainingScope(this->getScope());
  procedure.getAttributeFlags() = attributes;
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, name_expression))
      [[unlikely]] {
    this->setNotOk();
    return;
  }
  procedure.setName(name);
  this->getScope().addUserSymbol(procedure, this->getModule());
  requite::Expression &signature_expression = name_expression.getNext();
  this->enterScope(procedure.getScope());
  for (requite::Expression &statement :
       signature_expression.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateConstructor(requite::Expression &expression,
                                    bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::CONSTRUCTOR);
  requite::AttributeFlags attributes;
  if (has_attributes) {
    attributes = this->tabulateAttributes<
        requite::AttributeCategory::MEMBER_CONSTRUCTOR>(expression);
    if (this->getObject().getAttributeFlags().getHasAttribute(
            requite::AttributeType::EXPORT)) {
      attributes.addAttribute(requite::AttributeType::EXPORT);
    }
  }
  if (attributes.getHasAttribute(requite::AttributeType::TEMPLATE)) {
    // TODO tabulate template constructor
    return;
  }
  requite::Procedure &procedure = this->getContext().makeExtension();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContainingScope(this->getScope());
  procedure.getAttributeFlags() = attributes;
  this->getObject().addConstructor(procedure);
  requite::Expression &signature_expression = expression.getBranch();
  this->enterScope(procedure.getScope());
  for (requite::Expression &statement :
       signature_expression.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateDestructor(requite::Expression &expression,
                                   bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::DESTRUCTOR);
  requite::AttributeFlags attributes;
  requite::Procedure &procedure = this->getContext().makeExtension();
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  procedure.setContainingScope(this->getScope());
  if (has_attributes) {
    procedure.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::MEMBER_DESTRUCTOR>(
            expression);
    if (this->getObject().getAttributeFlags().getHasAttribute(
            requite::AttributeType::EXPORT)) {
      procedure.getAttributeFlags().addAttribute(
          requite::AttributeType::EXPORT);
    }
  }
  this->getObject().addDestructor(procedure);
  this->enterScope(procedure.getScope());
  for (requite::Expression &statement : expression.getBranchSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateObject(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::OBJECT);
  requite::AttributeFlags attributes;
  if (has_attributes) {
    if (this->getScope().getType() == requite::ScopeType::OBJECT) {
      attributes =
          this->tabulateAttributes<requite::AttributeCategory::MEMBER_OBJECT>(
              expression);
      if (this->getObject().getAttributeFlags().getHasAttribute(
              requite::AttributeType::EXPORT)) {
        attributes.addAttribute(requite::AttributeType::EXPORT);
      }
    } else {
      attributes = this->tabulateAttributes<requite::AttributeCategory::OBJECT>(
          expression);
    }
  }
  if (attributes.getHasAttribute(requite::AttributeType::TEMPLATE)) {
    // TODO tabulate template object
    return;
  }
  requite::Object &object = this->getContext().makeObject();
  object.setExpression(expression);
  expression.setObject(object);
  object.setContainingScope(this->getScope());
  object.getAttributeFlags() = attributes;
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, name_expression))
      [[unlikely]] {
    this->setNotOk();
  } else {
    object.setName(name);
    this->getScope().addUserSymbol(object, this->getModule());
  }
  this->enterScope(object.getScope());
  for (requite::Expression &statement : name_expression.getNextSubrange()) {
    this->tabulateObjectStatement(statement, false);
  }
  this->leaveScope();
}

requite::Table &Tabulator::getOrMakeTable(llvm::StringRef name) {
  if (this->getScope().getHasTable(name)) {
    requite::LookupTableEntry &entry = this->getScope().lookupUserSymbol(name);
    return entry.getTable();
  }
  requite::Table &table = this->getContext().makeTable();
  table.setContainingScope(this->getScope());
  table.setName(name);
  this->getScope().addTable(table);
  return table;
}

void Tabulator::tabulateTable(requite::Expression &expression,
                              bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::TABLE);
  if (has_attributes) {
    this->getContext().logErrorMustNotHaveAttributes(expression);
    this->setNotOk();
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, name_expression))
      [[unlikely]] {
    this->setNotOk();
    for (requite::Expression &statement : name_expression.getNextSubrange()) {
      this->tabulateTableStatement(statement, false);
    }
    return;
  }
  requite::Table &table = this->getOrMakeTable(name);
  expression.setTable(table);
  this->enterScope(table.getScope());
  for (requite::Expression &statement : name_expression.getNextSubrange()) {
    this->tabulateTableStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateAlias(requite::Expression &expression,
                              bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ALIAS);
  requite::AttributeFlags attributes;
  if (has_attributes) {
    if (this->getScope().getType() == requite::ScopeType::OBJECT) {
      attributes =
          this->tabulateAttributes<requite::AttributeCategory::MEMBER_ALIAS>(
              expression);
      if (this->getObject().getAttributeFlags().getHasAttribute(
              requite::AttributeType::EXPORT)) {
        attributes.addAttribute(requite::AttributeType::EXPORT);
      }
    } else {
      attributes = this->tabulateAttributes<requite::AttributeCategory::ALIAS>(
          expression);
    }
  }
  if (attributes.getHasAttribute(requite::AttributeType::TEMPLATE)) {
    // TODO tabulate template alias
    return;
  }
  requite::Alias &alias = this->getContext().makeAlias();
  alias.setExpression(expression);
  expression.setAlias(alias);
  alias.setContainingScope(this->getScope());
  alias.getAttributeFlags() = attributes;
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, name_expression))
      [[unlikely]] {
    this->setNotOk();
    return;
  }
  alias.setName(name);
  this->getScope().addUserSymbol(alias, this->getModule());
}

void Tabulator::tabulateImport(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::IMPORT);
  requite::Import &import = this->getContext().makeImport();
  import.setExpression(expression);
  expression.setImport(import);
  import.setContainingScope(this->getScope());
  import.setContainingModule(this->getModule());
  if (has_attributes) {
    import.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::IMPORT>(
            expression);
  }
  if (!this->getContext().importModule(import)) {
    this->setNotOk();
  } else {
    this->getModule().addImport(import);
  }
}

void Tabulator::tabulateUse(requite::Expression &expression,
                            bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::USE);
  requite::Use &use = this->getContext().makeUse();
  use.setExpression(expression);
  expression.setUse(use);
  use.setContainingScope(this->getScope());
  use.setContainingModule(this->getModule());
  if (has_attributes) {
    if (this->getScope().getType() == requite::ScopeType::OBJECT) {
      use.getAttributeFlags() =
          this->tabulateAttributes<requite::AttributeCategory::MEMBER_USE>(
              expression);
      if (this->getObject().getAttributeFlags().getHasAttribute(
              requite::AttributeType::EXPORT)) {
        use.getAttributeFlags().addAttribute(requite::AttributeType::EXPORT);
      }
    } else {
      use.getAttributeFlags() =
          this->tabulateAttributes<requite::AttributeCategory::USE>(expression);
    }
  }
}

void Tabulator::tabulateGlobal(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::GLOBAL);
  requite::AttributeFlags attributes;
  if (has_attributes) {
    if (this->getScope().getType() == requite::ScopeType::OBJECT) {
      attributes =
          this->tabulateAttributes<requite::AttributeCategory::MEMBER_GLOBAL>(
              expression);
      if (this->getObject().getAttributeFlags().getHasAttribute(
              requite::AttributeType::EXPORT)) {
        attributes.addAttribute(requite::AttributeType::EXPORT);
      }
    } else {
      attributes = this->tabulateAttributes<requite::AttributeCategory::GLOBAL>(
          expression);
    }
  }
  if (attributes.getHasAttribute(requite::AttributeType::TEMPLATE)) {
    // TODO tabulate template global
    return;
  }
  requite::Global &global = this->getContext().makeGlobal();
  global.setExpression(expression);
  expression.setGlobal(global);
  global.setContainingScope(this->getScope());
  global.getAttributeFlags() = attributes;
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, name_expression))
      [[unlikely]] {
    this->setNotOk();
    return;
  }
  global.setName(name);
  this->getScope().addUserSymbol(global, this->getModule());
}

void Tabulator::tabulate_Local(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_LOCAL);
  if (!requite::getCanHaveLocal(this->getScope().getType())) {
    this->getContext().logSourceMessage(expression, requite::LogType::ERROR,
                                        "local can not exist in scope");
    this->setNotOk();
  }
  requite::Local &local = this->getContext().makeLocal();
  local.setExpression(expression);
  expression.setLocal(local);
  local.setContainingScope(this->getScope());
  if (has_attributes) {
    this->getContext().logErrorMustNotHaveAttributes(expression);
    this->setNotOk();
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, name_expression))
      [[unlikely]] {
    this->setNotOk();
    return;
  }
  local.setName(name);
  this->getScope().addUserSymbol(local, this->getModule());
}

void Tabulator::tabulate_AnonymousFunction(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() ==
                 requite::Opcode::_ANONYMOUS_FUNCTION);
  requite::AnonymousFunction &function =
      this->getContext().makeAnonymousFunction();
  function.setExpression(expression);
  expression.setAnonymousFunction(function);
  function.setContainingScope(this->getScope());
  requite::Expression &capture = expression.getBranch();
  requite::Expression &signature = capture.getNext();
  this->enterScope(function.getScope());
  for (requite::Expression &statement : signature.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateProperty(requite::Expression &expression,
                                 bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::PROPERTY);
  requite::Property &property = this->getContext().makeProperty();
  property.setExpression(expression);
  expression.setProperty(property);
  property.setContainingScope(this->getScope());
  if (has_attributes) {
    property.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::MEMBER_PROPERTY>(
            expression);
    if (this->getObject().getAttributeFlags().getHasAttribute(
            requite::AttributeType::EXPORT)) {
      property.getAttributeFlags().addAttribute(requite::AttributeType::EXPORT);
    }
  }
  requite::Expression &name_expression = expression.getBranch();
  llvm::StringRef name;
  if (!this->getContext().evaluateInstantName(name, name_expression))
      [[unlikely]] {
    this->setNotOk();
    return;
  }
  property.setName(name);
  this->getScope().addUserSymbol(property, this->getModule());
  requite::Expression &value_expression = name_expression.getNext();
  this->tabulateExpression(value_expression);
}

void Tabulator::tabulateIf(requite::Expression &expression,
                           bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::IF);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::IF);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  if (has_attributes) {
    block.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::BLOCK>(expression);
  }
  this->enterScope(block.getScope());
  requite::Expression &condition_expression = expression.getBranch();
  this->tabulateExpression(condition_expression);
  for (requite::Expression &statement :
       condition_expression.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateElseIf(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ELSE_IF);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::ELSE_IF);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  if (has_attributes) {
    block.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::BLOCK>(expression);
  }
  this->enterScope(block.getScope());
  requite::Expression &condition_expression = expression.getBranch();
  this->tabulateExpression(condition_expression);
  for (requite::Expression &statement :
       condition_expression.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateElse(requite::Expression &expression,
                             bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ELSE);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::ELSE);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  if (has_attributes) {
    block.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::BLOCK>(expression);
  }
  this->enterScope(block.getScope());
  for (requite::Expression &statement : expression.getBranchSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateSwitch(requite::Expression &expression,
                               bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::SWITCH);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::SWITCH);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  if (has_attributes) {
    block.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::BLOCK>(expression);
  }
  this->enterScope(block.getScope());
  requite::Expression &value_expression = expression.getBranch();
  this->tabulateExpression(value_expression);
  for (requite::Expression &statement : value_expression.getNextSubrange()) {
    this->tabulateSwitchCaseStatement(statement);
  }
  this->leaveScope();
}

void Tabulator::tabulateSwitchCaseStatement(requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH: {
    requite::Expression &branch = expression.getBranch();
    this->getContext().logErrorMustNotHaveAttributes(branch);
    this->tabulateSwitchCaseStatement(branch);
  } break;
  case requite::Opcode::CASE:
    this->tabulateCase(expression);
    break;
  case requite::Opcode::DEFAULT_CASE:
    this->tabulateDefaultCase(expression);
    break;
  default:
    REQUITE_UNREACHABLE();
  }
}

void Tabulator::tabulateCase(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::CASE);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::CASE);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  this->enterScope(block.getScope());
  requite::Expression &value_expression = expression.getBranch();
  this->tabulateExpression(value_expression);
  for (requite::Expression &statement : value_expression.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateDefaultCase(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::DEFAULT_CASE);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::DEFAULT_CASE);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  this->enterScope(block.getScope());
  for (requite::Expression &statement : expression.getBranchSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateFor(requite::Expression &expression,
                            bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::FOR);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::FOR);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  if (has_attributes) {
    block.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::BLOCK>(expression);
  }
  this->enterScope(block.getScope());
  requite::Expression &value_expression = expression.getBranch();
  this->tabulateExpression(value_expression);
  for (requite::Expression &statement : value_expression.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateWhile(requite::Expression &expression,
                              bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::WHILE);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::WHILE);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  if (has_attributes) {
    block.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::BLOCK>(expression);
  }
  this->enterScope(block.getScope());
  requite::Expression &value_expression = expression.getBranch();
  this->tabulateExpression(value_expression);
  for (requite::Expression &statement : value_expression.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateDoWhile(requite::Expression &expression,
                                bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::DO_WHILE);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::DO_WHILE);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  if (has_attributes) {
    block.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::BLOCK>(expression);
  }
  this->enterScope(block.getScope());
  requite::Expression &value_expression = expression.getBranch();
  this->tabulateExpression(value_expression);
  for (requite::Expression &statement : value_expression.getNextSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateLoop(requite::Expression &expression,
                             bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::LOOP);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::LOOP);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  if (has_attributes) {
    block.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::BLOCK>(expression);
  }
  this->enterScope(block.getScope());
  for (requite::Expression &statement : expression.getBranchSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateScope(requite::Expression &expression,
                              bool has_attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::SCOPE);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::SCOPE);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  if (has_attributes) {
    block.getAttributeFlags() =
        this->tabulateAttributes<requite::AttributeCategory::BLOCK>(expression);
  }
  this->enterScope(block.getScope());
  for (requite::Expression &statement : expression.getBranchSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulate_ClosedInlineScope(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() ==
                 requite::Opcode::_CLOSED_INLINE_SCOPE);
  requite::Block &block = this->getContext().makeBlock();
  block.setType(requite::BlockType::CLOSED_INLINE_SCOPE);
  block.setExpression(expression);
  expression.setBlock(block);
  block.setContainingScope(this->getScope());
  this->enterScope(block.getScope());
  for (requite::Expression &statement : expression.getBranchSubrange()) {
    this->tabulateMatteLocalStatement(statement, false);
  }
  this->leaveScope();
}

void Tabulator::tabulateExpressionForest(requite::Expression &expression) {
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    this->tabulateExpression(branch);
  }
  for (requite::Expression &next : expression.getNextSubrange()) {
    this->tabulateExpression(next);
  }
}

void Tabulator::tabulateExpression(requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH: {
    requite::Expression &branch = expression.getBranch();
    this->getContext().logErrorMustNotHaveAttributes(branch);
    this->tabulateExpression(branch);
  } break;
  case requite::Opcode::_LOCAL:
    this->tabulate_Local(expression, false);
    break;
  case requite::Opcode::_ANONYMOUS_FUNCTION:
    this->tabulate_AnonymousFunction(expression);
    break;
  case requite::Opcode::_CLOSED_INLINE_SCOPE:
    this->tabulate_ClosedInlineScope(expression);
    break;
  default:
    if (!expression.getHasBranch()) {
      return;
    }
    requite::Expression &branch = expression.getBranch();
    this->tabulateExpressionForest(branch);
  }
}

} // namespace requite
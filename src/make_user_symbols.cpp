// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/maker.hpp>

namespace requite {

bool Context::makeUserSymbols(requite::Module &module) {
  requite::Maker maker(*this, module);
  maker.makeUserSymbols();
  const bool is_ok = maker.getIsOk();
  return is_ok;
}

void Maker::makeUserSymbols() {
  requite::Scope &scope = this->getModule().getScope();
  requite::Expression &root = this->getModule().getExpression();
  REQUITE_ASSERT(!root.getHasNext());
  requite::Expression &module_name_expression = root.getBranch();
  if (!module_name_expression.getHasNext()) {
    return;
  }
  requite::Expression &body = module_name_expression.getNext();
  this->makeUnorderedUserSymbols(scope, body, true);
}

void Maker::makeUnorderedUserSymbols(requite::Scope &scope,
                                     requite::Expression &body,
                                     bool conduits_have_scopes) {
  for (requite::Expression &branch : body.getHorizontalSubrange()) {
    if (branch.getOpcode() == requite::Opcode::_ASCRIBE_FIRST_BRANCH) {
      requite::Expression& symbol_expression = branch.getBranch();
      requite::MakeAttributesResult result =
          requite::Attributes::makeAttributes(this->getContext(), symbol_expression.getNext());
      if (result.has_error) {
        this->setNotOk();
      }
      this->makeAscribedUnorderedUserSymbol(
          scope, symbol_expression, result.attributes,
          conduits_have_scopes);
      continue;
    }
    this->makeUnorderedUserSymbol(scope, branch, conduits_have_scopes);
  }
}

void Maker::makeAscribedUnorderedUserSymbol(requite::Scope &scope,
                                            requite::Expression &expression,
                                            requite::Attributes attributes,
                                            bool conduits_have_scopes) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::ALIAS: {
    requite::Alias &alias = this->getModule().makeAlias();
    alias.setExpression(expression);
    expression.setAlias(alias);
    alias.setContaining(scope);
    alias.setAttributes(attributes);
    this->makeScopedValues(scope, expression.getBranch(), true);
  } break;
  case requite::Opcode::OBJECT: {
    requite::Object &object = this->getModule().makeObject();
    object.setExpression(expression);
    expression.setObject(object);
    object.setContaining(scope);
    object.setAttributes(attributes);
    this->makeUnorderedUserSymbols(object.getScope(), expression.getBranch(),
                                   conduits_have_scopes);
  } break;
  case requite::Opcode::GLOBAL: {
    requite::Variable &global = this->getModule().makeVariable();
    global.setType(requite::VariableType::GLOBAL);
    global.setExpression(expression);
    expression.setVariable(global);
    global.setContaining(scope);
    global.setAttributes(attributes);
    this->makeScopedValues(scope, expression.getBranch(), conduits_have_scopes);
  } break;
  case requite::Opcode::PROPERTY: {
    requite::Variable &property = this->getModule().makeVariable();
    property.setType(requite::VariableType::PROPERTY);
    property.setExpression(expression);
    expression.setVariable(property);
    property.setContaining(scope);
    property.setAttributes(attributes);
    this->makeScopedValues(scope, expression.getBranch(), conduits_have_scopes);
  } break;
  case requite::Opcode::CONSTANT: {
    requite::Variable &constant = this->getModule().makeVariable();
    constant.setType(requite::VariableType::CONSTANT);
    constant.setExpression(expression);
    expression.setVariable(constant);
    constant.setContaining(scope);
    constant.setAttributes(attributes);
    this->makeScopedValues(scope, expression.getBranch(), conduits_have_scopes);
  } break;
  case requite::Opcode::ENTRY_POINT: {
    this->getContext().logSourceMessage(expression, requite::LogType::ERROR,
                                        "attributes ascribed to entry_point");
    this->setNotOk();
  } break;
  case requite::Opcode::METHOD: {
    requite::Procedure &method = this->getModule().makeProcedure();
    method.setType(requite::ProcedureType::METHOD);
    method.setExpression(expression);
    expression.setProcedure(method);
    method.setContaining(scope);
    method.setAttributes(attributes);
    this->makeOrderedUserSymbols(method.getScope(), expression.getBranch());
  } break;
  case requite::Opcode::FUNCTION: {
    requite::Procedure &function = this->getModule().makeProcedure();
    function.setType(requite::ProcedureType::FUNCTION);
    function.setExpression(expression);
    expression.setProcedure(function);
    function.setContaining(scope);
    function.setAttributes(attributes);
    this->makeOrderedUserSymbols(function.getScope(), expression.getBranch());
  } break;
  case requite::Opcode::CONSTRUCTOR: {
    requite::Procedure &constructor = this->getModule().makeProcedure();
    constructor.setType(requite::ProcedureType::CONSTRUCTOR);
    constructor.setExpression(expression);
    expression.setProcedure(constructor);
    constructor.setContaining(scope);
    constructor.setAttributes(attributes);
    this->makeOrderedUserSymbols(constructor.getScope(),
                                 expression.getBranch());
  } break;
  case requite::Opcode::DESTRUCTOR: {
    requite::Procedure &destructor = this->getModule().makeProcedure();
    destructor.setType(requite::ProcedureType::DESTRUCTOR);
    destructor.setExpression(expression);
    expression.setProcedure(destructor);
    destructor.setContaining(scope);
    destructor.setAttributes(attributes);
    this->makeOrderedUserSymbols(destructor.getScope(), expression.getBranch());
  } break;
  case requite::Opcode::TABLE: {
    this->getContext().logSourceMessage(expression, requite::LogType::ERROR,
                                        "attributes ascribed to table");
    this->setNotOk();
  } break;
  case requite::Opcode::IMPORT: {
    requite::Node &import = scope.getNodes().emplace_back();
    import.setType(requite::NodeType::IMPORT);
    import.setExpression(expression);
    import.setAttributes(attributes);
  } break;
  case requite::Opcode::USE: {
    requite::Node &use = scope.getNodes().emplace_back();
    use.setType(requite::NodeType::USE);
    use.setExpression(expression);
    use.setAttributes(attributes);
  } break;
  default:
    this->makeScopedValues(scope, expression, false);
  }
}

void Maker::makeUnorderedUserSymbol(requite::Scope &scope,
                                    requite::Expression &expression,
                                    bool conduits_have_scopes) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::ALIAS: {
    requite::Alias &alias = this->getModule().makeAlias();
    alias.setExpression(expression);
    expression.setAlias(alias);
    alias.setContaining(scope);
    this->makeScopedValues(scope, expression.getBranch(), true);
  } break;
  case requite::Opcode::OBJECT: {
    requite::Object &object = this->getModule().makeObject();
    object.setExpression(expression);
    expression.setObject(object);
    object.setContaining(scope);
    this->makeUnorderedUserSymbols(object.getScope(), expression.getBranch(),
                                   conduits_have_scopes);
  } break;
  case requite::Opcode::GLOBAL: {
    requite::Variable &global = this->getModule().makeVariable();
    global.setType(requite::VariableType::GLOBAL);
    global.setExpression(expression);
    expression.setVariable(global);
    global.setContaining(scope);
    this->makeScopedValues(scope, expression.getBranch(), conduits_have_scopes);
  } break;
  case requite::Opcode::PROPERTY: {
    requite::Variable &property = this->getModule().makeVariable();
    property.setType(requite::VariableType::PROPERTY);
    property.setExpression(expression);
    expression.setVariable(property);
    property.setContaining(scope);
    this->makeScopedValues(scope, expression.getBranch(), conduits_have_scopes);
  } break;
  case requite::Opcode::CONSTANT: {
    requite::Variable &constant = this->getModule().makeVariable();
    constant.setType(requite::VariableType::CONSTANT);
    constant.setExpression(expression);
    expression.setVariable(constant);
    constant.setContaining(scope);
    this->makeScopedValues(scope, expression.getBranch(), conduits_have_scopes);
  } break;
  case requite::Opcode::ENTRY_POINT: {
    requite::Procedure &entry_point = this->getModule().makeProcedure();
    entry_point.setType(requite::ProcedureType::ENTRY_POINT);
    entry_point.setExpression(expression);
    expression.setProcedure(entry_point);
    entry_point.setContaining(scope);
    this->makeOrderedUserSymbols(entry_point.getScope(),
                                 expression.getBranch());
  } break;
  case requite::Opcode::METHOD: {
    requite::Procedure &method = this->getModule().makeProcedure();
    method.setType(requite::ProcedureType::METHOD);
    method.setExpression(expression);
    expression.setProcedure(method);
    method.setContaining(scope);
    this->makeOrderedUserSymbols(method.getScope(), expression.getBranch());
  } break;
  case requite::Opcode::FUNCTION: {
    requite::Procedure &function = this->getModule().makeProcedure();
    function.setType(requite::ProcedureType::FUNCTION);
    function.setExpression(expression);
    expression.setProcedure(function);
    function.setContaining(scope);
    this->makeOrderedUserSymbols(function.getScope(), expression.getBranch());
  } break;
  case requite::Opcode::CONSTRUCTOR: {
    requite::Procedure &constructor = this->getModule().makeProcedure();
    constructor.setType(requite::ProcedureType::CONSTRUCTOR);
    constructor.setExpression(expression);
    expression.setProcedure(constructor);
    constructor.setContaining(scope);
    this->makeOrderedUserSymbols(constructor.getScope(),
                                 expression.getBranch());
  } break;
  case requite::Opcode::DESTRUCTOR: {
    requite::Procedure &destructor = this->getModule().makeProcedure();
    destructor.setType(requite::ProcedureType::DESTRUCTOR);
    destructor.setExpression(expression);
    expression.setProcedure(destructor);
    destructor.setContaining(scope);
    this->makeOrderedUserSymbols(destructor.getScope(), expression.getBranch());
  } break;
  case requite::Opcode::TABLE: {
    requite::Scope &new_scope = this->getModule().makeScope();
    new_scope.setType(requite::ScopeType::TABLE);
    new_scope.setExpression(expression);
    expression.setScope(new_scope);
    new_scope.setContaining(scope);
    this->makeUnorderedUserSymbols(new_scope, expression.getBranch(),
                                   conduits_have_scopes);
  } break;
  case requite::Opcode::IMPORT: {
    requite::Node &import = scope.getNodes().emplace_back();
    import.setType(requite::NodeType::IMPORT);
    import.setExpression(expression);
  } break;
  case requite::Opcode::USE: {
    requite::Node &use = scope.getNodes().emplace_back();
    use.setType(requite::NodeType::USE);
    use.setExpression(expression);
  } break;
  default:
    this->makeScopedValues(scope, expression, false);
  }
}

void Maker::makeOrderedUserSymbols(requite::Scope &scope,
                                   requite::Expression &body) {
  for (requite::Expression &branch : body.getHorizontalSubrange()) {
    if (branch.getOpcode() == requite::Opcode::_ASCRIBE_FIRST_BRANCH) {
      requite::Expression& symbol_expression = branch.getBranch();
      requite::MakeAttributesResult result =
          requite::Attributes::makeAttributes(this->getContext(), symbol_expression.getNext());
      if (result.has_error) {
        this->setNotOk();
      }
      if (result.attributes.getHasAttribute(requite::AttributeType::LABEL)) {
        for (requite::Expression &attribute_expression :
             branch.getBranchSubrange()) {
          if (attribute_expression.getOpcode() != requite::Opcode::LABEL) {
            continue;
          }
          requite::Label &label = this->getModule().makeLabel();
          label.setAttributeExpression(attribute_expression);
          attribute_expression.setLabel(label);
          label.setStatementExpression(branch);
          label.setContaining(scope);
        }
      }
      this->makeAscribedOrderedUserSymbol(scope, result.attributes,
                                          symbol_expression);
      continue;
    }
    this->makeOrderedUserSymbol(scope, branch);
  }
}

void Maker::makeAscribedOrderedUserSymbol(requite::Scope &scope,
                                          requite::Attributes attributes,
                                          requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_LOCAL: {
    requite::Variable &local = this->getModule().makeVariable();
    local.setType(requite::VariableType::LOCAL);
    local.setExpression(expression);
    expression.setVariable(local);
    local.setContaining(scope);
    local.setAttributes(attributes);
    this->makeScopedValues(scope, expression.getBranch(), false);
  };
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
  case requite::Opcode::SCOPE: {
    this->getContext().logSourceMessage(expression, requite::LogType::ERROR,
                                        "attributes ascribed to scope");
    this->setNotOk();
  } break;
  default:
    this->makeAscribedUnorderedUserSymbol(scope, expression, attributes, false);
  }
}

void Maker::makeOrderedUserSymbol(requite::Scope &scope,
                                  requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_LOCAL: {
    requite::Variable &local = this->getModule().makeVariable();
    local.setType(requite::VariableType::LOCAL);
    local.setExpression(expression);
    expression.setVariable(local);
    local.setContaining(scope);
    this->makeScopedValues(scope, expression.getBranch(), false);
  } break;
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
  case requite::Opcode::SCOPE: {
    requite::Scope &new_scope = this->getModule().makeScope();
    new_scope.setType(requite::ScopeType::LOCAL_STATEMENT);
    expression.setScope(new_scope);
    new_scope.setExpression(expression);
    new_scope.setContaining(scope);
  } break;
  default:
    this->makeUnorderedUserSymbol(scope, expression, false);
  }
}

void Maker::makeScopedValues(requite::Scope &scope,
                             requite::Expression &expression,
                             bool conduits_have_scopes) {
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    switch (const requite::Opcode opcode = branch.getOpcode()) {
    case requite::Opcode::_ANONYMOUS_FUNCTION: {
      requite::AnonymousFunction &anonymous_function =
          this->getModule().makeAnonymousFunction();
      anonymous_function.setExpression(expression);
      expression.setAnonymousFunction(anonymous_function);
      anonymous_function.setContaining(scope);
      requite::Expression &capture = expression.getBranch();
      REQUITE_ASSERT(capture.getOpcode() == requite::Opcode::_CAPTURE);
      requite::Expression &signature = capture.getNext();
      requite::Expression &body = signature.getNext();
      requite::Scope &scope = anonymous_function.getScope();
      this->makeScopedValues(scope, body, false);
    } break;
    default:
      this->makeScopedValues(scope, branch, conduits_have_scopes);
    }
  }
}

} // namespace requite
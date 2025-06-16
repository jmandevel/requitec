#include <requite/context.hpp>

namespace requite {

bool Context::tabulateUserSymbols(requite::Module &module) {
  requite::Scope &scope = module.getScope();
  requite::Expression &root = module.getExpression();
  bool is_ok = true;
  for (requite::Expression &branch : root.getBranchSubrange()) {
    if (!this->tabulateBaseUserSymbol(module, scope, branch,
                                      requite::AttributeFlags())) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::tabulateBaseUserSymbol(requite::Module &module,
                                     requite::Scope &scope,
                                     requite::Expression &expression,
                                     requite::AttributeFlags attributes) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH: {
    REQUITE_ASSERT(!attributes.getHasAnyAttribute());
    // TODO
  }
  case requite::Opcode::ENTRY_POINT: {
    const bool attributes_ok = !attributes.getHasAnyAttribute();
    if (!attributes_ok) {
      this->logErrorMustNotHaveAttributeFlags(expression);
    }
    return this->tabulateEntryPoint(module, scope, expression) && attributes_ok;
  }
  case requite::Opcode::IMPORT:
    return this->tabulateImport(module, scope, expression, attributes);
  case requite::Opcode::USE:
    return this->tabulateUse(module, scope, expression, attributes);
  case requite::Opcode::TABLE: {
    const bool attributes_ok = !attributes.getHasAnyAttribute();
    if (!attributes_ok) {
      this->logErrorMustNotHaveAttributeFlags(expression);
    }
    return this->tabulateTable(module, scope, expression) && attributes_ok;
  }
  case requite::Opcode::OBJECT:
    return this->tabulateObject(module, scope, expression, attributes);
  case requite::Opcode::ALIAS:
    return this->tabulateAlias(module, scope, expression, attributes);
  case requite::Opcode::_UNORDERED_GLOBAL:
    return this->tabulateUnorderedGlobal(module, scope, expression, attributes);
  case requite::Opcode::CONSTANT:
    return this->tabulateConstant(module, scope, expression, attributes);
  case requite::Opcode::FUNCTION:
    return this->tabulateFunction(module, scope, expression, attributes);
  default:
    break;
  }
  return !attributes.getHasAnyAttribute();
}

bool Context::tabulateTableUserSymbol(requite::Module &module,
                                       requite::Scope &scope,
                                       requite::Expression &expression,
                                       requite::AttributeFlags attributes) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH: {
    REQUITE_ASSERT(!attributes.getHasAnyAttribute());
    // TODO
  }
  case requite::Opcode::IMPORT:
    return this->tabulateImport(module, scope, expression, attributes);
  case requite::Opcode::USE:
    return this->tabulateUse(module, scope, expression, attributes);
  case requite::Opcode::TABLE: {
    const bool attributes_ok = !attributes.getHasAnyAttribute();
    if (!attributes_ok) {
      this->logErrorMustNotHaveAttributeFlags(expression);
    }
    return this->tabulateTable(module, scope, expression) && attributes_ok;
  }
  case requite::Opcode::OBJECT:
    return this->tabulateObject(module, scope, expression, attributes);
  case requite::Opcode::ALIAS:
    return this->tabulateAlias(module, scope, expression, attributes);
  case requite::Opcode::_UNORDERED_GLOBAL:
    return this->tabulateUnorderedGlobal(module, scope, expression, attributes);
  case requite::Opcode::CONSTANT:
    return this->tabulateConstant(module, scope, expression, attributes);
  case requite::Opcode::FUNCTION:
    return this->tabulateFunction(module, scope, expression, attributes);
  default:
    break;
  }
  return !attributes.getHasAnyAttribute();
}
bool Context::tabulateMemberUserSymbol(requite::Module &module,
                                       requite::Scope &scope,
                                       requite::Expression &expression,
                                       requite::AttributeFlags attributes) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH: {
    REQUITE_ASSERT(!attributes.getHasAnyAttribute());
    requite::Expression &unascribed = expression.getBranch();
    for (requite::Expression &attribute : unascribed.getNextSubrange()) {
      requite::AttributeType type =
          requite::getAttributeType(attribute.getOpcode());
      REQUITE_ASSERT(type != requite::AttributeType::NONE);
    }
  }
  case requite::Opcode::PROPERTY:
    return this->tabulateProperty(module, scope, expression, attributes);
  case requite::Opcode::CONSTRUCTOR:
    return this->tabulateConstructor(module, scope, expression, attributes);
  case requite::Opcode::DESTRUCTOR:
    return this->tabulateDestructor(module, scope, expression, attributes);
  case requite::Opcode::METHOD:
    return this->tabulateMethod(module, scope, expression, attributes);
  case requite::Opcode::IMPORT:
    return this->tabulateImport(module, scope, expression, attributes);
  case requite::Opcode::USE:
    return this->tabulateUse(module, scope, expression, attributes);
  case requite::Opcode::OBJECT:
    return this->tabulateObject(module, scope, expression, attributes);
  case requite::Opcode::ALIAS:
    return this->tabulateAlias(module, scope, expression, attributes);
  case requite::Opcode::_UNORDERED_GLOBAL:
    return this->tabulateUnorderedGlobal(module, scope, expression, attributes);
  case requite::Opcode::CONSTANT:
    return this->tabulateConstant(module, scope, expression, attributes);
  case requite::Opcode::FUNCTION:
    return this->tabulateFunction(module, scope, expression, attributes);
  default:
    break;
  }
  return !attributes.getHasAnyAttribute();
}

bool Context::tabulateLocalUserSymbol(requite::Module &module,
                                      requite::Scope &scope,
                                      requite::Expression &expression,
                                      requite::AttributeFlags attributes) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH: {
    REQUITE_ASSERT(!attributes.getHasAnyAttribute());
    requite::Expression &unascribed = expression.getBranch();
    bool is_ok = true;
    for (requite::Expression &attribute : unascribed.getNextSubrange()) {
      if (attribute.getOpcode() == requite::Opcode::LABEL) {
        requite::Label &label = module.makeLabel();
        label.setAttributeExpression(attribute);
        label.setStatementExpression(unascribed);
        label.setContaining(scope);
        requite::Expression &name_expression = attribute.getBranch();
        if (name_expression.getOpcode() !=
            requite::Opcode::__IDENTIFIER_LITERAL) {
          this->logErrorNonInstantEvaluatableName(name_expression);
          is_ok = false;
        } else {
          llvm::StringRef name = name_expression.getSourceText();
          if (scope.getHasSymbolOfName(name)) {
            this->logErrorAlreadySymbolOfName(name_expression);
            is_ok = false;
          } else {
            label.setName(name);
            scope.addSymbol(label);
          }
        }
        continue;
      }
      requite::AttributeType type =
          requite::getAttributeType(attribute.getOpcode());
      REQUITE_ASSERT(type != requite::AttributeType::NONE);
      if (type != requite::AttributeType::USER &&
          attributes.getHasAttribute(type)) {
        this->logSourceMessage(attribute, requite::LogType::ERROR,
                               llvm::Twine(requite::getName(type)) +
                                   " attribute is ascribed more than once");
        is_ok = false;
        continue;
      }
      attributes.addAttribute(type);
    }
    return this->tabulateLocalUserSymbol(module, scope, unascribed,
                                         attributes) &&
           is_ok;
  }
  case requite::Opcode::_LOCAL: {
    const bool attributes_ok = !attributes.getHasAnyAttribute();
    return this->tabulateLocal(module, scope, expression) && attributes_ok;
  }
  case requite::Opcode::IF:
    [[fallthrough]];
  case requite::Opcode::ELSE_IF:
    [[fallthrough]];
  case requite::Opcode::ELSE:
    [[fallthrough]];
  case requite::Opcode::SWITCH:
    [[fallthrough]];
  case requite::Opcode::DO_WHILE:
    [[fallthrough]];
  case requite::Opcode::FOR_EACH:
    [[fallthrough]];
  case requite::Opcode::LOOP:
    [[fallthrough]];
  case requite::Opcode::SCOPE: {
    const bool attributes_ok = !attributes.getHasAnyAttribute();
    return this->tabulateLocalStatementScope(module, scope, expression) &&
           attributes_ok;
  }
  case requite::Opcode::IMPORT:
    return this->tabulateImport(module, scope, expression, attributes);
  case requite::Opcode::USE:
    return this->tabulateUse(module, scope, expression, attributes);
  case requite::Opcode::OBJECT:
    return this->tabulateObject(module, scope, expression, attributes);
  case requite::Opcode::ALIAS:
    return this->tabulateAlias(module, scope, expression, attributes);
  case requite::Opcode::_ORDERED_GLOBAL: {
    const bool attributes_ok = !attributes.getHasAnyAttribute();
    if (!attributes_ok) {
      this->logErrorMustNotHaveAttributeFlags(expression);
    }
    return this->tabulateOrderedGlobal(module, scope, expression) &&
           attributes_ok;
  }
  case requite::Opcode::CONSTANT:
    return this->tabulateConstant(module, scope, expression, attributes);
  case requite::Opcode::FUNCTION:
    return this->tabulateFunction(module, scope, expression, attributes);
  default:
    break;
  }
  const bool scoped_values_ok =
      this->tabulateScopedValues(module, scope, expression);
  return !attributes.getHasAnyAttribute() && scoped_values_ok;
}

bool Context::tabulateScopedValues(requite::Module &module,
                                   requite::Scope &scope,
                                   requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_ANONYMOUS_FUNCTION:
    return this->tabulateAnonymousFunction(module, scope, expression);
  case requite::Opcode::_LOCAL:
    return this->tabulateLocal(module, scope, expression);
  default:
    break;
  }
  bool is_ok = true;
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    if (!this->tabulateScopedValues(module, scope, expression)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::tabulateEntryPoint(requite::Module &module, requite::Scope &scope,
                                 requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ENTRY_POINT);
  REQUITE_ASSERT(scope.getModule() == module);
  REQUITE_ASSERT(module.getScope() == scope);
  if (module.getHasEntryPoint()) {
    this->logSourceMessage(expression, requite::LogType::ERROR,
                           "duplicate entry_point in module");
    return false;
  }
  requite::Procedure &procedure = module.makeProcedure();
  procedure.setType(requite::ProcedureType::ENTRY_POINT);
  procedure.setExpression(expression);
  procedure.setContaining(scope);
  module.setEntryPoint(procedure);
  bool is_ok = true;
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    if (!this->tabulateLocalUserSymbol(module, procedure.getScope(), branch,
                                       requite::AttributeFlags())) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::tabulateImport(requite::Module &module, requite::Scope &scope,
                             requite::Expression &expression,
                             requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::IMPORT);
  requite::Node &node = scope.getNodes().emplace_back();
  node.setType(requite::NodeType::IMPORT);
  node.setExpression(expression);
  node.setAttributeFlags(attributes);
  return true;
}

bool Context::tabulateUse(requite::Module &module, requite::Scope &scope,
                          requite::Expression &expression,
                          requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::USE);
  requite::Node &node = scope.getNodes().emplace_back();
  node.setType(requite::NodeType::USE);
  node.setExpression(expression);
  node.setAttributeFlags(attributes);
  return true;
}

bool Context::tabulateTable(requite::Module &module, requite::Scope &scope,
                            requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::TABLE);
  requite::Expression &name_expression = expression.getBranch();
  if (name_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    requite::Table &table = module.makeTable();
    requite::Expression &name_expression = expression.getBranch();
    table.setContaining(scope);
    this->logErrorNonInstantEvaluatableName(expression);
    if (name_expression.getHasNext()) {
      requite::Expression &next = name_expression.getNext();
      for (requite::Expression &statement : next.getHorizontalSubrange()) {
        std::ignore = this->tabulateTableUserSymbol(
            module, table.getScope(), statement, requite::AttributeFlags());
      }
    }
    return false;
  }
  llvm::StringRef name = name_expression.getDataText();
  requite::Table *table_ptr = nullptr;
  if (scope.getHasSymbolOfName(name)) {
    requite::Table &table = scope.lookupInternalRootSymbol(name).getTable();
    REQUITE_ASSERT(table.getName() == name);
    REQUITE_ASSERT(table.getContaining() == scope);
    table_ptr = &table;
  } else {
    requite::Table &table = module.makeTable();
    table.setName(name);
    table.setContaining(scope);
    table_ptr = &table;
  }
  requite::Table &table = requite::getRef(table_ptr);
  bool is_ok = true;
  for (requite::Expression &statement : name_expression.getNextSubrange()) {
    if (!this->tabulateTableUserSymbol(module, table.getScope(), statement,
                                        requite::AttributeFlags())) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::tabulateLocalStatementScope(requite::Module &module,
                                          requite::Scope &scope,
                                          requite::Expression &expression) {
  requite::Scope &new_scope = module.makeScope();
  new_scope.setContaining(scope);
  new_scope.setLocalStatement(expression);
  expression.setScope(new_scope);
  bool is_ok = true;
  for (requite::Expression &statement : expression.getBranchSubrange()) {
    if (!this->tabulateLocalUserSymbol(module, new_scope, statement,
                                       requite::AttributeFlags())) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::tabulateObject(requite::Module &module, requite::Scope &scope,
                             requite::Expression &expression,
                             requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::OBJECT);
  requite::Object &object = module.makeObject();
  object.setContaining(scope);
  object.setExpression(expression);
  object.setAttributeFlags(attributes);
  requite::Expression &name_expression = expression.getBranch();
  bool is_ok = true;
  if (name_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    this->logErrorNonInstantEvaluatableName(name_expression);
    is_ok = false;
  } else {
    llvm::StringRef name = name_expression.getSourceText();
    object.setName(name);
    if (scope.getHasSymbolOfName(name)) {
      this->logErrorAlreadySymbolOfName(name_expression);
      is_ok = false;
    } else {
      scope.addSymbol(object);
    }
  }
  for (requite::Expression &statement : name_expression.getNextSubrange()) {
    if (!this->tabulateMemberUserSymbol(module, object.getScope(), statement,
                                        requite::AttributeFlags())) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::tabulateAlias(requite::Module &module, requite::Scope &scope,
                            requite::Expression &expression,
                            requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ALIAS);
  requite::Alias &alias = module.makeAlias();
  alias.setExpression(expression);
  alias.setContaining(scope);
  alias.setAttributeFlags(attributes);
  requite::Expression &name_expression = expression.getBranch();
  bool is_ok = true;
  if (name_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    this->logErrorNonInstantEvaluatableName(name_expression);
    is_ok = false;
  } else {
    llvm::StringRef name = name_expression.getSourceText();
    alias.setName(name);
    if (scope.getHasSymbolOfName(name)) {
      this->logErrorAlreadySymbolOfName(name_expression);
      is_ok = false;
    } else {
      scope.addSymbol(alias);
    }
  }
  return is_ok;
}

bool Context::tabulateOrderedGlobal(requite::Module &module,
                                    requite::Scope &scope,
                                    requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_ORDERED_GLOBAL);
  requite::OrderedVariable &variable = module.makeOrderedVariable();
  variable.setType(requite::VariableType::GLOBAL);
  variable.setExpression(expression);
  variable.setContaining(scope);
  requite::Expression &name_expression = expression.getBranch();
  bool is_ok = true;
  if (name_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    this->logErrorNonInstantEvaluatableName(name_expression);
    is_ok = false;
  } else {
    llvm::StringRef name = name_expression.getSourceText();
    variable.setName(name);
    if (scope.getHasSymbolOfName(name)) {
      this->logErrorAlreadySymbolOfName(name_expression);
      is_ok = false;
    } else {
      scope.addSymbol(variable);
    }
  }
  return is_ok;
}

bool Context::tabulateUnorderedGlobal(requite::Module &module,
                                      requite::Scope &scope,
                                      requite::Expression &expression,
                                      requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_UNORDERED_GLOBAL);
  requite::UnorderedVariable &variable = module.makeUnorderedVariable();
  variable.setType(requite::VariableType::GLOBAL);
  variable.setExpression(expression);
  variable.setContaining(scope);
  variable.setAttributeFlags(attributes);
  requite::Expression &name_expression = expression.getBranch();
  bool is_ok = true;
  if (name_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    this->logErrorNonInstantEvaluatableName(name_expression);
    is_ok = false;
  } else {
    llvm::StringRef name = name_expression.getSourceText();
    variable.setName(name);
    if (scope.getHasSymbolOfName(name)) {
      this->logErrorAlreadySymbolOfName(name_expression);
      is_ok = false;
    } else {
      scope.addSymbol(variable);
    }
  }
  return is_ok;
}

bool Context::tabulateConstant(requite::Module &module, requite::Scope &scope,
                               requite::Expression &expression,
                               requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::CONSTANT);
  requite::UnorderedVariable &variable = module.makeUnorderedVariable();
  variable.setType(requite::VariableType::CONSTANT);
  variable.setExpression(expression);
  variable.setContaining(scope);
  variable.setAttributeFlags(attributes);
  requite::Expression &name_expression = expression.getBranch();
  bool is_ok = true;
  if (name_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    this->logErrorNonInstantEvaluatableName(name_expression);
    is_ok = false;
  } else {
    llvm::StringRef name = name_expression.getSourceText();
    variable.setName(name);
    if (scope.getHasSymbolOfName(name)) {
      this->logErrorAlreadySymbolOfName(name_expression);
      is_ok = false;
    } else {
      scope.addSymbol(variable);
    }
  }
  return is_ok;
}

bool Context::tabulateFunction(requite::Module &module, requite::Scope &scope,
                               requite::Expression &expression,
                               requite::AttributeFlags attributes) {
  return false;
}

bool Context::tabulateProperty(requite::Module &module, requite::Scope &scope,
                               requite::Expression &expression,
                               requite::AttributeFlags attributes) {
  return false;
}

bool Context::tabulateConstructor(requite::Module &module,
                                  requite::Scope &scope,
                                  requite::Expression &expression,
                                  requite::AttributeFlags attributes) {
  return false;
}

bool Context::tabulateDestructor(requite::Module &module, requite::Scope &scope,
                                 requite::Expression &expression,
                                 requite::AttributeFlags attributes) {
  return false;
}

bool Context::tabulateMethod(requite::Module &module, requite::Scope &scope,
                             requite::Expression &expression,
                             requite::AttributeFlags attributes) {
  return false;
}

bool Context::tabulateLocal(requite::Module &module, requite::Scope &scope,
                            requite::Expression &expression) {
  return false;
}

bool Context::tabulateAnonymousFunction(requite::Module &module,
                                        requite::Scope &scope,
                                        requite::Expression &expression) {
  return false;
}

} // namespace requite
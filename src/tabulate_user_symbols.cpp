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
    requite::Expression &unascribed = expression.getBranch();
    bool is_ok = true;
    for (requite::Expression &attribute : unascribed.getNextSubrange()) {
      requite::AttributeType type =
          requite::getAttributeType(attribute.getOpcode());
      if (attributes.getHasAttribute(type) && type != requite::AttributeType::USER) {
        this->logErrorDuplicateAttribute(attribute, type);
        is_ok = false;
        continue;
      }
    }
    if (!this->tabulateBaseUserSymbol(module, scope, unascribed, attributes)) {
      is_ok = false;
    }
    return is_ok;
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
  case requite::Opcode::GLOBAL:
    return this->tabulateGlobal(module, scope, expression, attributes);
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
    requite::Expression &unascribed = expression.getBranch();
    bool is_ok = true;
    for (requite::Expression &attribute : unascribed.getNextSubrange()) {
      requite::AttributeType type =
          requite::getAttributeType(attribute.getOpcode());
      if (attributes.getHasAttribute(type) && type != requite::AttributeType::USER) {
        this->logErrorDuplicateAttribute(attribute, type);
        is_ok = false;
        continue;
      }
    }
    if (!this->tabulateTableUserSymbol(module, scope, unascribed, attributes)) {
      is_ok = false;
    }
    return is_ok;
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
  case requite::Opcode::GLOBAL:
    return this->tabulateGlobal(module, scope, expression, attributes);
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
    bool is_ok = true;
    for (requite::Expression &attribute : unascribed.getNextSubrange()) {
      requite::AttributeType type =
          requite::getAttributeType(attribute.getOpcode());
      if (attributes.getHasAttribute(type) && type != requite::AttributeType::USER) {
        this->logErrorDuplicateAttribute(attribute, type);
        is_ok = false;
        continue;
      }
    }
    if (!this->tabulateMemberUserSymbol(module, scope, unascribed, attributes)) {
      is_ok = false;
    }
    return is_ok;
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
  case requite::Opcode::GLOBAL:
    return this->tabulateGlobal(module, scope, expression, attributes);
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
      if (type != requite::AttributeType::USER &&
          attributes.getHasAttribute(type)) {
        this->logErrorDuplicateAttribute(attribute, type);
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
  requite::RootSymbol found = scope.lookupInternalRootSymbol(name);
  bool is_ok = true;
  if (found.getIsTable()) {
    requite::Table &table = found.getTable();
    REQUITE_ASSERT(table.getName() == name);
    REQUITE_ASSERT(table.getContaining() == scope);
    table_ptr = &table;
  } else {
    requite::Table &table = module.makeTable();
    table.setName(name);
    table_ptr = &table;
    if (found.getIsNone()) {
      scope.addSymbol(table);
    } else {
      this->logErrorAlreadySymbolOfName(name_expression);
      is_ok = false;
    }
  }
  requite::Table &table = requite::getRef(table_ptr);
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

bool Context::tabulateGlobal(requite::Module &module,
                                      requite::Scope &scope,
                                      requite::Expression &expression,
                                      requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::GLOBAL);
  requite::UnorderedVariable &variable = module.makeUnorderedVariable();
  variable.setType(requite::VariableType::GLOBAL);
  variable.setExpression(expression);
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
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::FUNCTION);
  requite::Procedure &function = module.makeProcedure();
  function.setType(requite::ProcedureType::FUNCTION);
  function.setAttributeFlags(attributes);
  requite::Expression &name_expression = expression.getBranch();
  bool is_ok = true;
  if (name_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    this->logErrorNonInstantEvaluatableName(name_expression);
    is_ok = false;
  } else {
    llvm::StringRef name = name_expression.getSourceText();
    requite::RootSymbol found = scope.lookupInternalRootSymbol(name);
    if (found.getIsNone()) {
      requite::NamedProcedureGroup &group = module.makeNamedProcedureGroup();
      group.setName(name);
      scope.addSymbol(group);
      function.setNamedProcedureGroup(group);
    } else if (found.getIsNamedProcedureGroup()) {
      requite::NamedProcedureGroup &group = found.getNamedProcedureGroup();
      REQUITE_ASSERT(name == group.getName());
      function.setNamedProcedureGroup(group);
    } else {
      this->logErrorAlreadySymbolOfName(name_expression);
      is_ok = false;
    }
  }
  requite::Expression &signature_expression = name_expression.getNext();
  for (requite::Expression &statement :
       signature_expression.getNextSubrange()) {
    if (!this->tabulateLocalUserSymbol(module, function.getScope(), statement,
                                       requite::AttributeFlags())) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::tabulateProperty(requite::Module &module, requite::Scope &scope,
                               requite::Expression &expression,
                               requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::PROPERTY);
  REQUITE_ASSERT(scope.getType() == requite::ScopeType::TABLE);
  requite::UnorderedVariable &variable = module.makeUnorderedVariable();
  variable.setType(requite::VariableType::PROPERTY);
  variable.setExpression(expression);
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

bool Context::tabulateConstructor(requite::Module &module,
                                  requite::Scope &scope,
                                  requite::Expression &expression,
                                  requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::CONSTRUCTOR);
  REQUITE_ASSERT(scope.getType() == requite::ScopeType::OBJECT);
  requite::Procedure &constructor = module.makeProcedure();
  constructor.setType(requite::ProcedureType::CONSTRUCTOR);
  constructor.setContaining(scope);
  requite::Object &object = scope.getObject();
  object.addConstructor(constructor);
  return true;
}

bool Context::tabulateDestructor(requite::Module &module, requite::Scope &scope,
                                 requite::Expression &expression,
                                 requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::DESTRUCTOR);
  REQUITE_ASSERT(scope.getType() == requite::ScopeType::OBJECT);
  requite::Procedure &destructor = module.makeProcedure();
  destructor.setType(requite::ProcedureType::DESTRUCTOR);
  destructor.setContaining(scope);
  requite::Object &object = scope.getObject();
  if (object.getHasDestructor()) {
    this->logSourceMessage(expression, requite::LogType::ERROR,
                           "more than one destructor");
    return false;
  }
  object.setDestructor(destructor);
  return true;
}

bool Context::tabulateMethod(requite::Module &module, requite::Scope &scope,
                             requite::Expression &expression,
                             requite::AttributeFlags attributes) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::METHOD);
  REQUITE_ASSERT(scope.getType() == requite::ScopeType::OBJECT);
  requite::Procedure &method = module.makeProcedure();
  method.setType(requite::ProcedureType::METHOD);
  method.setAttributeFlags(attributes);
  requite::Expression &name_expression = expression.getBranch();
  bool is_ok = true;
  if (name_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    this->logErrorNonInstantEvaluatableName(name_expression);
    is_ok = false;
  } else {
    llvm::StringRef name = name_expression.getSourceText();
    requite::RootSymbol found = scope.lookupInternalRootSymbol(name);
    if (found.getIsNone()) {
      requite::NamedProcedureGroup &group = module.makeNamedProcedureGroup();
      group.setName(name);
      scope.addSymbol(group);
      method.setNamedProcedureGroup(group);
    } else if (found.getIsNamedProcedureGroup()) {
      requite::NamedProcedureGroup &group = found.getNamedProcedureGroup();
      REQUITE_ASSERT(name == group.getName());
      method.setNamedProcedureGroup(group);
    } else {
      this->logErrorAlreadySymbolOfName(name_expression);
      is_ok = false;
    }
  }
  requite::Expression &signature_expression = name_expression.getNext();
  for (requite::Expression &statement :
       signature_expression.getNextSubrange()) {
    if (!this->tabulateLocalUserSymbol(module, method.getScope(), statement,
                                       requite::AttributeFlags())) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::tabulateLocal(requite::Module &module, requite::Scope &scope,
                            requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_LOCAL);
  requite::OrderedVariable &variable = module.makeOrderedVariable();
  variable.setType(requite::VariableType::LOCAL);
  variable.setExpression(expression);
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

bool Context::tabulateAnonymousFunction(requite::Module &module,
                                        requite::Scope &scope,
                                        requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() ==
                 requite::Opcode::_ANONYMOUS_FUNCTION);
  requite::AnonymousFunction &anonymous_function =
      module.makeAnonymousFunction();
  anonymous_function.setExpression(expression);
  anonymous_function.setContaining(scope);
  bool is_ok = true;
  requite::Expression &capture = expression.getBranch();
  requite::Expression &signature = capture.getNext();
  for (requite::Expression &branch : signature.getNextSubrange()) {
    if (!this->tabulateLocalUserSymbol(module, anonymous_function.getScope(),
                                       branch, requite::AttributeFlags())) {
      is_ok = false;
    }
  }
  return is_ok;
}

} // namespace requite
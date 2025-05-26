#include <requite/context.hpp>
#include <requite/expression.hpp>

namespace requite {

void Context::makeUserSymbols() {
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    requite::Table &table = module.getTable();
    requite::Scope &scope = module.getScope();
    requite::Expression &root = module.getExpression();
    REQUITE_ASSERT(!root.getHasNext());
    requite::Expression &module_name_expression = root.getBranch();
    if (!module_name_expression.getHasNext()) {
      return;
    }
    requite::Expression &body = module_name_expression.getNext();
    this->makeUnorderedUserSymbols(module, scope, body, true);
  }
}

void Context::makeUnorderedUserSymbols(requite::Module &module,
                                       requite::Scope &scope,
                                       requite::Expression &body, bool conduits_have_scopes) {
  for (requite::Expression &branch : body.getHorizontalSubrange()) {
    switch (const requite::Opcode opcode = branch.getOpcode()) {
    case requite::Opcode::ALIAS: {
      requite::Alias &alias = module.makeAlias();
      alias.setExpression(branch);
      branch.setAlias(alias);
      alias.setContainingScope(scope);
      this->makeScopedValues(module, scope, branch.getBranch(), true);
    } break;
    case requite::Opcode::OBJECT: {
      requite::Object &object = module.makeObject();
      object.setExpression(branch);
      branch.setObject(object);
      object.setContainingScope(scope);
      this->makeUnorderedUserSymbols(module, object.getScope(), branch.getBranch(), conduits_have_scopes);
    } break;
    case requite::Opcode::GLOBAL: {
      requite::Variable &global = module.makeVariable();
      global.setType(requite::VariableType::GLOBAL);
      global.setExpression(branch);
      branch.setVariable(global);
      global.setContainingScope(scope);
      this->makeScopedValues(module, scope, branch.getBranch(), conduits_have_scopes);
    } break;
    case requite::Opcode::PROPERTY: {
      requite::Object &object = module.makeObject();
      object.setExpression(branch);
      branch.setObject(object);
      object.setContainingScope(scope);
      this->makeUnorderedUserSymbols(module, object.getScope(), branch.getBranch(), true);
    } break;
    case requite::Opcode::ENTRY_POINT: {
      requite::Procedure &entry_point = module.makeProcedure();
      entry_point.setType(requite::ProcedureType::ENTRY_POINT);
      entry_point.setExpression(branch);
      branch.setProcedure(entry_point);
      entry_point.setContainingScope(scope);
      this->makeOrderedUserSymbols(module, entry_point.getScope(),
                                   branch.getBranch());
    } break;
    case requite::Opcode::METHOD: {
      requite::Procedure &method = module.makeProcedure();
      method.setType(requite::ProcedureType::METHOD);
      method.setExpression(branch);
      branch.setProcedure(method);
      method.setContainingScope(scope);
      this->makeOrderedUserSymbols(module, method.getScope(),
                                   branch.getBranch());
    } break;
    case requite::Opcode::FUNCTION: {
      requite::Procedure &function = module.makeProcedure();
      function.setType(requite::ProcedureType::FUNCTION);
      function.setExpression(branch);
      branch.setProcedure(function);
      function.setContainingScope(scope);
      this->makeOrderedUserSymbols(module, function.getScope(),
                                   branch.getBranch());
    } break;
    case requite::Opcode::CONSTRUCTOR: {
      requite::Procedure &constructor = module.makeProcedure();
      constructor.setType(requite::ProcedureType::CONSTRUCTOR);
      constructor.setExpression(branch);
      branch.setProcedure(constructor);
      constructor.setContainingScope(scope);
      this->makeOrderedUserSymbols(module, constructor.getScope(),
                                   branch.getBranch());
    } break;
    case requite::Opcode::DESTRUCTOR: {
      requite::Procedure &destructor = module.makeProcedure();
      destructor.setType(requite::ProcedureType::DESTRUCTOR);
      destructor.setExpression(branch);
      branch.setProcedure(destructor);
      destructor.setContainingScope(scope);
      this->makeOrderedUserSymbols(module, destructor.getScope(),
                                   branch.getBranch());
    } break;
    case requite::Opcode::TABLE: {
      requite::Table &table = module.makeTable();
      table.setExpression(branch);
      branch.setTable(table);
      table.setContainingScope(scope);
      this->makeUnorderedUserSymbols(module, table.getScope(),
                                     branch.getBranch(), conduits_have_scopes);
    } break;
    default:
      this->makeScopedValues(module, scope, branch, false);
    }
  }
}

void Context::makeOrderedUserSymbols(requite::Module &module,
                                     requite::Scope &scope,
                                     requite::Expression &body) {
  for (requite::Expression &branch : body.getHorizontalSubrange()) {
    switch (const requite::Opcode opcode = branch.getOpcode()) {
    case requite::Opcode::LOCAL: {
      requite::Variable &local = module.makeVariable();
      local.setType(requite::VariableType::LOCAL);
      local.setExpression(branch);
      branch.setVariable(local);
      local.setContainingScope(scope);
      this->makeScopedValues(module, scope, branch.getBranch(), false);
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
      requite::Scope &new_scope = module.makeScope();
      branch.setScope(new_scope);
      new_scope.setExpression(branch);
      new_scope.setContainingScope(scope);
    } break;
    default:
      this->makeUnorderedUserSymbols(module, scope, branch, false);
    }
  }
}

void Context::makeScopedValues(requite::Module &module, requite::Scope &scope,
                               requite::Expression &expression,
                               bool conduits_have_scopes) {
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    switch (const requite::Opcode opcode = branch.getOpcode()) {
    case requite::Opcode::ANONYMOUS_FUNCTION: {
      requite::AnonymousFunction &anonymous_function =
          module.makeAnonymousFunction();
      anonymous_function.setExpression(expression);
      expression.setAnonymousFunction(anonymous_function);
      anonymous_function.setContainingScope(scope);
      requite::Expression &capture = expression.getBranch();
      REQUITE_ASSERT(capture.getOpcode() == requite::Opcode::CAPTURE);
      requite::Expression &signature = capture.getNext();
      requite::Expression &body = signature.getNext();
      requite::Scope &scope = anonymous_function.getScope();
      this->makeScopedValues(module, scope, body, false);
    } break;
    case requite::Opcode::CONDUIT: {
      if (conduits_have_scopes) {
        requite::Scope &new_scope = module.makeScope();
        branch.setScope(new_scope);
        new_scope.setExpression(branch);
        new_scope.setContainingScope(scope);
        this->makeOrderedUserSymbols(module, new_scope, expression.getBranch());
        break;
      }
      this->makeOrderedUserSymbols(module, scope, expression.getBranch());
    } break;
    default:
      this->makeScopedValues(module, scope, branch, conduits_have_scopes);
    }
  }
}

} // namespace requite
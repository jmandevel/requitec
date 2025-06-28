#include <requite/assert.hpp>
#include <requite/context.hpp>

namespace requite {

bool Context::contextualizeModule(requite::Module &module) {
  REQUITE_ASSERT(module.getHasExpression());
  requite::Scope &module_scope = module.getScope();
  REQUITE_ASSERT(module_scope.getModule() == module);
  requite::Expression &root = module.getExpression();
  REQUITE_ASSERT(root.getOpcode() == requite::Opcode::MODULE);
  REQUITE_ASSERT(!root.getHasNext());
  requite::Expression &module_name = root.getBranch();
  REQUITE_ASSERT(module_name.getOpcode() ==
                 requite::Opcode::__IDENTIFIER_LITERAL);
  bool is_ok = true;
  for (requite::Expression &expression : module_name.getNextSubrange()) {
    switch (const requite::Opcode opcode = expression.getOpcode()) {
    case requite::Opcode::_ASCRIBE_FIRST_BRANCH: {
      requite::Expression &ascribed = expression.getBranch();
      switch (const requite::Opcode ascribed_opcode = ascribed.getOpcode()) {
      case requite::Opcode::ENTRY_POINT:
        this->logErrorMustNotHaveAttributeFlags(ascribed);
        is_ok = false;
        break;
      case requite::Opcode::FUNCTION:
        this->logNotSupportedYet(expression);
        is_ok = false;
        break;
      case requite::Opcode::GLOBAL:
        this->logNotSupportedYet(expression);
        is_ok = false;
        break;
      case requite::Opcode::OBJECT:
        this->logNotSupportedYet(expression);
        is_ok = false;
        break;
      case requite::Opcode::TABLE:
        this->logNotSupportedYet(expression);
        is_ok = false;
        break;
      case requite::Opcode::IMPORT:
        this->logNotSupportedYet(expression);
        is_ok = false;
        break;
      case requite::Opcode::USE:
        this->logNotSupportedYet(expression);
        is_ok = false;
        break;
      case requite::Opcode::_EXPAND_VALUE:
        this->logNotSupportedYet(expression);
        is_ok = false;
        break;
      default:
        REQUITE_UNREACHABLE();
      }
      break;
    }
    case requite::Opcode::ENTRY_POINT:
      this->tabulateEntryPoint(module, expression);
      break;
    case requite::Opcode::FUNCTION:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::GLOBAL:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::OBJECT:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::TABLE:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::IMPORT:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::USE:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::_EXPAND_VALUE:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    default:
      REQUITE_UNREACHABLE();
    }
  }
  if (!is_ok) {
    return false;
  }
  for (requite::Expression &expression : module_name.getNextSubrange()) {
    switch (const requite::Opcode opcode = expression.getOpcode()) {
    case requite::Opcode::ENTRY_POINT:
      if (!this->prototypeEntryPoint(expression.getProcedure())) {
        is_ok = false;
      }
      break;
    case requite::Opcode::FUNCTION:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::GLOBAL:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::OBJECT:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::TABLE:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::IMPORT:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::USE:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    case requite::Opcode::_EXPAND_VALUE:
      this->logNotSupportedYet(expression);
      is_ok = false;
      break;
    default:
      REQUITE_UNREACHABLE();
    }
  }
  
  return is_ok;
}

} // namespace requite
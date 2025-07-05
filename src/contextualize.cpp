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
  if (!this->tabulateModuleGlobalUserSymbols(module)) {
    return false;
  }
  bool is_ok = true;
  if (!this->implementGlobalUserSymbols()) {
    is_ok = false;
  }
  if (!this->checkEntryPointCount()) {
    return false;
  }
  return is_ok;
}

bool Context::checkEntryPointCount() {
  requite::Module &source_module = this->getSourceModule();
  if (!source_module.getHasEntryPoint()) {
    return true;
  }
  requite::Procedure &entry_point = source_module.getEntryPoint();
  if (!entry_point.getHasNextProcedure()) {
    return true;
  }
  for (requite::Procedure &overload : entry_point.getOverloadSubrange()) {
    this->logSourceMessage(overload.getExpression(), requite::LogType::ERROR,
                           "multiple entry points in module.");
  }
  return false;
}

} // namespace requite
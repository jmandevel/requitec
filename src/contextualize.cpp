#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/contextualizer1.hpp>

namespace requite {

bool Context::contextualizeAll() {
  requite::Module &module = this->getSourceModule();
  REQUITE_ASSERT(module.getHasExpression());
  requite::Scope &module_scope = module.getScope();
  REQUITE_ASSERT(module_scope.getModule() == module);
  requite::Expression &root = module.getExpression();
  REQUITE_ASSERT(root.getOpcode() == requite::Opcode::MODULE);
  REQUITE_ASSERT(!root.getHasNext());
  requite::Expression &module_name = root.getBranch();
  REQUITE_ASSERT(module_name.getOpcode() ==
                 requite::Opcode::__IDENTIFIER_LITERAL);
  if (!this->passContextualize0()) {
    return false;
  }
  bool is_ok = true;
  if (!this->passContextualize1()) {
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

bool Context::passContextualize0() {
  REQUITE_ASSERT(!this->getIsContextualize0Done());
  requite::Module &module = this->getSourceModule();
  const bool is_ok = this->contextualize0Module(module);
  this->setContextualize0Done();
  return is_ok;
}

bool Context::contextualize0Module(requite::Module &module) {
  requite::Contextualizer0 contextualizer0(*this, module);
  if (!contextualizer0.tabulateModule()) {
    return false;
  }
  return true;
}

bool Context::passContextualize1() {
  // implement them in order to try to decrease recursive implementations
  bool is_ok = true;
  for (std::unique_ptr<requite::Alias> &alias_uptr : this->getAliasUptrs()) {
    requite::Alias &alias = requite::getRef(alias_uptr);
    if (!this->implementAlias(alias)) {
      return false;
    }
  }
  for (std::unique_ptr<requite::Global> &global_uptr : this->getGlobalUptrs()) {
    requite::Global &global = requite::getRef(global_uptr);
    if (!this->implementGlobal(global)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::Property> &property_uptr :
       this->getPropertyUptrs()) {
    requite::Property &property = requite::getRef(property_uptr);
    if (!this->implementProperty(property)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::Procedure> &procedure_uptr :
       this->getProcedureUptrs()) {
    requite::Procedure &procedure = requite::getRef(procedure_uptr);
    if (!this->implementProcedure(procedure)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::Object> &object_uptr : this->getObjectUptrs()) {
    requite::Object &object = requite::getRef(object_uptr);
    if (!this->implementObject(object)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::getIsContextualize0Done() const {
  return this->_contextualize0_done;
}

void Context::setContextualize0Done() {
  REQUITE_ASSERT(!this->getIsContextualize0Done());
  this->_contextualize0_done = true;
}

} // namespace requite
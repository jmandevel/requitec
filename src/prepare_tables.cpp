#include <requite/assert.hpp>
#include <requite/context.hpp>

namespace requite {

void Context::propogateExportedImports() {
  while (!this->propogateExportImportPass()) {
  }
}

bool Context::propogateExportImportPass() {
  bool is_done = true;
  for (std::unique_ptr<requite::Module> &module_uptr_a :
       this->getModuleUptrs()) {
    requite::Module &module_a = requite::getRef(module_uptr_a);
    for (std::unique_ptr<requite::Module> &module_uptr_b :
         this->getModuleUptrs()) {
      requite::Module &module_b = requite::getRef(module_uptr_b);
      if (module_a == module_b) {
        continue;
      }
      if (module_b.getImportModulePtrSet().contains(&module_a)) {
        for (requite::Module *exported_module_ptr :
             module_a.getExportModulePtrs()) {
          requite::Module &exported_module =
              requite::getRef(exported_module_ptr);
          if (module_a.getImportModulePtrSet().contains(&exported_module)) {
            continue;
          }
          is_done = false;
          module_a.getImportModulePtrSet().insert(&exported_module);
        }
      }
    }
  }
  return is_done;
}

void Context::diffuseUses() {
  while (!this->diffuseUsePass()) {
  }
}

bool Context::diffuseUsePass() {
  bool is_done = true;
  for (std::unique_ptr<requite::Use> &use_uptr : this->getUseUptrs()) {
    requite::Use &use = requite::getRef(use_uptr);
    // TODO
  }
  return is_done;
}

} // namespace requite
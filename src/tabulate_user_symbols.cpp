// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/symbol.hpp>
#include <requite/tabulator.hpp>

#include <vector>

namespace requite {

bool Context::tabulateUserSymbols() {
  requite::Tabulator tabulator(*this);
  tabulator.tabulateUserSymbols();
  return tabulator.getIsOk();
}

void Tabulator::tabulateUserSymbols() {
  std::vector<requite::RootSymbol> unamed_symbols = {};
  for (std::unique_ptr<requite::Module> &module_uptr :
       this->getContext().getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    for (std::unique_ptr<requite::Scope> &scope_uptr : module.getScopeUptrs()) {
      requite::Scope& scope = requite::getRef(scope_uptr);
    }
    for (std::unique_ptr<requite::Object> &object_uptr :
         module.getObjectUptrs()) {
      requite::Object &object = requite::getRef(object_uptr);
      requite::Expression &expression = object.getExpression();
      requite::Expression &name_expression = expression.getBranch();
      if (name_expression.getIsIdentifier()) {
        llvm::StringRef name = name_expression.getDataText();
        object.setName(name);
        continue;
      }
      unamed_symbols.emplace_back().setObject(object);
    }
    for (std::unique_ptr<requite::Procedure> &procedure_uptr :
         module.getProcedureUptrs()) {
      requite::Procedure &procedure = requite::getRef(procedure_uptr);
      if (!procedure.getIsNamed()){
        continue;
      }
      
    }
    for (std::unique_ptr<requite::Alias> &alias_uptr : module.getAliasUptrs()) {
      requite::Alias &alias = requite::getRef(alias_uptr);
    }
    for (std::unique_ptr<requite::Variable> &variable_uptr :
         module.getVariableUptrs()) {
      requite::Variable &variable = requite::getRef(variable_uptr);
    }
    for (std::unique_ptr<requite::Label> &label_uptr : module.getLabelUptrs()) {
      requite::Label &label = requite::getRef(label_uptr);
    }
  }
}

} // namespace requite
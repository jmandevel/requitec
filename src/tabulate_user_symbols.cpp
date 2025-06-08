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
  this->tabulateInstantNamedSymbols();
  if (!this->getIsOk()) {
    return;
  }
}

void Tabulator::tabulateInstantNamedSymbols() {
  for (std::unique_ptr<requite::Module> &module_uptr :
       this->getContext().getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    for (std::unique_ptr<requite::Scope> &scope_uptr : module.getScopeUptrs()) {
      requite::Scope &scope = requite::getRef(scope_uptr);
      if (scope.getType() != requite::ScopeType::TABLE) {
        continue;
      }
      requite::Scope &containing_scope = scope.getContainingScope();
      requite::Expression &expression = scope.getExpression();
      requite::Expression &name_expression = expression.getBranch();
      if (name_expression.getIsIdentifier()) {
        llvm::StringRef name = name_expression.getDataText();
        requite::RootSymbol found =
            containing_scope.lookupInternalRootSymbol(name);
        if (found.getIsNone()) {
          requite::Table &table = module.makeTable();
          table.addSubScope(scope);
          scope.setTable(table);
          table.setName(name.str());
          containing_scope.addSymbol(table);
          continue;
        } else if (found.getIsTable()) {
          requite::Table &table = found.getTable();
          REQUITE_ASSERT(name == table.getName());
          table.addSubScope(scope);
          scope.setTable(table);
          continue;
        } else {
          this->getContext().logSourceMessage(expression,
                                              requite::LogType::ERROR,
                                              "multiple symbols with name");
          this->setNotOk();
          continue;
        }
      }
      this->getContext().logSourceMessage(
          expression, requite::LogType::ERROR,
          "evaluated names not supported in current version of requitec");
      this->setNotOk();
    }
    for (std::unique_ptr<requite::Object> &object_uptr :
         module.getObjectUptrs()) {
      requite::Object &object = requite::getRef(object_uptr);
      requite::Scope &containing_scope = object.getContainingScope();
      requite::Expression &expression = object.getExpression();
      requite::Expression &name_expression = expression.getBranch();
      if (name_expression.getIsIdentifier()) {
        llvm::StringRef name = name_expression.getDataText();
        object.setName(name);
        requite::RootSymbol found =
            containing_scope.lookupInternalRootSymbol(name);
        if (!found.getIsNone()) {
          this->getContext().logSourceMessage(expression,
                                              requite::LogType::ERROR,
                                              "multiple symbols with name");
          this->setNotOk();
          continue;
        }
        containing_scope.addSymbol(object);
        continue;
      }
      this->getContext().logSourceMessage(
          expression, requite::LogType::ERROR,
          "evaluated names not supported in current version of requitec");
      this->setNotOk();
    }
    for (std::unique_ptr<requite::Procedure> &procedure_uptr :
         module.getProcedureUptrs()) {
      requite::Procedure &procedure = requite::getRef(procedure_uptr);
      if (!procedure.getIsNamed()) {
        continue;
      }
      requite::Scope &containing_scope = procedure.getContainingScope();
      requite::Expression &expression = procedure.getExpression();
      requite::Expression &name_expression = expression.getBranch();
      llvm::StringRef name = name_expression.getDataText();
      requite::RootSymbol found =
          containing_scope.lookupInternalRootSymbol(name);
      if (found.getIsNone()) {
        requite::NamedProcedureGroup &group = module.makeNamedProcedureGroup();
        group.setName(name);
        procedure.setNamedProcedureGroup(group);
        group.addProcedure(procedure);
        group.setContainingScope(containing_scope);
        containing_scope.addSymbol(group);
        continue;
      } else if (found.getIsNamedProcedureGroup()) {
        requite::NamedProcedureGroup &group = found.getNamedProcedureGroup();
        REQUITE_ASSERT(group.getName() == name);
        procedure.setNamedProcedureGroup(group);
        group.addProcedure(procedure);
        continue;
      } else {
        this->getContext().logSourceMessage(expression, requite::LogType::ERROR,
                                            "multiple symbols with name");
        this->setNotOk();
        continue;
      }
      this->getContext().logSourceMessage(
          expression, requite::LogType::ERROR,
          "evaluated names not supported in current version of requitec");
      this->setNotOk();
    }
    for (std::unique_ptr<requite::Alias> &alias_uptr : module.getAliasUptrs()) {
      requite::Alias &alias = requite::getRef(alias_uptr);
      requite::Scope &containing_scope = alias.getContainingScope();
      requite::Expression &expression = alias.getExpression();
      requite::Expression &name_expression = expression.getBranch();
      if (name_expression.getIsIdentifier()) {
        llvm::StringRef name = name_expression.getDataText();
        requite::RootSymbol found =
            containing_scope.lookupInternalRootSymbol(name);
        if (!found.getIsNone()) {
          this->getContext().logSourceMessage(name_expression,
                                              requite::LogType::ERROR,
                                              "duplicate symbol with name");
          this->setNotOk();
          continue;
        }
        alias.setName(name);
        containing_scope.addSymbol(alias);
        continue;
      }
      this->getContext().logSourceMessage(
          expression, requite::LogType::ERROR,
          "evaluated names not supported in current version of requitec");
      this->setNotOk();
    }
    for (std::unique_ptr<requite::Variable> &variable_uptr :
         module.getVariableUptrs()) {
      requite::Variable &variable = requite::getRef(variable_uptr);
      requite::Scope &containing_scope = variable.getContainingScope();
      requite::Expression &expression = variable.getExpression();
      requite::Expression &name_expression = expression.getBranch();
      if (name_expression.getIsIdentifier()) {
        llvm::StringRef name = name_expression.getDataText();
        requite::RootSymbol found =
            containing_scope.lookupInternalRootSymbol(name);
        if (!found.getIsNone()) {
          this->getContext().logSourceMessage(name_expression,
                                              requite::LogType::ERROR,
                                              "duplicate symbol with name");
          this->setNotOk();
          continue;
        }
        variable.setName(name);
        containing_scope.addSymbol(variable);
        continue;
      }
      this->getContext().logSourceMessage(
          expression, requite::LogType::ERROR,
          "evaluated names not supported in current version of requitec");
      this->setNotOk();
    }
    for (std::unique_ptr<requite::Label> &label_uptr : module.getLabelUptrs()) {
      requite::Label &label = requite::getRef(label_uptr);
      requite::Scope &containing_scope = label.getContainingScope();
      requite::Expression &expression = label.getAttributeExpression();
      requite::Expression &name_expression = expression.getBranch();
      if (name_expression.getIsIdentifier()) {
        llvm::StringRef name = name_expression.getDataText();
        requite::RootSymbol found =
            containing_scope.lookupInternalRootSymbol(name);
        if (!found.getIsNone()) {
          this->getContext().logSourceMessage(name_expression,
                                              requite::LogType::ERROR,
                                              "duplicate symbol with name");
          this->setNotOk();
          continue;
        }
        label.setName(name);
        containing_scope.addSymbol(label);
        continue;
      }
      this->getContext().logSourceMessage(
          expression, requite::LogType::ERROR,
          "evaluated names not supported in current version of requitec");
      this->setNotOk();
    }
  }
}

} // namespace requite
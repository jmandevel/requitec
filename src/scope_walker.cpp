#include <requite/assert.hpp>
#include <requite/attributes.hpp>
#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/module.hpp>
#include <requite/node.hpp>
#include <requite/scope.hpp>
#include <requite/scope_walker.hpp>

#include <llvm/ADT/SmallPtrSet.h>

namespace requite {

ScopeWalker::ScopeWalker(requite::Context &context, requite::Scope &scope)
    : _context_ref(context), _scope_ref(scope) {}

requite::Context &ScopeWalker::getContext() { return this->_context_ref.get(); }

const requite::Context &ScopeWalker::getContext() const {
  return this->_context_ref.get();
}

requite::Scope &ScopeWalker::getScope() { return this->_scope_ref.get(); }

const requite::Scope &ScopeWalker::getScope() const {
  return this->_scope_ref.get();
}

requite::ScopeWalkerResult &ScopeWalker::getResult() { return this->_result; }

const requite::ScopeWalkerResult &ScopeWalker::getResult() const {
  return this->_result;
}

requite::ScopeWalkerResult ScopeWalker::doSearch(
    const std::function<requite::ScopeWalkerResultType(requite::Scope &scope)>
        &predicate) {
  for (requite::Scope &scope : this->getScope().getInternalScopeSubrange()) {
    if (this->_searched_scopes.contains(&scope)) {
      continue;
    }
    if (predicate(scope) == requite::ScopeWalkerResultType::FOUND) {
      this->getResult().type = requite::ScopeWalkerResultType::FOUND;
      return this->getResult();
    }
    this->_searched_scopes.insert(&scope);
    for (requite::Node &node : scope.getNodes()) {
      const requite::NodeType type = node.getType();
      if (type == requite::NodeType::IMPORT) {
        const bool should_continue = this->doImportSearch(node, predicate);
        if (!should_continue) {
          break;
        }
      } else if (type == requite::NodeType::USE) {
        // TODO
      } else {
        REQUITE_UNREACHABLE();
      }
    }
  }
  this->getResult().type = requite::ScopeWalkerResultType::NOT_FOUND;
  return this->getResult();
}

void ScopeWalker::doModuleExportsSearch(
    requite::Module &module,
    const std::function<requite::ScopeWalkerResultType(requite::Scope &scope)>
        &predicate) {
  requite::Scope &module_scope = module.getTable().getScope();
  if (this->_searched_scopes.contains(&module_scope)) {
    return;
  }
  this->_searched_scopes.insert(&module_scope);
  this->getResult().type = predicate(module_scope);
  if (this->getResult().type == requite::ScopeWalkerResultType::FOUND)
    return;
  for (requite::Node &node : module_scope.getNodes()) {
    if (!node.getAttributes().getHasAttribute(requite::AttributeType::EXPORT)) {
      continue;
    }
    const requite::NodeType type = node.getType();
    if (type == requite::NodeType::IMPORT) {
      const bool is_done = this->doImportSearch(node, predicate);
      if (is_done) {
        break;
      }
    } else if (type == requite::NodeType::USE) {
      // TODO
    } else {
      REQUITE_UNREACHABLE();
    }
  }
}

bool ScopeWalker::doImportSearch(
    requite::Node &node,
    const std::function<requite::ScopeWalkerResultType(requite::Scope &scope)>
        &predicate) {
  REQUITE_ASSERT(node.getType() == requite::NodeType::IMPORT);
  requite::Expression &import_expression = node.getExpression();
  REQUITE_ASSERT(import_expression.getOpcode() == requite::Opcode::IMPORT);
  requite::Expression &name_expression = import_expression.getBranch();
  llvm::StringRef name = name_expression.getDataText();
  if (!this->getContext().getHasModule(name)) {
    this->getContext().logSourceMessage(
        name_expression, requite::LogType::ERROR,
        llvm::Twine("no module with name: \"") + name + ".\"");
    this->getResult().has_error = true;
    return true;
  }
  requite::Module &module = this->getContext().getModule(name);
  this->doModuleExportsSearch(module, predicate);
  if (this->getResult().type == requite::ScopeWalkerResultType::FOUND) {
    return true;
  }
  return false;
}

bool ScopeWalker::doUseSearch(
    requite::Node &node, requite::Scope &scope,
    const std::function<requite::ScopeWalkerResultType(requite::Scope &scope)>
        &predicate) {
  REQUITE_ASSERT(node.getType() == requite::NodeType::USE);
  requite::Expression &use_expression = node.getExpression();
  REQUITE_ASSERT(use_expression.getOpcode() == requite::Opcode::USE);
  requite::Expression &branch_expression = use_expression.getBranch();
  requite::Expression &first_path_expression =
      (branch_expression.getHasBranch()) ? branch_expression.getBranch()
                                         : branch_expression;
  llvm::StringRef first_path_name = first_path_expression.getDataText();
  requite::ScopeWalkerResult result =
      scope.walkScopes(this->getContext()).doSearch([&](requite::Scope scope) {
        requite::RootSymbol root =
            scope.lookupInternalRootSymbol(first_path_name);
        if (root.getIsNone()) {
          return requite::ScopeWalkerResultType::NOT_FOUND;
        } else if (root.getHasScope()) {
          requite::Scope &root_scope = root.getScope();
          if (first_path_expression.getHasNext())
          {
            this->searchForRoot(root_scope, first_path_expression.getNext(), predicate);
          } else {
            
          }          
        }
      });
}

} // namespace requite
#include <rq/context.hpp>
#include <rq/generate.hpp>
#include <rq/literals.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallString.h>

#include <optional>

namespace rq {

void Generator::generateSourceModule() {
  const rq::Expression &root =
      this->getContext().getSourceModule().getExpression();
  if (!root.getHasBranch()) {
    return;
  }
  const rq::Expression &first = root.getBranch();
  this->generateGlobalForest(first, this->getContext().getTop(),
                             this->getContext().getSourceModule());
  for (auto &symbol : this->getContext().getTop().getUnamedSymbolsListRef()) {
    if (llvm::isa<rq::Procedure>(symbol)) {
      rq::Procedure &procedure = llvm::cast<rq::Procedure>(symbol);
      if (procedure.getContainingModule() !=
          this->getContext().getSourceModule()) {
        continue;
      }
      if (procedure.getIsImplemented()) {
        continue;
      }
      if (!this->implementProcedure(procedure)) {
        this->setNotOk();
      }
      continue;
    }
    if (llvm::isa<rq::Global>(symbol)) {
      rq::Global &global = llvm::cast<rq::Global>(symbol);
      if (global.getContainingModule() !=
          this->getContext().getSourceModule()) {
        continue;
      }
      if (global.getIsImplemented()) {
        continue;
      }
      if (!this->implementGlobal(global)) {
        this->setNotOk();
      }
      continue;
    }
  }
  for (const auto &[_, list] :
       this->getContext().getTop().getNamedListsSubrange()) {
    std::ignore = list;
  }
}

void Generator::generateGlobalForest(const rq::Expression &first_expression,
                                     rq::Table &hosting_table,
                                     rq::Module &module) {
  using K = rq::Keyword;
  for (const rq::Expression &branch_expression :
       first_expression.getInclusiveNextSubrange()) {
    const rq::Expression &unascribed_expression =
        branch_expression.getUnascribed();
    rq::ExpressionFlagsFactory factory{};
    if (branch_expression.getKeyword() == K::ASCRIBE_EXPRESSION) {
      factory.addAllAttributres(unascribed_expression);
    }
    rq::Table &containing_table = this->determineContainingTable(
        factory, unascribed_expression, hosting_table, module);
    switch (unascribed_expression.getKeyword()) {
    case K::ASSIGN: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      const rq::Expression &lvalue_expression =
          unascribed_expression.getBranch();
      if (lvalue_expression.getKeyword() != K::BINDING) {
        this->getContext().logErrorGlobalIndeterminateDynamicExpression(
            branch_expression);
        this->setNotOk();
        continue;
      }
      const rq::Expression &name_expression = lvalue_expression.getBranch();
      const rq::Expression &expression = name_expression.getNext();
      const rq::Expression &rvalue_expression = lvalue_expression.getNext();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        return;
      }
      llvm::StringRef name = name_o.value();
      rq::Global &variable = this->getContext().allocateValue<rq::Global>(
          name, unascribed_expression, factory.getFlags(), module,
          containing_table);
      variable.setTypeExpression(expression);
      variable.setValueExpression(rvalue_expression);
      containing_table.addNamedSymbol(this->getContext(), name, variable);
    }
    case K::DESTRUCTOR:
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    case K::DESTROY_OF:
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    case K::CALL: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::DROP_OF: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::RANGER: {
      rq::Ranger &ranger = this->getContext().allocateValue<rq::Ranger>(
          unascribed_expression, factory.getFlags(), module, containing_table,
          hosting_table);
      containing_table.addUnamedSymbol(this->getContext(), ranger);
    } break;
    case K::REVERSE_RANGER: {
      rq::ReverseRanger &reverse_ranger =
          this->getContext().allocateValue<rq::ReverseRanger>(
              unascribed_expression, factory.getFlags(), module,
              containing_table, hosting_table);
      containing_table.addUnamedSymbol(this->getContext(), reverse_ranger);
    } break;
    case K::ENTRY: {
      if (factory.getHasStatic()) {
        this->getContext().logErrorInvalidExpressionAttribute(
            unascribed_expression, factory.getStatic());
        this->setNotOk();
        continue;
      }
      if (!llvm::isa<rq::Top>(containing_table)) {
        this->getContext().logErrorNotInTop(unascribed_expression);
        this->setNotOk();
      }
      rq::Entry &entry = this->getContext().allocateValue<rq::Entry>(
          unascribed_expression, factory.getFlags(), module, containing_table,
          hosting_table);
      containing_table.addUnamedSymbol(this->getContext(), entry);
      if (unascribed_expression.getHasBranch()) {
        const rq::Expression &body_start_expression =
            unascribed_expression.getBranch();
        entry.setBodyStartExpression(body_start_expression);
      }
    } break;
    case K::FUNCTION: {
      const rq::Expression &name_expression = branch_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::Function &function = this->getContext().allocateValue<rq::Function>(
          name, unascribed_expression, factory.getFlags(), module,
          containing_table, hosting_table);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          function.setSignatureExpression(next_expression);
        }
      }
      containing_table.addNamedSymbol(this->getContext(), name, function);
    } break;
    case K::METHOD: {
      const rq::Expression &name_expression = branch_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::Method &method = this->getContext().allocateValue<rq::Method>(
          name, unascribed_expression, factory.getFlags(), module,
          containing_table, hosting_table);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          method.setSignatureExpression(next_expression);
        }
      }
      containing_table.addNamedSymbol(this->getContext(), name, method);
    } break;
    case K::EXTENSION_FUNCTION: {
      const rq::Expression &name_expression = branch_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::ExtensionFunction &function =
          this->getContext().allocateValue<rq::ExtensionFunction>(
              name, unascribed_expression, factory.getFlags(), module,
              containing_table, hosting_table);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          function.setSignatureExpression(next_expression);
        }
      }
      containing_table.addNamedSymbol(this->getContext(), name, function);
    } break;
    case K::EXTENSION_METHOD: {
      const rq::Expression &name_expression = branch_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::ExtensionMethod &method =
          this->getContext().allocateValue<rq::ExtensionMethod>(
              name, unascribed_expression, factory.getFlags(), module,
              containing_table, hosting_table);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          method.setSignatureExpression(next_expression);
        }
      }
      containing_table.addNamedSymbol(this->getContext(), name, method);
    } break;
    case K::IMPLEMENT_FUNCTION:
      RQ_TODO_IMPLEMENTATION();
    case K::IMPLEMENT_METHOD:
      RQ_TODO_IMPLEMENTATION();
    case K::IMPLEMENT_EXTENSION_FUNCTION:
      RQ_TODO_IMPLEMENTATION();
    case K::IMPLEMENT_EXTENSION_METHOD:
      RQ_TODO_IMPLEMENTATION();
    case K::SPECIALIZE_FUNCTION:
      RQ_TODO_IMPLEMENTATION();
    case K::SPECIALIZE_METHOD:
      RQ_TODO_IMPLEMENTATION();
    case K::SPECIALIZE_EXTENSION_FUNCTION:
      RQ_TODO_IMPLEMENTATION();
    case K::SPECIALIZE_EXTENSION_METHOD:
      RQ_TODO_IMPLEMENTATION();
    case K::RETURN: {
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          unascribed_expression);
      this->setNotOk();
      continue;
    }
    case K::BREAK: {
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          unascribed_expression);
      this->setNotOk();
      continue;
    }
    case K::CONTINUE: {
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          unascribed_expression);
      this->setNotOk();
      continue;
    }
    case K::FALLTHROUGH: {
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          unascribed_expression);
      this->setNotOk();
      continue;
    }
    case K::GOTO: {
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          unascribed_expression);
      this->setNotOk();
      continue;
    }
    case K::RANGE_OVER: {
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          unascribed_expression);
      this->setNotOk();
      continue;
    }
    case K::CLASS: {
      const rq::Expression &name_expression = unascribed_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::Class &class_ = this->getContext().allocateValue<rq::Class>(
          name, unascribed_expression, factory.getFlags(), module,
          containing_table, hosting_table);
      containing_table.addNamedSymbol(this->getContext(), name, class_);
      for (const rq::Expression &member_expression :
           name_expression.getNextSubrange()) {
        if (member_expression.getIsHeader()) {
          class_.setClassLayoutExpression(member_expression);
          break;
        }
      }
    } break;
    case K::ENUMERATION: {
      const rq::Expression &name_expression = unascribed_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::Enumeration &enumeration =
          this->getContext().allocateValue<rq::Enumeration>(
              name, unascribed_expression, factory.getFlags(), module,
              containing_table, hosting_table);
      containing_table.addNamedSymbol(this->getContext(), name, enumeration);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          enumeration.setUnderlyingTypeExpression(next_expression);
        }
      }
    } break;
    case K::CODE: {
      const rq::Expression &name_expression = unascribed_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::Code &code = this->getContext().allocateValue<rq::Code>(
          name, unascribed_expression, factory.getFlags(), module,
          containing_table, hosting_table);
      containing_table.addNamedSymbol(this->getContext(), name, code);
    } break;
    case K::CATEGORY: {
      const rq::Expression &name_expression = unascribed_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::Category &category = this->getContext().allocateValue<rq::Category>(
          name, unascribed_expression, factory.getFlags(), module,
          containing_table, hosting_table);
      containing_table.addNamedSymbol(this->getContext(), name, category);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          category.setDiscriminantTypeExpression(next_expression);
        }
      }
    } break;
    case K::IF: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::ELSE_IF: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::ELSE: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::MATCH: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::SWITCH: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::CASE: {
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::WITH: {
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::FOR: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::WHILE: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::SCOPE: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::BLOCK: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      if (factory.getHasOutside()) {
        this->getContext().logErrorInvalidExpressionAttribute(
            unascribed_expression, factory.getOutside());
        this->setNotOk();
        continue;
      }
      if (!unascribed_expression.getHasBranch()) {
        continue;
      }
      const rq::Expression &block_first_expression =
          unascribed_expression.getBranch();
      this->generateGlobalForest(block_first_expression, hosting_table, module);
    } break;
    case K::IMPORT: {
      if (!llvm::isa<rq::Top>(containing_table)) {
        this->getContext().logErrorNotInTop(unascribed_expression);
        this->setNotOk();
      }
      const rq::Expression &path_cstr_expression =
          unascribed_expression.getBranch();
      std::optional<llvm::StringRef> path_cstr_o =
          this->evaluateUtf8Cstr(path_cstr_expression, hosting_table);
      if (!path_cstr_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateUtf8Cstr(
            path_cstr_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef path_cstr = path_cstr_o.value();
      rq::Module *imported_ptr =
          this->getContext().loadImportModule(path_cstr_expression, path_cstr);
      if (imported_ptr == nullptr) {
        this->getContext().logErrorFailedToImportModule(path_cstr_expression,
                                                        path_cstr);
        this->setNotOk();
        continue;
      }
      // rq::Module &imported = rq::dereferencePtr(imported_ptr);
      rq::Import &import = this->getContext().allocateValue<rq::Import>(
          unascribed_expression, factory.getFlags(), module);
      containing_table.addUnamedSymbol(this->getContext(), import);
    }
    case K::USE: {
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::NAMESPACE: {
      const rq::Expression &name_expression = branch_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::BumpPtrListRef<rq::Symbol> name_list =
          hosting_table.getNamedListRef(name);
      rq::Namespace *namespace_ptr = nullptr;
      for (rq::Symbol &symbol : name_list) {
        if (llvm::isa<rq::Namespace>(symbol)) {
          rq::Namespace &namespace_ = llvm::cast<rq::Namespace>(symbol);
          rq::assignSingleValue(namespace_ptr, &namespace_);
        }
      }
      if (namespace_ptr == nullptr) {
        rq::Namespace &namespace_ =
            this->getContext().allocateValue<rq::Namespace>(name,
                                                            containing_table);
        containing_table.addNamedSymbol(this->getContext(), name, namespace_);
        namespace_ptr = &namespace_;
      }
      rq::Namespace &namespace_ = rq::dereferencePtr(namespace_ptr);
      if (name_expression.getHasNext()) {
        const rq::Expression &first_statement = name_expression.getNext();
        this->generateGlobalForest(first_statement, namespace_, module);
      }
      break;
    }
    case K::DEBUG_TRAP: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::UNREACHABLE: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::ASSUME: {
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
    case K::EXPAND_STATEMENT: {
      RQ_TODO_IMPLEMENTATION();
      break;
    }
    default:
      RQ_UNREACHABLE();
    }
  }
}

rq::Instruction *
Generator::generateLocalForest(const rq::Expression &first_expression,
                               rq::Table &hosting_table,
                               rq::Procedure &procedure) {
  std::ignore = hosting_table;
  std::ignore = procedure;
  using K = rq::Keyword;
  for (const rq::Expression &branch_expression :
       first_expression.getInclusiveNextSubrange()) {
    const rq::Expression &unascribed_expression =
        branch_expression.getUnascribed();
    rq::ExpressionFlagsFactory factory{};
    if (branch_expression.getKeyword() == K::ASCRIBE_EXPRESSION) {
      factory.addAllAttributres(unascribed_expression);
    }
    // rq::Table &containing_table = this->resolveContainingTable(
    //      factory, unascribed_expression, hosting_table);
    // rq::ExecutionFactory exec{};
    switch (unascribed_expression.getKeyword()) {
    case K::ASSIGN:

    case K::DESTRUCTOR:

    case K::DESTROY_OF:

    case K::CALL:

    case K::DROP_OF:

    case K::ENTRY:

    case K::FUNCTION:

    case K::METHOD:

    case K::RANGER:

    case K::EXTENSION_FUNCTION:

    case K::EXTENSION_METHOD:

    case K::IMPLEMENT_FUNCTION:

    case K::IMPLEMENT_METHOD:

    case K::IMPLEMENT_EXTENSION_FUNCTION:

    case K::IMPLEMENT_EXTENSION_METHOD:

    case K::SPECIALIZE_FUNCTION:

    case K::SPECIALIZE_METHOD:

    case K::SPECIALIZE_EXTENSION_FUNCTION:

    case K::SPECIALIZE_EXTENSION_METHOD:

    case K::RETURN:

    case K::BREAK:

    case K::CONTINUE:

    case K::FALLTHROUGH:

    case K::GOTO:

    case K::RANGE_OVER:

    case K::CLASS:

    case K::ENUMERATION:

    case K::CODE:

    case K::CATEGORY:

    case K::IF:

    case K::ELSE_IF:

    case K::ELSE:

    case K::MATCH:

    case K::INLINE_MATCH:

    case K::SWITCH:

    case K::INLINE_SWITCH:

    case K::CASE:

    case K::WITH:

    case K::DEFAULT:

    case K::FOR:

    case K::WHILE:

    case K::SCOPE:

    case K::BLOCK:

    case K::IMPORT:

    case K::USE:

    case K::NAMESPACE:

    case K::DEBUG_TRAP:

    case K::UNREACHABLE:

    case K::ASSUME:

    case K::EXPAND_STATEMENT:

    default:
      RQ_TODO_IMPLEMENTATION();
    }
  }
  RQ_UNREACHABLE();
}

std::optional<llvm::StringRef>
Generator::evaluateName(const rq::Expression &expression,
                        rq::Table &hosting_table) {
  RQ_ASSERT(expression.getKeyword() != rq::Keyword::NO_NAME,
            "must not be evaluated here");
  std::ignore = hosting_table;
  if (expression.getKeyword() == rq::Keyword::IDENTIFIER_LITERAL) {
    return expression.getSourceText();
  } else if (expression.getKeyword() == rq::Keyword::IDENTIFY) {
    RQ_TODO_IMPLEMENTATION();
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] std::optional<llvm::StringRef>
Generator::evaluateUtf8Cstr(const rq::Expression &expression,
                            rq::Table &hosting_table) {
  std::ignore = hosting_table;
  if (expression.getKeyword() == rq::Keyword::STRING_LITERAL) {
    llvm::SmallString<16> cstr{};
    // TODO
  }
  return std::nullopt;
}

[[nodiscard]] rq::Table &
Generator::determineContainingTable(const rq::ExpressionFlagsFactory &factory,
                                    const rq::Expression &unascribed_expression,
                                    rq::Table &hosting_table,
                                    rq::Module &module) {
  std::ignore = factory;
  std::ignore = unascribed_expression;
  std::ignore = module;
  // if (factory.getHasOutside()) {
  //   const rq::Expression &outside_expression = factory.getOutside();
  //   const rq::Expression &path_expression = outside_expression.getBranch();
  //   rq::Symbol *outside_ptr =
  //       this->evaluateSymbol(path_expression, hosting_table, module);
  //   if (outside_ptr == nullptr) {
  //     this->getContext().logErrorNotSymbol(path_expression);
  //     this->getContext().logErrorFailedToAscribeExpression(
  //         unascribed_expression, outside_expression);
  //     this->setNotOk();
  //     return hosting_table;
  //   }
  //   rq::Symbol &outside = rq::dereferencePtr(outside_ptr);
  //   if (!llvm::isa<rq::Label>(outside)) {
  //     this->getContext().logErrorNotLabel(path_expression);
  //     this->getContext().logErrorFailedToAscribeExpression(
  //         unascribed_expression, outside_expression);
  //     this->setNotOk();
  //     return hosting_table;
  //   }
  //   rq::Label &label = llvm::cast<rq::Label>(outside);
  //   rq::Entity &subject = label.getSubject();
  //   if (!llvm::isa<rq::Table>(subject)) {
  //     const rq::Expression &label_ascription = label.getAscription();
  //     this->getContext().logErrorLabelSubjectNotTable(label_ascription);
  //     this->getContext().logErrorFailedToAscribeExpression(
  //         unascribed_expression, outside_expression);
  //     this->setNotOk();
  //     return hosting_table;
  //   }
  //   rq::Table &subject_table =
  //       llvm::cast<rq::Table>(subject);
  //   rq::Table &containing_table =
  //       subject_table.getContainingTable();
  //   {
  //     bool left_frame = false;
  //     for (rq::Table &table :
  //          subject_table.getInclusiveFrameSubrange()) {
  //       if (table == containing_table) {
  //         if (left_frame) {
  //           this->getContext().logErrorOutsideNotInFrame(outside_expression);
  //           this->getContext().logErrorFailedToAscribeExpression(
  //               unascribed_expression, outside_expression);
  //           this->setNotOk();
  //           return hosting_table;
  //         }
  //         return containing_table;
  //       }
  //       if (table.getIsTopOfFrame()) {
  //         left_frame = true;
  //       }
  //     }
  //     this->getContext().logErrorOutsideNotAncestor(outside_expression);
  //     this->getContext().logErrorFailedToAscribeExpression(
  //         unascribed_expression, outside_expression);
  //     this->setNotOk();
  //   }
  // }
  return hosting_table;
}

[[nodiscard]] const rq::TypeConstant *
Generator::inferenceType(const rq::Expression &expression,
                         rq::Table &hosting_table, rq::Module &module) {
  std::ignore = module;
  // NOTE: this switch contains anything that can be rvalue situation
  using K = rq::Keyword;
  using O = rq::Opcode;
  bool has_error = false;
  switch (expression.getKeyword()) {
  case K::INTEGER_LITERAL:
    return &this->getContext().acquireTypeConstant(
        this->getContext().acquireIntegerLiteral(), {});
  case K::FLOAT_LITERAL:
    return &this->getContext().acquireTypeConstant(
        this->getContext().acquireFloatLiteral(), {});
  case K::STRING_LITERAL:
    return &this->getContext().acquireTypeConstant(
        this->getContext().acquireStringLiteral(), {});
  case K::CODEUNIT_LITERAL:
    return &this->getContext().acquireTypeConstant(
        this->getContext().acquireCodeunitLiteral(), {});
  case K::IDENTIFIER_LITERAL: {
    llvm::SmallVector<rq::Symbol *> matches{};
    for (rq::Table &table : hosting_table.getInclusiveFrameSubrange()) {
      rq::BumpPtrListRef<rq::Symbol> list =
          table.getNamedListRef(expression.getSourceText());
      for (rq::Symbol &symbol : list) {
        switch (symbol.getOpcode()) {
        case O::SY_CODE: {
          rq::Code &code = llvm::cast<rq::Code>(symbol);
          if (!code.getHasExport() && code.getContainingModule() != module) {
            continue;
          }
          this->getContext().logErrorInvalidValueSymbol(expression, code);
          this->setNotOk();
          has_error = true;
          matches.push_back(&symbol);
        } break;
        case O::SY_LABEL: {
          rq::Label &label = llvm::cast<rq::Label>(symbol);
          if (label.getContainingModule() != module) {
            continue;
          }
          this->getContext().logErrorInvalidValueSymbol(expression, label);
          this->setNotOk();
          has_error = true;
          matches.push_back(&symbol);
        } break;
        case O::SY_LOCAL: {
          matches.push_back(&symbol);
        } break;
        case O::SY_STATIC: {
          matches.push_back(&symbol);
        } break;
        case O::SY_ENUMERATOR: {
          this->getContext().logErrorInvalidValueSymbol(expression, symbol);
          this->setNotOk();
          has_error = true;
          matches.push_back(&symbol);
        } break;
        case O::SY_CATEGORY_ALTERNATIVE: {
          this->getContext().logErrorInvalidValueSymbol(expression, symbol);
          this->setNotOk();
          has_error = true;
          matches.push_back(&symbol);
        } break;
        case O::SY_GLOBAL: {
          rq::Global &global = llvm::cast<rq::Global>(symbol);
          if (!global.getHasExport() &&
              global.getContainingModule() != module) {
            continue;
          }
          matches.push_back(&symbol);
        } break;
        case O::SY_GLOBAL_STATIC: {
          rq::GlobalStatic &global_static =
              llvm::cast<rq::GlobalStatic>(symbol);
          if (!global_static.getHasExport() &&
              global_static.getContainingModule() != module) {
            continue;
          }
          matches.push_back(&symbol);
        } break;
        case O::SY_NAMESPACE: {
          this->getContext().logErrorInvalidValueSymbol(expression, symbol);
          this->setNotOk();
          has_error = true;
          matches.push_back(&symbol);
        } break;
        case O::SY_CLASS: {
          rq::Class &class_ = llvm::cast<rq::Class>(symbol);
          if (!class_.getHasExport() &&
              class_.getContainingModule() != module) {
            continue;
          }
          matches.push_back(&symbol);
        } break;
        case O::SY_ENUMERATION: {
          rq::Enumeration &enumeration = llvm::cast<rq::Enumeration>(symbol);
          if (!enumeration.getHasExport() &&
              enumeration.getContainingModule() != module) {
            continue;
          }
          this->getContext().logErrorInvalidValueSymbol(expression, symbol);
          this->setNotOk();
          has_error = true;
          matches.push_back(&symbol);
        } break;
        case O::SY_CATEGORY: {
          rq::Category &category = llvm::cast<rq::Category>(symbol);
          if (!category.getHasExport() &&
              category.getContainingModule() != module) {
            continue;
          }
          this->getContext().logErrorInvalidValueSymbol(expression, symbol);
          this->setNotOk();
          has_error = true;
          matches.push_back(&symbol);
        } break;
        case O::SY_FUNCTION:
          [[fallthrough]];
        case O::SY_METHOD:
          [[fallthrough]];
        case O::SY_RANGER:
          [[fallthrough]];
        case O::SY_EXTENSION_FUNCTION:
          [[fallthrough]];
        case O::SY_EXTENSION_METHOD: {
          rq::Procedure &procedure = llvm::cast<rq::Procedure>(symbol);
          if (!procedure.getHasExport() &&
              procedure.getContainingModule() != module) {
            continue;
          }
          this->getContext().logErrorInvalidValueSymbol(expression, procedure);
          this->setNotOk();
          has_error = true;
          matches.push_back(&symbol);
        } break;
        case O::SY_TEMPLATE_CLASS:
          [[fallthrough]];
        case O::SY_TEMPLATE_ENUMERATION:
          [[fallthrough]];
        case O::SY_TEMPLATE_CATEGORY:
          [[fallthrough]];
        case O::SY_TEMPLATE_GLOBAL:
          [[fallthrough]];
        case O::SY_TEMPLATE_GLOBAL_STATIC:
          [[fallthrough]];
        case O::SY_TEMPLATE_FUNCTION:
          [[fallthrough]];
        case O::SY_TEMPLATE_METHOD:
          [[fallthrough]];
        case O::SY_TEMPLATE_EXTENSION_FUNCTION:
          [[fallthrough]];
        case O::SY_TEMPLATE_EXTENSION_METHOD: {
          rq::Template &template_ = llvm::cast<rq::Template>(symbol);
          if (!template_.getHasExport() &&
              template_.getContainingModule() != module) {
            continue;
          }
          this->getContext().logErrorInvalidValueSymbol(expression, template_);
          this->setNotOk();
          has_error = true;
          matches.push_back(&symbol);
        } break;
        case O::SY_PARTIAL_CLASS:
          [[fallthrough]];
        case O::SY_PARTIAL_ENUMERATION:
          [[fallthrough]];
        case O::SY_PARTIAL_CATEGORY:
          [[fallthrough]];
        case O::SY_PARTIAL_GLOBAL:
          [[fallthrough]];
        case O::SY_PARTIAL_GLOBAL_STATIC:
          [[fallthrough]];
        case O::SY_PARTIAL_FUNCTION:
          [[fallthrough]];
        case O::SY_PARTIAL_METHOD:
          [[fallthrough]];
        case O::SY_PARTIAL_EXTENSION_FUNCTION:
          [[fallthrough]];
        case O::SY_PARTIAL_EXTENSION_METHOD:
          // NOTE: only need to worry about catching templates because partial
          // specializations are only valid if associated template in the same
          // table is accessable.
          break;
        default:
          RQ_UNREACHABLE();
        }
      }
      if (!matches.empty()) {
        break;
      }
    }
    if (matches.empty()) {
      this->getContext().logErrorNotSymbol(expression);
      this->setNotOk();
      return nullptr;
    }
    if (matches.size() > 1) {
      this->getContext().logErrorNameCollision(expression);
      this->setNotOk();
      for (rq::Symbol *symbol_ptr : matches) {
        rq::Symbol &symbol = rq::dereferencePtr(symbol_ptr);
        this->getContext().logInfoNameCollisionDeclaration(symbol);
      }
      return nullptr;
    }
    if (has_error) {
      return nullptr;
    }
    rq::Symbol &match = rq::dereferencePtr(matches.front());
    switch (match.getOpcode()) {
    case O::SY_LOCAL: {
      rq::Local &local = llvm::cast<rq::Local>(match);
      if (local.getType().getIsInferencing()) {
        this->getContext().logErrorIndeterminateVariableValue(expression,
                                                              local);
        this->setNotOk();
        return nullptr;
      }
      return &local.getType();
    } break;
    case O::SY_STATIC: {
      rq::Static &static_ = llvm::cast<rq::Static>(match);
      if (static_.getType().getIsInferencing()) {
        this->getContext().logErrorIndeterminateVariableValue(expression,
                                                              static_);
        this->setNotOk();
        return nullptr;
      }
      return &static_.getType();
    } break;
    case O::SY_GLOBAL: {
      rq::Global &global = llvm::cast<rq::Global>(match);
      if (!global.getIsImplemented()) {
        if (!this->implementGlobal(global)) {
          this->getContext().logErrorIndeterminateVariableValue(expression,
                                                                global);
          this->setNotOk();
          return nullptr;
        }
      }
      return &global.getType();
    } break;
    case O::SY_GLOBAL_STATIC: {
      rq::GlobalStatic &global_static = llvm::cast<rq::GlobalStatic>(match);
      if (!global_static.getIsImplemented()) {
        if (!this->implementGlobalStatic(global_static)) {
          this->getContext().logErrorIndeterminateVariableValue(expression,
                                                                global_static);
          this->setNotOk();
          return nullptr;
        }
      }
      return &global_static.getType();
    } break;
    case O::SY_CLASS:
      return &this->getContext().acquireTypeConstant(match, {});
    default:
      RQ_UNREACHABLE();
    }
  } break;
  case K::LOGICAL_AND:
    [[fallthrough]];
  case K::LOGICAL_OR:
    [[fallthrough]];
  case K::LOGICAL_COMPLEMENT:
    [[fallthrough]];
  case K::GREATER:
    [[fallthrough]];
  case K::GREATER_EQUAL:
    [[fallthrough]];
  case K::LESS:
    [[fallthrough]];
  case K::LESS_EQUAL:
    [[fallthrough]];
  case K::EQUAL:
    [[fallthrough]];
  case K::NOT_EQUAL:
    return &this->getContext().acquireTypeConstant(
        this->getContext().acquireBoolean(), {});
  case K::EXTEND:
    RQ_TODO_IMPLEMENTATION();
  case K::INITIALIZE_RECIEVER:
    return &this->getContext().acquireTypeConstant(
        this->getContext().acquireType(), {});
  case K::BINDING:
    RQ_UNREACHABLE();
  case K::ASCRIBE_TYPE:
    return &this->getContext().acquireTypeConstant(
        this->getContext().acquireType(), {});
  case K::ASCRIBE_ROOT_OF_VALUE: {
    RQ_TODO_IMPLEMENTATION();
  }
  case K::IDENTIFY: {
    RQ_TODO_IMPLEMENTATION();
  }
  case K::ADD:
    [[fallthrough]];
  case K::SUBTRACT:
    [[fallthrough]];
  case K::MULTIPLY:
    [[fallthrough]];
  case K::DIVIDE:
    [[fallthrough]];
  case K::MODULUS: {
    RQ_TODO_IMPLEMENTATION();
  }
  case K::NEGATE: {
    const rq::Expression &negated_expression = expression.getBranch();
    return this->inferenceType(negated_expression, hosting_table, module);
  }
  case K::AS_OF: {
    const rq::Expression &rvalue_expression = expression.getBranch();
    const rq::Expression &type_expression = rvalue_expression.getNext();
    const rq::TypeConstant *type_ptr =
        this->inferenceType(type_expression, hosting_table, module);
    if (type_ptr == nullptr) {
      return nullptr;
    }
    const rq::TypeConstant &type = rq::dereferencePtr(type_ptr);
    if (!type.getIsType()) {
      RQ_TODO_IMPLEMENTATION();
      // this->getContext().logErrorNotType(expression, type);
      this->setNotOk();
      return nullptr;
    }
    RQ_TODO_IMPLEMENTATION();
    // return this->evaluateType(type_expression, hosting_table, module);
  }
  case K::OF_OF: {

  }
  default:
    break;
  }
  RQ_UNREACHABLE();
}

bool Generator::implementProcedure(rq::Procedure &procedure) {
  RQ_ASSERT(!procedure.getIsImplemented(), "already implemented");

  // STEP 1: resolve signature
  if (llvm::isa<rq::Entry>(procedure)) {
    rq::TypeConstant &signature = this->getContext().acquireEntrySignature();
    procedure.setSignature(signature);
  }

  // STEP 2: add signature parameters to symbol table
  if (!llvm::isa<rq::Entry>(procedure)) {
    RQ_TODO_IMPLEMENTATION();
  }

  // STEP 3: generate
  if (procedure.getHasBodyStartExpression()) {
    const rq::Expression &body_start = procedure.getBodyStartExpression();
    rq::Instruction *instruction_ptr =
        this->generateLocalForest(body_start, procedure, procedure);
    procedure.setInstruction(instruction_ptr);
  }

  return true;
}

bool Generator::implementGlobal(rq::Global &global) {
  std::ignore = global;
  RQ_TODO_IMPLEMENTATION();
}

bool Generator::implementGlobalStatic(rq::GlobalStatic &global_static) {
  std::ignore = global_static;
  RQ_TODO_IMPLEMENTATION();
}

} // namespace rq

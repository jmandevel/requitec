#include <rq/context.hpp>
#include <rq/tabulator.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallString.h>

#include <optional>

namespace rq {

void Tabulator::tabulateModule() {
  const rq::Expression &root = this->getModule().getExpression();
  if (!root.getHasBranch()) {
    return;
  }
  const rq::Expression &first = root.getBranch();
  this->tabulateGlobalForest(first, this->getContext().getTop());
}

void Tabulator::tabulateGlobalForest(const rq::Expression &first_expression,
                                     rq::SymbolTable &hosting_table) {
  using K = rq::Keyword;
  for (const rq::Expression &branch_expression :
       first_expression.getInclusiveNextSubrange()) {
    const rq::Expression &unascribed_expression =
        branch_expression.getUnascribed();
    rq::ExpressionFlagsFactory factory{};
    if (branch_expression.getKeyword() == K::ASCRIBE_EXPRESSION) {
      factory.addAllAttributres(unascribed_expression);
    }
    rq::SymbolTable &containing_table = this->resolveContainingTable(
        factory, unascribed_expression, hosting_table);
    switch (unascribed_expression.getKeyword()) {
    case K::BINDING: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      const rq::Expression &name_expression = unascribed_expression.getBranch();
      const rq::Expression &type_expression = name_expression.getNext();
      this->tabulateGlobalVariable(containing_table, hosting_table, factory,
                                   unascribed_expression, name_expression,
                                   type_expression, nullptr);
      break;
    }
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
      const rq::Expression &type_expression = name_expression.getNext();
      const rq::Expression &rvalue_expression = lvalue_expression.getNext();
      this->tabulateGlobalVariable(containing_table, hosting_table, factory,
                                   unascribed_expression, name_expression,
                                   type_expression, &rvalue_expression);
      break;
    }
    case K::STRUCTURED_BINDING: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      RQ_TODO_IMPLEMENTATION();
      break;
    }
    case K::IGNORE: {
      if (factory.getHasStatic()) {
        RQ_TODO_IMPLEMENTATION();
        break;
      }
      this->getContext().logErrorGlobalIndeterminateDynamicExpression(
          branch_expression);
      this->setNotOk();
      continue;
    }
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
          unascribed_expression, factory.getFlags(), this->getModule(),
          containing_table, hosting_table);
      containing_table.addUnamedSymbol(this->getContext(), entry);
      break;
    }
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
          name, unascribed_expression, factory.getFlags(), this->getModule(),
          containing_table, hosting_table);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          function.setSignatureExpression(next_expression);
        }
      }
      containing_table.addNamedSymbol(this->getContext(), name, function);
      break;
    }
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
          name, unascribed_expression, factory.getFlags(), this->getModule(),
          containing_table, hosting_table);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          method.setSignatureExpression(next_expression);
        }
      }
      containing_table.addNamedSymbol(this->getContext(), name, method);
      break;
    }
    case K::RANGER: {
      const rq::Expression &name_expression = branch_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::Ranger &ranger = this->getContext().allocateValue<rq::Ranger>(
          name, unascribed_expression, factory.getFlags(), this->getModule(),
          containing_table, hosting_table);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          ranger.setSignatureExpression(next_expression);
        }
      }
      containing_table.addNamedSymbol(this->getContext(), name, ranger);
      break;
    }
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
              name, unascribed_expression, factory.getFlags(),
              this->getModule(), containing_table, hosting_table);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          function.setSignatureExpression(next_expression);
        }
      }
      containing_table.addNamedSymbol(this->getContext(), name, function);
      break;
    }
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
              name, unascribed_expression, factory.getFlags(),
              this->getModule(), containing_table, hosting_table);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          method.setSignatureExpression(next_expression);
        }
      }
      containing_table.addNamedSymbol(this->getContext(), name, method);
      break;
    }
    case K::EXTENSION_RANGER: {
      const rq::Expression &name_expression = branch_expression.getBranch();
      std::optional<llvm::StringRef> name_o =
          this->evaluateName(name_expression, hosting_table);
      if (!name_o.has_value()) {
        this->getContext().logErrorUnableToEvaluateName(name_expression);
        this->setNotOk();
        continue;
      }
      llvm::StringRef name = name_o.value();
      rq::ExtensionRanger &ranger =
          this->getContext().allocateValue<rq::ExtensionRanger>(
              name, unascribed_expression, factory.getFlags(),
              this->getModule(), containing_table, hosting_table);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          ranger.setSignatureExpression(next_expression);
        }
      }
      containing_table.addNamedSymbol(this->getContext(), name, ranger);
      break;
    }
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
          name, unascribed_expression, factory.getFlags(), this->getModule(),
          containing_table, hosting_table);
      containing_table.addNamedSymbol(this->getContext(), name, class_);
      for (const rq::Expression &member_expression :
           name_expression.getNextSubrange()) {
        if (member_expression.getIsHeader()) {
          class_.setClassLayoutExpression(member_expression);
          break;
        }
      }
      break;
    }
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
              name, unascribed_expression, factory.getFlags(),
              this->getModule(), containing_table, hosting_table);
      containing_table.addNamedSymbol(this->getContext(), name, enumeration);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          enumeration.setUnderlyingTypeExpression(next_expression);
        }
      }
      break;
    }
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
          name, unascribed_expression, factory.getFlags(), this->getModule(),
          containing_table, hosting_table);
      containing_table.addNamedSymbol(this->getContext(), name, code);
      break;
    }
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
          name, unascribed_expression, factory.getFlags(), this->getModule(),
          containing_table, hosting_table);
      containing_table.addNamedSymbol(this->getContext(), name, category);
      if (name_expression.getHasNext()) {
        const rq::Expression &next_expression = name_expression.getNext();
        if (next_expression.getIsHeader()) {
          category.setDiscriminantTypeExpression(next_expression);
        }
      }
      break;
    }
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
      this->tabulateGlobalForest(block_first_expression, hosting_table);
    }
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
          unascribed_expression, factory.getFlags(), this->getModule());
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
      // TODO look for existing namespace of name
      rq::Namespace &namespace_ =
          this->getContext().allocateValue<rq::Namespace>(name,
                                                          containing_table);
      containing_table.addNamedSymbol(this->getContext(), name, namespace_);
      if (name_expression.getHasNext()) {
        const rq::Expression &first_statement = name_expression.getNext();
        this->tabulateGlobalForest(first_statement, namespace_);
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

std::optional<llvm::StringRef>
Tabulator::evaluateName(const rq::Expression &expression,
                        rq::SymbolTable &hosting_table) {
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
Tabulator::evaluateUtf8Cstr(const rq::Expression &expression,
                            rq::SymbolTable &hosting_table) {
  std::ignore = hosting_table;
  if (expression.getKeyword() == rq::Keyword::STRING_LITERAL) {
    llvm::SmallString<16> cstr{};
    // TODO
  }
  return std::nullopt;
}

[[nodiscard]] rq::SymbolTable &
Tabulator::resolveContainingTable(const rq::ExpressionFlagsFactory &factory,
                                  const rq::Expression &unascribed_expression,
                                  rq::SymbolTable &hosting_table) {
  if (factory.getHasOutside()) {
    const rq::Expression &outside_expression = factory.getOutside();
    const rq::Expression &path_expression = outside_expression.getBranch();
    rq::Symbol *outside_ptr =
        this->resolveSymbol(path_expression, hosting_table);
    if (outside_ptr == nullptr) {
      this->getContext().logErrorNotSymbol(path_expression);
      this->getContext().logErrorFailedToAscribeExpression(
          unascribed_expression, outside_expression);
      this->setNotOk();
      return hosting_table;
    }
    rq::Symbol &outside = rq::dereferencePtr(outside_ptr);
    if (!llvm::isa<rq::Label>(outside)) {
      this->getContext().logErrorNotLabel(path_expression);
      this->getContext().logErrorFailedToAscribeExpression(
          unascribed_expression, outside_expression);
      this->setNotOk();
      return hosting_table;
    }
    rq::Label &label = llvm::cast<rq::Label>(outside);
    rq::Entity &subject = label.getSubject();
    if (!llvm::isa<rq::SymbolTable>(subject)) {
      const rq::Expression &label_ascription = label.getAscription();
      this->getContext().logErrorLabelSubjectNotSymbolTable(label_ascription);
      this->getContext().logErrorFailedToAscribeExpression(
          unascribed_expression, outside_expression);
      this->setNotOk();
      return hosting_table;
    }
    rq::SymbolTable &subject_symbol_table =
        llvm::cast<rq::SymbolTable>(subject);
    rq::SymbolTable &containing_table =
        subject_symbol_table.getContainingSymbolTable();
    {
      bool left_frame = false;
      for (rq::SymbolTable &table :
           subject_symbol_table.getInclusiveFrameSubrange()) {
        if (table == containing_table) {
          if (left_frame) {
            this->getContext().logErrorOutsideNotInFrame(outside_expression);
            this->getContext().logErrorFailedToAscribeExpression(
                unascribed_expression, outside_expression);
            this->setNotOk();
            return hosting_table;
          }
          return containing_table;
        }
        if (table.getIsTopOfFrame()) {
          left_frame = true;
        }
      }
      this->getContext().logErrorOutsideNotAncestor(outside_expression);
      this->getContext().logErrorFailedToAscribeExpression(
          unascribed_expression, outside_expression);
      this->setNotOk();
    }
  }
  return hosting_table;
}

[[nodiscard]] rq::Symbol *Tabulator::resolveSymbol(const rq::Expression &path,
                                                   rq::SymbolTable &table) {
  std::ignore = path;
  std::ignore = table;
  RQ_TODO_IMPLEMENTATION();
}

[[nodiscard]] rq::TypeConstant *
Tabulator::evaluateType(const rq::Expression &path, rq::SymbolTable &table) {
  std::ignore = path;
  std::ignore = table;
  RQ_TODO_IMPLEMENTATION();
}

void Tabulator::tabulateGlobalVariable(
    rq::SymbolTable &hosting_table, rq::SymbolTable &containing_table,
    const rq::ExpressionFlagsFactory &factory,
    const rq::Expression &unascribed_expression,
    const rq::Expression &name_expression,
    const rq::Expression &type_expression,
    const rq::Expression *value_expression_ptr) {
  std::optional<llvm::StringRef> name_o =
      this->evaluateName(name_expression, hosting_table);
  if (!name_o.has_value()) {
    this->getContext().logErrorUnableToEvaluateName(name_expression);
    this->setNotOk();
    return;
  }
  llvm::StringRef name = name_o.value();
  rq::GlobalVariable &variable =
      this->getContext().allocateValue<rq::GlobalVariable>(
          name, unascribed_expression, factory.getFlags(), this->getModule(),
          containing_table, hosting_table);
  variable.setTypeExpression(type_expression);
  variable.setValueExpression(value_expression_ptr);
  containing_table.addNamedSymbol(this->getContext(), name, variable);
}

} // namespace rq

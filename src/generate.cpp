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
      rq::Procedure &procedure = llvm::integer_cast<rq::Procedure>(symbol);
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
      rq::Global &global = llvm::integer_cast<rq::Global>(symbol);
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
                                      std::ignore = first_expression;
                                      std::ignore = hosting_table;
                                      std::ignore = module;
                                      RQ_TODO_IMPLEMENTATION(); }

rq::Instruction *
Generator::generateProcedureForest(const rq::Expression &first_expression,
                               rq::Table &hosting_table,
                               rq::Procedure &procedure) {
                                std::ignore = first_expression;
  std::ignore = hosting_table;
  std::ignore = procedure;
  RQ_TODO_IMPLEMENTATION();
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
Generator::determineContainingTable(const rq::Expression &unascribed_expression,
                                    rq::Table &hosting_table,
                                    rq::Module &module) {
  std::ignore = unascribed_expression;
  std::ignore = hosting_table;
  std::ignore = module;
  RQ_TODO_IMPLEMENTATION();
}

[[nodiscard]] const rq::TypeConstant *
Generator::inferenceType(const rq::Expression &expression,
                         rq::Table &hosting_table, rq::Module &module) {
  std::ignore = expression;
  std::ignore = hosting_table;
                          std::ignore = module;
  RQ_TODO_IMPLEMENTATION();
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
        this->generateProcedureForest(body_start, procedure, procedure);
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

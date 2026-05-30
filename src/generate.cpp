#include <rq/context.hpp>
#include <rq/generate.hpp>
#include <rq/literals.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallString.h>

#include <optional>

namespace rq {

void Generator::generateSourceModule() {
  // TODO
}

void Generator::generateGlobalForest(const rq::Expression &first_expression,
                                     rq::Table &hosting_table,
                                     rq::Module &module) {
  std::ignore = first_expression;
  std::ignore = hosting_table;
  std::ignore = module;
  RQ_TODO_IMPLEMENTATION();
}

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

[[nodiscard]] const rq::SymbolConstant *
Generator::inferenceType(const rq::Expression &expression,
                         rq::Table &hosting_table, rq::Module &module) {
  std::ignore = expression;
  std::ignore = hosting_table;
  std::ignore = module;
  RQ_TODO_IMPLEMENTATION();
}

bool Generator::implementProcedure(rq::Procedure &procedure) {
  std::ignore = procedure;
  // TODO
  return true;
}

bool Generator::implementGlobal(rq::GlobalDynamicVariable &global) {
  std::ignore = global;
  RQ_TODO_IMPLEMENTATION();
}

} // namespace rq

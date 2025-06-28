#include <requite/context.hpp>
#include <requite/scope.hpp>
#include <requite/expression.hpp>

namespace requite {

void Context::tabulateEntryPoint(requite::Module &module,
                                 requite::Expression &expression) {
  requite::Procedure& procedure = this->makeProcedure();
  procedure.setType(requite::ProcedureType::ENTRY_POINT);
  procedure.setModule(module);
  procedure.setExpression(expression);
  expression.setProcedure(procedure);
  module.addEntryPoint(procedure);
}

} // namespace requite
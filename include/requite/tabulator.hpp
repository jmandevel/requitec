#pragma once

#include <requite/attribute_flags.hpp>
#include <requite/user_symbol.hpp>

#include <functional>
#include <unordered_map>

namespace requite {

struct Context;
struct Module;
struct Scope;
struct Object;
struct Expression;
struct Property;
struct Block;

struct Tabulator final {
  using Self = requite::Tabulator;

  std::reference_wrapper<requite::Context> _context_ref;
  std::reference_wrapper<requite::Module> _module_ref;
  requite::Scope *_scope_ptr = nullptr;
  bool _is_ok = true;

  // tabulator.cpp
  Tabulator(requite::Context &constext, requite::Module &module);
  Tabulator(const Self &) = delete;
  Tabulator(Self &&) = delete;
  ~Tabulator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Context &getContext();
  [[nodiscard]] const requite::Context &getContext() const;
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] const requite::Module &getModule() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] requite::Object &getObject();
  [[nodiscard]] const requite::Object &getObject() const;
  void enterScope(requite::Scope &scope);
  void leaveScope();
  [[nodiscard]] bool getIsOk() const;
  void setNotOk();

  // tabulate.cpp
  [[nodiscard]] bool tabulateModule();
  void tabulateModuleStatement(requite::Expression &statement,
                               bool has_attributes);
  void tabulateTableStatement(requite::Expression &statement,
                              bool has_attributes);
  void tabulateObjectStatement(requite::Expression &statement,
                               bool has_attributes);
  void tabulateMatteLocalStatement(requite::Expression &statement,
                                   bool has_attributes);
  void tabulateEntryPoint(requite::Expression &expression, bool has_attributes);
  void tabulateFunction(requite::Expression &expression, bool has_attributes);
  void tabulateConstructor(requite::Expression &expression,
                           bool has_attributes);
  void tabulateDestructor(requite::Expression &expression, bool has_attributes);
  void tabulateObject(requite::Expression &expression, bool has_attributes);
  [[nodiscard]] requite::Table& getOrMakeTable(llvm::StringRef name);
  void tabulateTable(requite::Expression &expression, bool has_attributes);
  void tabulateAlias(requite::Expression &expression, bool has_attributes);
  void tabulateImport(requite::Expression &expression, bool has_attributes);
  void tabulateUse(requite::Expression &expression, bool has_attributes);
  void tabulate_Global(requite::Expression &expression, bool has_attributes);
  void tabulateTableUse(requite::Expression &expression, bool has_attributes);
  void tabulateTableAlias(requite::Expression &expression, bool has_attributes);
  void tabulate_Property(requite::Expression &expression, bool has_attributes);
  void tabulate_Local(requite::Expression &expression, bool has_attributes);
  void tabulate_AnonymousFunction(requite::Expression &expression);
  void tabulateIf(requite::Expression &expression, bool has_attributes);
  void tabulateElseIf(requite::Expression &expression, bool has_attributes);
  void tabulateElse(requite::Expression &expression, bool has_attributes);
  void tabulateSwitch(requite::Expression &expression, bool has_attributes);
  void tabulateSwitchCaseStatement(requite::Expression &expression);
  void tabulateCase(requite::Expression &expression);
  void tabulateDefaultCase(requite::Expression &expression);
  void tabulateFor(requite::Expression &expression, bool has_attributes);
  void tabulateWhile(requite::Expression &expression, bool has_attributes);
  void tabulateDoWhile(requite::Expression &expression, bool has_attributes);
  void tabulateLoop(requite::Expression &expression, bool has_attributes);
  void tabulateScope(requite::Expression &expression, bool has_attributes);
  void tabulate_ClosedInlineScope(requite::Expression &expression);
  void tabulateExpressionForest(requite::Expression& expression);
  void tabulateExpression(requite::Expression& expression);

  // detail/tabulate.hpp
  template <requite::AttributeCategory CATEGORY_PARAM>
  requite::AttributeFlags tabulateAttributes(requite::Expression &expression);
};

} // namespace requite

#include <requite/detail/tabulate.hpp>
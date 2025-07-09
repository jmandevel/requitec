#pragma once

#include <requite/log_mode.hpp>
#include <requite/situation.hpp>
#include <requite/yield.hpp>

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

struct Contextualizer0 final {
  using Self = requite::Contextualizer0;

  std::reference_wrapper<requite::Context> _context_ref;
  std::reference_wrapper<requite::Module> _module_ref;
  requite::Scope *_scope_ptr = nullptr;
  bool _is_ok = true;
  std::unordered_map<void *, requite::Yield> _yield_map = {};

  // contextualizer0.cpp
  Contextualizer0(requite::Context &constext, requite::Module &module);
  Contextualizer0(const Self &) = delete;
  Contextualizer0(Self &&) = delete;
  ~Contextualizer0() = default;
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
  [[nodiscard]] const std::unordered_map<void *, requite::Yield> &
  getYieldMap() const;
  [[nodiscard]] std::unordered_map<void *, requite::Yield> &getYieldMap();

  // yielding.cpp
  void addYield(requite::Object &object, requite::Situation situation);
  void removeYield(requite::Object &object);
  void addYield(requite::Procedure &procedure, requite::Situation situation);
  void removeYield(requite::Procedure &procedure);
  void addYield(requite::Table &table, requite::Situation situation);
  void removeYield(requite::Table &table);
  void addYield(requite::Property &property, requite::Situation situation);
  void removeYield(requite::Property &property);
  void addYield(requite::Global &global, requite::Situation situation);
  void removeYield(requite::Global &global);
  void addYield(requite::Alias &alias, requite::Situation situation);
  void removeYield(requite::Alias &alias);
  void addYield(requite::Block &block, requite::Situation situation);
  void removeYield(requite::Block &block);
  void addYield(requite::Expression &expand_statement,
                requite::Situation situation);
  void removeYield(requite::Expression &expand_statement);
  [[nodiscard]] bool tryFinishYields();
  [[nodiscard]] bool tryFinishObject(requite::Yield &yield);
  [[nodiscard]] bool tryFinishProcedure(requite::Yield &yield);
  [[nodiscard]] bool tryFinishTable(requite::Yield &yield);
  [[nodiscard]] bool tryFinishProperty(requite::Yield &yield);
  [[nodiscard]] bool tryFinishGlobal(requite::Yield &yield);
  [[nodiscard]] bool tryFinishAlias(requite::Yield &yield);
  [[nodiscard]] bool tryFinishBlock(requite::Yield &yield);
  [[nodiscard]] bool tryFinishExpandStatement(requite::Yield &yield);

  // tabulate.cpp
  [[nodiscard]] bool contextualizeModule();
  void tabulateStatement(requite::Expression &statement,
                         requite::Situation situation, bool has_attributes);
  void tabulateEntryPoint(requite::Expression &expression,
                          requite::Situation situation, bool has_attributes);
  void tabulateFunction(requite::Expression &expression,
                        requite::Situation situation, bool has_attributes);
  void tabulateMethod(requite::Expression &expression,
                      requite::Situation situation, bool has_attributes);
  void tabulateExtension(requite::Expression &expression,
                         requite::Situation situation, bool has_attributes);
  void tabulateConstructor(requite::Expression &expression,
                           requite::Situation situation, bool has_attributes);
  void tabulateDestructor(requite::Expression &expression,
                          requite::Situation situation, bool has_attributes);
  void tabulateObject(requite::Expression &expression,
                      requite::Situation situation, bool has_attributes);
  void tabulateAlias(requite::Expression &expression,
                     requite::Situation situation, bool has_attributes);
  void tabulateImport(requite::Expression &expression,
                      requite::Situation situation, bool has_attributes);
  void tabulateUse(requite::Expression &expression,
                   requite::Situation situation, bool has_attributes);
  void tabulateGlobal(requite::Expression &expression,
                      requite::Situation situation, bool has_attributes);
  void tabulateProperty(requite::Expression &expression,
                        requite::Situation situation, bool has_attributes);
  void tabulateBlock(requite::Expression &expression,
                     requite::Situation situation, bool has_attributes);
  void tabulate_ExpandValue(requite::Expression &expression,
                            requite::Situation situation, bool has_attributes);

  // expand.cpp
  [[nodiscard]] bool expandTree(requite::Expression &expression,
                                requite::Situation situation,
                                requite::LogMode log_mode);
  [[nodiscard]] bool expandForest(requite::Expression &expression,
                                requite::Situation situation,
                                requite::LogMode log_mode);
  [[nodiscard]] bool expandNameBranch(requite::Expression &expression,
                                      requite::Situation situation,
                                      requite::LogMode log_mode);
};

} // namespace requite
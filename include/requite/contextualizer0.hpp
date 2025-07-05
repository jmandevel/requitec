#pragma once

#include <functional>

namespace requite {

struct Context;
struct Module;
struct Scope;
struct Object;
struct Expression;

struct Contextualizer0 final {
  using Self = requite::Contextualizer0;

  std::reference_wrapper<requite::Context> _context_ref;
  std::reference_wrapper<requite::Module> _module_ref;
  requite::Scope *_scope_ptr = nullptr;

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
  void enterScope(requite::Scope &scope);
  void leaveScope();

  // tabulate.cpp
  [[nodiscard]] bool tabulateModule();
  [[nodiscard]] bool tabulateStatement(requite::Expression &statement);
  [[nodiscard]] bool tabulateStatement(requite::Expression &statement,
                                       bool has_attributes);
  [[nodiscard]] bool tabulateEntryPoint(requite::Expression &expression, bool has_attributes);
  [[nodiscard]] bool tabulateFunction(requite::Expression &expression,
                                      bool has_attributes);
  [[nodiscard]] bool tabulateMethod(requite::Expression &expression,
                                    bool has_attributes);
  [[nodiscard]] bool tabulateExtension(requite::Expression &expression,
                                       bool has_attributes);
  [[nodiscard]] bool tabulateConstructor(requite::Expression &expression,
                                         bool has_attributes);
  [[nodiscard]] bool tabulateDestructor(requite::Expression &expression,
                                        bool has_attributes);
  [[nodiscard]] bool tabulateObject(requite::Expression &expression,
                                    bool has_attributes);
  [[nodiscard]] bool tabulateAlias(requite::Expression &expression,
                                   bool has_attributes);
  [[nodiscard]] bool tabulateImport(requite::Expression &import,
                                    bool has_attributes);
  [[nodiscard]] bool tabulateUse(requite::Expression &use, bool has_attributes);
  [[nodiscard]] bool tabulateGlobal(requite::Expression &expression,
                                    bool has_attributes);
  [[nodiscard]] bool tabulateProperty(requite::Expression &expression,
                                      bool has_attributes);

  // expand.cpp
  [[nodiscard]] bool expandMacroExpression(requite::Expression& expression);
  [[nodiscard]] bool expandMacroBranchTree(requite::Expression& expression);
  [[nodiscard]] bool expandMacroTree(requite::Expression& expression);
  [[nodiscard]] bool expandMacroForest(requite::Expression& expression);
};

} // namespace requite
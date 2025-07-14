#pragma once

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
  void tabulateStatement(requite::Expression &statement, bool has_attributes);
  void tabulateEntryPoint(requite::Expression &expression, bool has_attributes);
  void tabulateFunction(requite::Expression &expression, bool has_attributes);
  void tabulateMethod(requite::Expression &expression, bool has_attributes);
  void tabulateExtension(requite::Expression &expression, bool has_attributes);
  void tabulateConstructor(requite::Expression &expression, bool has_attributes);
  void tabulateDestructor(requite::Expression &expression, bool has_attributes);
  void tabulateObject(requite::Expression &expression, bool has_attributes);
  void tabulateAlias(requite::Expression &expression, bool has_attributes);
  void tabulateImport(requite::Expression &expression, bool has_attributes);
  void tabulateUse(requite::Expression &expression, bool has_attributes);
  void tabulateGlobal(requite::Expression &expression, bool has_attributes);
  void tabulateProperty(requite::Expression &expression, bool has_attributes);
  void tabulateBlock(requite::Expression &expression, bool has_attributes);
};

} // namespace requite
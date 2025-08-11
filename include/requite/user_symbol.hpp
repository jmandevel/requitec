#pragma once

#include <requite/attribute_type.hpp>
#include <requite/scope_type.hpp>

#include <llvm/ADT/StringRef.h>

#include <string_view>

namespace requite {

struct Module;
struct Global;
struct Local;
struct Object;
struct Table;
struct Property;
struct Procedure;

enum class UserSymbolType {
  NONE,
  GLOBAL,
  LOCAL,
  OBJECT,
  PROPERTY,
  PROCEDURE
};

struct UserSymbol final {
  using Self = UserSymbol;

  requite::UserSymbolType _type = requite::UserSymbolType::NONE;
  union {
    void* _nothing_ptr = nullptr;
    requite::Global* _global_ptr;
    requite::Local* _local_ptr;
    requite::Object* _object_ptr;
    requite::Property* _property_ptr;
    requite::Procedure* _procedure_ptr;
  };
  bool _is_exported = false;
  requite::Module* _containing_module_ptr = nullptr;

  // user_symbol.cpp
  UserSymbol() = default;
  UserSymbol(requite::Module& module);
  UserSymbol(requite::Global& global, requite::Module& module);
  UserSymbol(requite::Local& local, requite::Module& module);
  UserSymbol(requite::Object& object, requite::Module& module);
  UserSymbol(requite::Property& property, requite::Module& module);
  UserSymbol(requite::Procedure& procedure, requite::Module& module);
  UserSymbol(const Self &) = default;
  UserSymbol(Self &&) = default;
  ~UserSymbol() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] bool operator==(const requite::Global&) const;
  [[nodiscard]] bool operator!=(const requite::Global&) const;
  [[nodiscard]] bool operator==(const requite::Local&) const;
  [[nodiscard]] bool operator!=(const requite::Local&) const;
  [[nodiscard]] bool operator==(const requite::Object&) const;
  [[nodiscard]] bool operator!=(const requite::Object&) const;
  [[nodiscard]] bool operator==(const requite::Property&) const;
  [[nodiscard]] bool operator!=(const requite::Property&) const;
  [[nodiscard]] bool operator==(const requite::Procedure&) const;
  [[nodiscard]] bool operator!=(const requite::Procedure&) const;
  [[nodiscard]] requite::UserSymbolType getType() const;
  [[nodiscard]] bool getIsExported() const;
  [[nodiscard]] bool getHasContainingModule() const;
  void setContainingModule(requite::Module& module);
  [[nodiscard]] requite::Module &getContainingModule();
  [[nodiscard]] const requite::Module &getContainingModule() const;
  [[nodiscard]] bool getIsGlobal() const;
  [[nodiscard]] requite::Global& getGlobal();
  [[nodiscard]] const requite::Global& getGlobal() const;
  [[nodiscard]] bool getIsLocal() const;
  [[nodiscard]] requite::Local& getLocal();
  [[nodiscard]] const requite::Local& getLocal() const;
  [[nodiscard]] bool getIsObject() const;
  [[nodiscard]] requite::Object& getObject();
  [[nodiscard]] const requite::Object& getObject() const;
  [[nodiscard]] bool getIsProperty() const;
  [[nodiscard]] requite::Property& getProperty();
  [[nodiscard]] const requite::Property& getProperty() const;
  [[nodiscard]] bool getIsProcedure() const;
  [[nodiscard]] requite::Procedure& getProcedure();
  [[nodiscard]] const requite::Procedure& getProcedure() const;
};

// detail/user_symbol.hpp
[[nodiscard]] inline constexpr std::string_view getName(requite::UserSymbolType type);

} // namespace requite

#include <requite/detail/user_symbol.hpp>
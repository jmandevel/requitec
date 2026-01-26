#pragma once
#include <rq/utility.hpp>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <functional>

namespace rq {

struct Context;
struct ModuleSymbol;
struct Expression;
struct SymbolTableSymbol;

struct Tabulator final {
  using Self = rq::Tabulator;

  std::reference_wrapper<rq::Context> _context_ref;
  std::reference_wrapper<rq::ModuleSymbol> _module_ref;
  rq::Expression *_trunk_ptr{nullptr};
  rq::SymbolTableSymbol *_highest_symbol_table_ptr{nullptr};
  rq::SymbolTableSymbol *_lowest_symbol_table_ptr{nullptr};
  llvm::DenseMap<rq::Expression *, llvm::SmallVector<rq::Symbol *>>
      _declaration_map{};
  bool _is_ok : 1 = true;
  bool _is_started : 1 = false;
  bool _is_building_instructions : 1 = false;
  rq::EntityKind _result_keyword = rq::EntityKind::KW_NONE;

  Tabulator(rq::Context &context, rq::ModuleSymbol &module)
      : _context_ref(context), _module_ref(module) {}
  Tabulator(const Self &) = delete;
  Tabulator(Self &&) = delete;
  ~Tabulator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleSymbol &getModule() {
    return this->_module_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ModuleSymbol &getModule() const {
    return this->_module_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const { return this->_is_ok; }
  void setNotOk() { this->_is_ok = false; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStarted() const {
    return this->_is_started;
  }
  RQ_ALWAYS_INLINE void setIsStarted() { this->_is_started = true; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBuildingInstructions() const {
    return this->_is_building_instructions;
  }
  RQ_ALWAYS_INLINE void setIsBuildingInstructions() {
    RQ_ASSERT(this->_is_building_instructions == false,
              "is building instructions already set");
    this->_is_building_instructions = true;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind getResultKeyword() const {
    return this->_result_keyword;
  }
  RQ_ALWAYS_INLINE void setResultKeyword(rq::EntityKind keyword) {
    RQ_ASSERT_KEYWORD(keyword);
    RQ_ASSERT(this->_result_keyword == rq::EntityKind::KW_NONE,
              "result keyword already set");
    this->_result_keyword = keyword;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSymbolTables() const {
    RQ_ASSERT((this->_highest_symbol_table_ptr == nullptr) ==
                  (this->_lowest_symbol_table_ptr == nullptr),
              "must have both or no tables");
    return this->_highest_symbol_table_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &
  getHighestSymbolTable() {
    return rq::dereferencePtr(this->_highest_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &
  getHighestSymbolTable() const {
    return rq::dereferencePtr(this->_highest_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &getLowestSymbolTable() {
    return rq::dereferencePtr(this->_lowest_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &
  getLowestSymbolTable() const {
    return rq::dereferencePtr(this->_lowest_symbol_table_ptr);
  }
  RQ_ALWAYS_INLINE void setHighestSymbolTable(rq::SymbolTableSymbol &highest) {
    rq::assignSingleValue(this->_highest_symbol_table_ptr, &highest);
    rq::assignSingleValue(this->_lowest_symbol_table_ptr, &highest);
  }
  RQ_ALWAYS_INLINE void setLowestSymbolTable(rq::SymbolTableSymbol &lowest) {
    RQ_ASSERT(lowest.getContainingSymbolTable() == this->getLowestSymbolTable(),
              "lowest not contained by existing lowest");
    this->_lowest_symbol_table_ptr = &lowest;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanAscendLowestSymbolTable() const {
    return this->getLowestSymbolTable() != this->getHighestSymbolTable();
  }
  RQ_ALWAYS_INLINE void ascendLowestSymbolTable() {
    RQ_ASSERT(this->getCanAscendLowestSymbolTable(), "lowest is already highest");
    this->_lowest_symbol_table_ptr = &this->getLowestSymbolTable().getContainingSymbolTable();
  }
  void tabulateModule();
  void tabulateEntry(rq::EntrySymbol &entry);
  void tabulateForest(rq::Expression &first, rq::SymbolTableSymbol &scope);
  [[nodiscard]] llvm::StringRef evaluateName(rq::Expression &expression);
};

} // namespace rq

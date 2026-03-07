#pragma once
#include <rq/utility.hpp>

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <functional>
#include <optional>

namespace rq {

struct Context;
struct Module;
struct Expression;
struct SymbolTable;

enum class FullDeductionResultCode {

};

struct DeductionResult final {
  using Self = rq::DeductionResult;
};

enum class DeductionWithIncompleteResult {

};

struct PartialDeductionResult final {
  using Self = rq::PartialDeductionResult;
};

enum class SubstitutionResultCode {

};

struct SubstitutionResult final {
  using Self = rq::SubstitutionResult;
};

enum class EvaluationResultCode {
  OK_CONCRETE,
  OK_GENERIC,
  OK_DETERMINATE_STATIC_VALUE,
  ERROR_NOT_TYPE,
  ERROR_INVALID_COUNT
};

struct EvaluationResult final {
  using Self = EvaluationResult;

  rq::EvaluationResultCode _code;
  rq::Entity *_entity_ptr;

  EvaluationResult(rq::EvaluationResultCode code, rq::Entity &entity)
      : _code(code), _entity_ptr(&entity) {}
  EvaluationResult(rq::EvaluationResultCode code) : _code(code) {}
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EvaluationResultCode getCode() const {
    return this->_code;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasEntity() const {
    return this->_entity_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getEntity() const {
    return rq::dereferencePtr(this->_entity_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getEntity() {
    return rq::dereferencePtr(this->_entity_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDeterminiteStaticValue() const {
    return this->_code == rq::EvaluationResultCode::OK_DETERMINATE_STATIC_VALUE;
  }
};

struct Tabulator final {
  using Self = rq::Tabulator;

  std::reference_wrapper<rq::Context> _context_ref;
  std::reference_wrapper<rq::Module> _module_ref;
  rq::Expression *_trunk_ptr{nullptr};
  rq::SymbolTable *_highest_symbol_table_ptr{nullptr};
  rq::SymbolTable *_lowest_symbol_table_ptr{nullptr};
  llvm::DenseMap<rq::Expression *, llvm::SmallVector<rq::Symbol *>>
      _declaration_map{};
  bool _is_ok : 1 = true;
  bool _is_started : 1 = false;
  bool _is_building_instructions : 1 = false;

  Tabulator(rq::Context &context, rq::Module &module)
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getModule() {
    return this->_module_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getModule() const {
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSymbolTables() const {
    RQ_ASSERT((this->_highest_symbol_table_ptr == nullptr) ==
                  (this->_lowest_symbol_table_ptr == nullptr),
              "must have both or no tables");
    return this->_highest_symbol_table_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &
  getHighestSymbolTable() {
    return rq::dereferencePtr(this->_highest_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
  getHighestSymbolTable() const {
    return rq::dereferencePtr(this->_highest_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getLowestSymbolTable() {
    return rq::dereferencePtr(this->_lowest_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &
  getLowestSymbolTable() const {
    return rq::dereferencePtr(this->_lowest_symbol_table_ptr);
  }
  RQ_ALWAYS_INLINE void setHighestSymbolTable(rq::SymbolTable &highest) {
    RQ_ASSERT(highest.getIsTopOfFrame(), "highest not top of frame");
    rq::assignSingleValue(this->_highest_symbol_table_ptr, &highest);
    rq::assignSingleValue(this->_lowest_symbol_table_ptr, &highest);
  }
  RQ_ALWAYS_INLINE void setLowestSymbolTable(rq::SymbolTable &lowest) {
    RQ_ASSERT(lowest.getContainingSymbolTable() == this->getLowestSymbolTable(),
              "lowest not contained by existing lowest");
    RQ_ASSERT(!lowest.getIsTopOfFrame(), "lowest is top of frame");
    this->_lowest_symbol_table_ptr = &lowest;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanAscendLowestSymbolTable() const {
    return this->getLowestSymbolTable() != this->getHighestSymbolTable();
  }
  RQ_ALWAYS_INLINE void ascendLowestSymbolTable() {
    RQ_ASSERT(this->getCanAscendLowestSymbolTable(),
              "lowest is already highest");
    this->_lowest_symbol_table_ptr =
        &this->getLowestSymbolTable().getContainingSymbolTable();
  }
  void tabulateModule();
  void tabulateEntry(rq::Entry &entry);
  void tabulateForest(rq::Expression &first, rq::SymbolTable &table);
  [[nodiscard]] rq::DeductionResult
  deduceTypeOfValue(rq::Expression &expression);
  [[nodiscard]] rq::DeductionWithIncompleteResult
  deduceTypeOfValueWithIncomplete(rq::Entity &incomplete_type,
                           rq::Expression &expression);
  [[nodiscard]] rq::SubstitutionResult
  substituteTypeOfValue(rq::Entity &type, rq::Expression &expression);
  [[nodiscard]] rq::EvaluationResult evaluateValue(rq::Entity &type,
                                                   rq::Expression &expression);
  [[nodiscard]] rq::Entity *evaluateType(rq::Expression &expression);
  [[nodiscard]] std::optional<llvm::StringRef>
  evaluateName(rq::Expression &expression);
};

} // namespace rq

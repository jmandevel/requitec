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
struct Instruction;

struct ExecutionFactory final {
  using Self = rq::ExecutionFactory;

  rq::Instruction *_instruction_ptr{nullptr};
  rq::Instruction *_last_exec_ptr{nullptr};

  ExecutionFactory() = default;

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInstruction() const {
    return this->_instruction_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getInstruction() {
    return rq::dereferencePtr(this->_instruction_ptr);
  }
  void addInstruction(rq::Context & context,
                             rq::Instruction &instruction);
};

struct TablePosition final {
  using Self = rq::TablePosition;


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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &getHighestSymbolTable() {
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
  void tabulateGlobalForest(const rq::Expression &first,
                            rq::SymbolTable &hosting_table);
  rq::Instruction *tabulateLocalForest(const rq::Expression &first,
                                       rq::SymbolTable &hosting_table,
                                       rq::Procedure &procedure);
  [[nodiscard]] std::optional<llvm::StringRef>
  evaluateName(const rq::Expression &expression,
               rq::SymbolTable &hosting_table);
  [[nodiscard]] std::optional<llvm::StringRef>
  evaluateUtf8Cstr(const rq::Expression &expression,
                   rq::SymbolTable &hosting_table);
  [[nodiscard]] rq::SymbolTable &
  resolveContainingTable(const rq::ExpressionFlagsFactory &factory,
                         const rq::Expression &unascribed,
                         rq::SymbolTable &hosting_table);
  [[nodiscard]] rq::Entity *evaluateValue(const rq::Expression &expression,
                                          rq::TypeConstant &type,
                                          rq::SymbolTable &hosting_table);
  [[nodiscard]] rq::Symbol *resolveSymbol(const rq::Expression &path,
                                          rq::SymbolTable &hosting_table);
  [[nodiscard]] rq::TypeConstant *evaluateType(const rq::Expression &path,
                                               rq::SymbolTable &hosting_table);
  void evaluateProcedure(rq::Procedure &procedure);
  void tabulateGlobalVariable(rq::SymbolTable &containing_table,
                              rq::SymbolTable &hosting_table,
                              const rq::ExpressionFlagsFactory &factory,
                              const rq::Expression &unascribed,
                              const rq::Expression &name,
                              const rq::Expression &type,
                              const rq::Expression *value_ptr);
};

} // namespace rq

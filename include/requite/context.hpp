// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/alias.hpp>
#include <requite/anonymous_function.hpp>
#include <requite/assert.hpp>
#include <requite/file.hpp>
#include <requite/label.hpp>
#include <requite/log_type.hpp>
#include <requite/module.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/node.hpp>
#include <requite/object.hpp>
#include <requite/opcode.hpp>
#include <requite/ordered_variable.hpp>
#include <requite/procedure.hpp>
#include <requite/scope.hpp>
#include <requite/situation.hpp>
#include <requite/table.hpp>
#include <requite/unordered_variable.hpp>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/ThreadPool.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace requite {

struct File;
struct Options;
struct Expression;
struct Token;
struct SourceLocation;
struct SourceRange;
struct Value;

// llvm context is in inherited type to ensure that the llvm context is
// destroyed last.
struct _ContextLlvmContext {
  std::unique_ptr<llvm::LLVMContext> _llvm_context_uptr = {};
};

struct Context final : public requite::_ContextLlvmContext {
  std::string _executable_path;
  llvm::SourceMgr _source_mgr = {};
  mutable std::mutex _mutex = {};
  std::unique_ptr<llvm::ThreadPoolInterface> _scheduler_ptr = {};
  llvm::StringMap<requite::Opcode> _opcode_table = {};
  std::vector<std::unique_ptr<requite::Module>> _module_uptrs = {};
  requite::Module _source_module = {};
  requite::ExportTable _base_export_table = {};
  std::vector<std::unique_ptr<requite::Scope>> _scope_uptrs = {};
  std::vector<std::unique_ptr<requite::Table>> _table_uptrs = {};
  std::vector<std::unique_ptr<requite::Object>> _object_uptrs = {};
  std::vector<std::unique_ptr<requite::NamedProcedureGroup>>
      _named_procedure_group_uptrs = {};
  std::vector<std::unique_ptr<requite::Procedure>> _procedure_uptrs = {};
  std::vector<std::unique_ptr<requite::Alias>> _alias_uptrs = {};
  std::vector<std::unique_ptr<requite::UnorderedVariable>>
      _unordered_variable_uptrs = {};
  std::vector<std::unique_ptr<requite::OrderedVariable>>
      _ordered_variable_uptrs = {};
  std::vector<std::unique_ptr<requite::AnonymousFunction>>
      _anonymous_function_uptrs = {};
  std::vector<std::unique_ptr<requite::Label>> _label_uptrs = {};
  llvm::StringMap<requite::Module *> _module_map = {};
  std::string _target_triple = {};
  llvm::TargetOptions _llvm_options = {};
  llvm::TargetMachine *_llvm_target_machine_ptr = {};
  const llvm::Target *_llvm_target_ptr = {};
  std::unique_ptr<llvm::DataLayout> _llvm_data_layout_uptr = {};
  std::unique_ptr<llvm::IRBuilder<>> _llvm_builder_uptr = {};
  std::unique_ptr<llvm::Module> _llvm_module_uptr = nullptr;

  // context.cpp
  Context(std::string &&executable_path);
  [[nodiscard]]
  llvm::StringRef getExecutablePath() const;

  // make_symbols.cpp
  [[nodiscard]] requite::Scope &makeScope();
  [[nodiscard]] requite::Table &makeTable();
  [[nodiscard]] requite::Object &makeObject();
  [[nodiscard]] requite::NamedProcedureGroup &makeNamedProcedureGroup();
  [[nodiscard]] requite::Procedure &makeProcedure();
  [[nodiscard]] requite::Alias &makeAlias();
  [[nodiscard]] requite::OrderedVariable &makeOrderedVariable();
  [[nodiscard]] requite::UnorderedVariable &makeUnorderedVariable();
  [[nodiscard]] requite::AnonymousFunction &makeAnonymousFunction();
  [[nodiscard]] requite::Label &makeLabel();
  [[nodiscard]] std::vector<std::unique_ptr<requite::Scope>> &getScopeUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Scope>> &
  getScopeUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Table>> &getTableUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Table>> &
  getTableUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Object>> &getObjectUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Object>> &
  getObjectUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::NamedProcedureGroup>> &
  getNamedProcedureGroupUptrs();
  [[nodiscard]] const std::vector<
      std::unique_ptr<requite::NamedProcedureGroup>> &
  getNamedProcedureGroupUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Procedure>> &
  getProcedureUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Procedure>> &
  getProcedureUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Alias>> &getAliasUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Alias>> &
  getAliasUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::OrderedVariable>> &
  getOrderedVariableUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::OrderedVariable>> &
  getOrderedVariableUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::UnorderedVariable>> &
  getUnorderedVariableUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::UnorderedVariable>> &
  getUnorderedVariableUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::AnonymousFunction>> &
  getAnonymousFunctionUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::AnonymousFunction>> &
  getAnonymousFunctionUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Label>> &getLabelUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Label>> &
  getLabelUptrs() const;

  // file.cpp
  [[nodiscard]]
  bool loadFileBuffer(requite::File &file, llvm::StringRef path);
  [[nodiscard]]
  requite::SourceLocation getSourceLocation(llvm::SMLoc loc) const;
  [[nodiscard]]
  requite::SourceLocation
  getSourceStartLocation(const requite::Expression &expression) const;
  [[nodiscard]]
  requite::SourceLocation
  getSourceEndLocation(const requite::Expression &expression) const;
  [[nodiscard]]
  requite::SourceRange
  getSourceRange(const requite::Expression &expression) const;
  [[nodiscard]]
  bool getHasModule(llvm::StringRef name) const;
  [[nodiscard]]
  requite::Module &getModule(llvm::StringRef name);
  [[nodiscard]]
  const requite::Module &getModule(llvm::StringRef name) const;
  [[nodiscard]]
  std::vector<std::unique_ptr<requite::Module>> &getModuleUptrs();
  [[nodiscard]]
  const std::vector<std::unique_ptr<requite::Module>> &getModuleUptrs() const;

  // validate_source.cpp
  [[nodiscard]]
  bool validateSourceFileText(requite::File &file);

  // situate_ast.cpp
  [[nodiscard]]
  bool situateAst(requite::Module &module);

  // tokenize_tokens.cpp
  [[nodiscard]]
  bool tokenizeTokens(requite::Module &module,
                      std::vector<requite::Token> &tokens);

  // token_csv.cpp
  void writeTokenCsv(requite::Module &module,
                     std::vector<requite::Token> &tokens);

  // parse_ast.cpp
  [[nodiscard]]
  bool parseAst(requite::Module &module, std::vector<requite::Token> &token);

  // write_ast.cpp
  void writeAst(const requite::Module &module,  llvm::StringRef output_path);

  // write_llvm_ir.cpp
  void writeLlvmIr(llvm::StringRef output_path);

  // source_name.cpp
  [[nodiscard]] bool determineModuleName(requite::Module &module);

  // tabulate_user_symbols.cpp
  // TODO

  // prototype_user_symbols.cpp
  // TODO

  // build_user_symbols.cpp
  // TODO

  // resolve_symbols.cpp
  [[nodiscard]] bool resolveSymbol(requite::Symbol &out_symbol,
                                   requite::Scope &scope,
                                   requite::Expression &symbol_expression);
  [[nodiscard]] bool resolveTypeOfValue(requite::Symbol &out_symbol,
                                        requite::Scope &scope,
                                        requite::Expression &symbol_expression,
                                        requite::Expression &value_expression);
  [[nodiscard]] bool
  inferenceTypeOfValue(requite::Symbol &out_symbol, requite::Scope &scope,
                       requite::Expression &value_expression);
  [[nodiscard]] bool resolveTypeAttributes(requite::AttributeFlags flags,
                                           requite::Expression &first);

  // choose_overload.cpp
  [[nodiscard]] bool chooseOverload(requite::Scope &scope,
                                    requite::Expression &call_expression);

  // evaluate_values.cpp
  [[nodiscard]] bool
  evaluateName(llvm::StringRef &name, requite::Scope &scope,
                           requite::Expression &value_expression);
  [[nodiscard]] bool
  evaluateConstantUnsigned(unsigned &out_unsigned, requite::Scope &scope,
                           requite::Expression &value_expression);
  [[nodiscard]] requite::Value
  evaluateValue(requite::Scope &scope, requite::Expression &value_expression,
                const requite::Symbol &type);

  // compile_object_files.cpp
  [[nodiscard]] bool compileObjectFiles();
  [[nodiscard]] bool compileObjectFile(requite::Module &module);

  // get_module.cpp
  [[nodiscard]]
  requite::Module &getSourceModule();
  [[nodiscard]]
  const requite::Module &getSourceModule() const;
  [[nodiscard]]
  requite::Module *getModulePtr(llvm::StringRef import_path);
  [[nodiscard]]
  const requite::Module *getModulePtr(llvm::StringRef import_path) const;

  // llvm_target.cpp
  void initializeLlvm();
  void initializeLlvmBuilder();
  void initializeLlvmContext();
  [[nodiscard]]
  bool getIsLlvmContextInitialized() const;
  void terminateLlvmContext();
  [[nodiscard]]
  llvm::LLVMContext &getLlvmContext();
  [[nodiscard]]
  const llvm::LLVMContext &getLlvmContext() const;
  [[nodiscard]]
  bool initializeLlvmTarget();
  [[nodiscard]]
  const llvm::Target &getLlvmTarget() const;
  [[nodiscard]]
  llvm::TargetOptions &getLlvmTargetOptions();
  [[nodiscard]]
  const llvm::TargetOptions &getLlvmTargetOptions() const;
  [[nodiscard]]
  llvm::TargetMachine &getLlvmTargetMachine();
  [[nodiscard]]
  const llvm::TargetMachine &getLlvmTargetMachine() const;
  [[nodiscard]]
  const llvm::DataLayout &getLlvmDataLayout() const;
  [[nodiscard]]
  llvm::StringRef getLlvmTargetTriple() const;
  [[nodiscard]]
  llvm::IRBuilder<> &getLlvmBuilder();
  [[nodiscard]]
  const llvm::IRBuilder<> &getLlvmBuilder() const;
  [[nodiscard]]
  bool getIsLittleEndian() const;
  [[nodiscard]]
  bool getIsBigEndian() const;
  [[nodiscard]]
  bool getIsLegalIntegerDepth(unsigned depth) const;
  [[nodiscard]]
  bool getFitsInLegalIntegerDepth(unsigned depth) const;
  [[nodiscard]]
  unsigned getAddressDepth() const;
  [[nodiscard]]
  unsigned getAddressSize() const;
  [[nodiscard]]
  unsigned getLargestLegalIntegerDepth() const;
  [[nodiscard]]
  unsigned getMaxIndexDepth() const;
  [[nodiscard]]
  unsigned getIndexDepth() const;

  // llvm_module.cpp
  void initializeLlvmModule();
  [[nodiscard]] bool getIsLlvmModuleInitialized() const;
  void terminateLlvmModule();
  [[nodiscard]] llvm::Module &getLlvmModule();
  [[nodiscard]] const llvm::Module &getLlvmModule() const;
  [[nodiscard]] std::string getLlvmIrSourceText() const;

  // run.cpp
  [[nodiscard]]
  bool run();

  // opcode.cpp
  [[nodiscard]]
  requite::Opcode getOpcode(llvm::StringRef text) const;
  void createOpcodeTable();

  // tasks.cpp
  void startScheduler();
  void waitForTasks();

  // detail/tasks.inl
  template <typename TaskPram> void scheduleTask(TaskPram &&task);

  // log.cpp
  void logMessage(llvm::Twine message);
  void logInputFileMessage(requite::LogType type,
                        llvm::Twine message);
  void logSourceMessage(llvm::Twine filename, requite::LogType type,
                        llvm::Twine message);
  void logSourceMessage(const requite::Token &token, requite::LogType type,
                        llvm::Twine message,
                        llvm::ArrayRef<llvm::SMRange> ranges = {},
                        llvm::ArrayRef<llvm::SMFixIt> fixits = {});
  void logSourceMessage(const requite::Expression &expression,
                        requite::LogType type, llvm::Twine message,
                        llvm::ArrayRef<llvm::SMRange> ranges = {},
                        llvm::ArrayRef<llvm::SMFixIt> fixits = {});
  void logErrorNonInstantEvaluatableName(requite::Expression &expression);
  void logErrorNonExternallyAccessableTable(requite::Expression &expression);
  void logErrorAlreadySymbolOfName(requite::Expression &expression);
  void logErrorDuplicateAttribute(requite::Expression &expression,
                                  requite::AttributeType type);
  void logErrorMustNotHaveAttributeFlags(requite::Expression &expression);
  void logNotSupportedYet(requite::Expression &expression);

  // detail/log.hpp
  template <requite::Situation SITUATION_PARAM>
  void logNotAtLeastBranchCount(requite::Expression &expression,
                                unsigned count);
  template <requite::Situation SITUATION_PARAM>
  void logNotExactBranchCount(requite::Expression &expression, unsigned count);
  template <requite::Situation SITUATION_PARAM>
  void logTooNotLessOrEqualToBranchCount(requite::Expression &expression,
                                         unsigned count);
  template <requite::Situation SITUATION_PARAM>
  void logInvalidBranchSituation(requite::Expression &branch,
                                 requite::Opcode outer_opcode,
                                 requite::Opcode branch_opcode,
                                 unsigned branch_i, llvm::Twine log_context);
  inline void logInvalidOperation(requite::Expression &expression);
};

} // namespace requite

#include <requite/detail/log.hpp>
#include <requite/detail/tasks.hpp>
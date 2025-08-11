// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/anonymous_function.hpp>
#include <requite/assert.hpp>
#include <requite/file.hpp>
#include <requite/global.hpp>
#include <requite/import.hpp>
#include <requite/local.hpp>
#include <requite/log_type.hpp>
#include <requite/module.hpp>
#include <requite/numeric.hpp>
#include <requite/object.hpp>
#include <requite/opcode.hpp>
#include <requite/procedure.hpp>
#include <requite/property.hpp>
#include <requite/scope.hpp>
#include <requite/situation.hpp>
#include <requite/table.hpp>
#include <requite/use.hpp>
#include <requite/use_table.hpp>
#include <requite/saved_string.hpp>

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
#include <llvm/Support/Allocator.h>

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
  using Self = requite::Context;

  llvm::BumpPtrAllocator _arena;
  llvm::StringSaver _string_saver = {_arena};
  std::string _executable_path;
  llvm::SourceMgr _source_mgr = {};
  mutable std::mutex _mutex = {};
  std::unique_ptr<llvm::ThreadPoolInterface> _scheduler_ptr = {};
  llvm::StringMap<requite::Opcode> _opcode_table = {};
  requite::Module _source_module = {};
  llvm::StringMap<requite::Module *> _module_map = {};
  std::string _target_triple = {};
  llvm::TargetOptions _llvm_options = {};
  llvm::TargetMachine *_llvm_target_machine_ptr = {};
  const llvm::Target *_llvm_target_ptr = {};
  std::unique_ptr<llvm::DataLayout> _llvm_data_layout_uptr = {};
  std::unique_ptr<llvm::IRBuilder<>> _llvm_builder_uptr = {};
  std::unique_ptr<llvm::Module> _llvm_module_uptr = nullptr;
  requite::Scope _outer_scope = {};

  // context.cpp
  Context() = delete;
  Context(std::string &&executable_path);
  Context(const Self &) = delete;
  Context(Self &&) = delete;
  ~Context() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] llvm::StringRef getExecutablePath() const;
  [[nodiscard]] requite::Scope &getOuterScope();
  [[nodiscard]] const requite::Scope &getOuterScope() const;

  // detail/allocate.hpp
  template<typename TypeParam, typename... ArgNParam>
  [[nodiscard]] TypeParam& allocate(ArgNParam... arg_n);
  [[nodiscard]] inline requite::SavedString saveString(llvm::StringRef string);

  // file.cpp
  [[nodiscard]] bool loadFileBuffer(requite::File &file, llvm::StringRef path);

  // source_text.cpp
  [[nodiscard]] requite::SourceLocation
  getSourceLocation(llvm::SMLoc loc) const;
  [[nodiscard]] requite::SourceLocation
  getSourceStartLocation(const requite::Expression &expression) const;
  [[nodiscard]] requite::SourceLocation
  getSourceEndLocation(const requite::Expression &expression) const;
  [[nodiscard]] requite::SourceRange
  getSourceRange(const requite::Expression &expression) const;

  // module_map.cpp
  [[nodiscard]] bool importModule(requite::Import &import);
  [[nodiscard]]
  bool getHasModule(llvm::StringRef path) const;
  [[nodiscard]]
  requite::Module &getModule(llvm::StringRef path);
  [[nodiscard]]
  const requite::Module &getModule(llvm::StringRef path) const;
  [[nodiscard]]
  requite::Module &getSourceModule();
  [[nodiscard]]
  const requite::Module &getSourceModule() const;
  [[nodiscard]]
  requite::Module *getModulePtr(llvm::StringRef import_path);
  [[nodiscard]]
  const requite::Module *getModulePtr(llvm::StringRef import_path) const;

  // prepare_tables.cpp
  void propogateExportedImports();
  [[nodiscard]] bool propogateExportImportPass();
  void diffuseUses();
  [[nodiscard]] bool diffuseUsePass();

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

  // parse_ast.cpp
  [[nodiscard]]
  bool parseAst(requite::Module &module, std::vector<requite::Token> &token);

  // tabulate.cpp
  [[nodiscard]] bool tabulateModule(requite::Module &module);

  // implement.cpp
  [[nodiscard]] bool implementAll();

  // build.cpp
  [[nodiscard]] bool buildIr();

  // resolve.cpp
  [[nodiscard]] bool resolveSymbol(requite::Symbol &out_symbol,
                                   requite::Scope &scope,
                                   requite::Expression &symbol_expression);
  [[nodiscard]] bool resolveTypeAttributes(requite::AttributeFlags &flags,
                                           requite::Expression &first);
  void finalizeIfLiteralType(requite::Symbol &symbol);

  // evaluate.cpp
  [[nodiscard]] bool evaluateInstantName(llvm::StringRef &out_name,
                                         requite::Expression &value_expression);
  [[nodiscard]] bool
  evaluateConstantUnsigned(unsigned &out_unsigned, requite::Scope &scope,
                           requite::Expression &value_expression);

  // choose_overload.cpp
  [[nodiscard]] bool chooseOverload(requite::Scope &scope,
                                    requite::Expression &call_expression);

  // write_tokens.cpp
  [[nodiscard]] bool writeTokens(requite::Module &module,
                                 std::vector<requite::Token> &tokens,
                                 llvm::StringRef output_path);

  // write_ast.cpp
  [[nodiscard]] bool writeAst(const requite::Module &module,
                              llvm::StringRef output_path);

  // write_user_symbols.cpp
  [[nodiscard]] bool writeUserSymbols(llvm::StringRef output_path);

  // write_llvm_ir.cpp
  [[nodiscard]] bool writeLlvmIr(llvm::StringRef output_path);

  // write_assembly.cpp
  [[nodiscard]] bool writeAssembly(llvm::StringRef output_path);

  // write_object.cpp
  [[nodiscard]] bool writeObject(llvm::StringRef output_path);

  // llvm_target.cpp
  [[nodiscard]] bool initializeLlvm();
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
  [[nodiscard]] bool run();

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
  void logInputFileMessage(requite::LogType type, llvm::Twine message);
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
  void logNotSupportedYet(requite::Expression &expression);
  void logErrorNumericParse(requite::Expression &expression,
                            requite::NumericResult result);
  void
  logErrorInvalidExpectedTypeForOperation(requite::Expression &expression,
                                          const requite::Symbol &expected_type);
  void logErrorMustNotHaveAttributes(requite::Expression &expression);
  void logErrorMissingTrailingSemicolon(requite::Expression &expression);
  void logErrorExpectedCommaSeperator(const requite::Token &token);
  void logErrorExpectedSemicolonSeperator(const requite::Token &token);
  void logErrorExpectedSeperatorOrRightBracket(const requite::Token &token);
  void logErrorExpectedSeperator(const requite::Token &token);
  void logErrorUnterminatedExpression(requite::Expression &expression);
  void logErrorUnterminatedAttribute(const requite::Token &token);
  void
  logErrorMustNotHaveParameterMark(requite::Expression &containing_expression,
                                   const requite::Token &token);
  void logErrorMustHaveParameterMarks(requite::Expression &expression);

  // detail/log.hpp
  template <requite::Situation SITUATION_PARAM>
  void logErrorNotAtLeastBranchCount(requite::Expression &expression,
                                     unsigned count);
  template <requite::Situation SITUATION_PARAM>
  void logErrorNotExactBranchCount(requite::Expression &expression,
                                   unsigned count);
  template <requite::Situation SITUATION_PARAM>
  void logErrorTooNotLessOrEqualToBranchCount(requite::Expression &expression,
                                              unsigned count);
  template <requite::Situation SITUATION_PARAM,
            requite::Situation BRANCH_SITUATION_PARAM>
  void logErrorInvalidBranchSituation(requite::Expression &branch,
                                      requite::Opcode outer_opcode,
                                      requite::Opcode branch_opcode,
                                      unsigned branch_i,
                                      llvm::Twine log_context);
  inline void logErrorInvalidOperation(requite::Expression &expression);
};

} // namespace requite

#include <requite/detail/allocate.hpp>
#include <requite/detail/log.hpp>
#include <requite/detail/tasks.hpp>
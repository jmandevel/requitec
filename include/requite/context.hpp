// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>
#include <requite/log_type.hpp>
#include <requite/module.hpp>
#include <requite/opcode.hpp>
#include <requite/situation.hpp>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
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

// llvm context is in inherited type to ensure that the llvm context is
// destroyed last.
struct _ContextLlvmContext {
  std::unique_ptr<llvm::LLVMContext> _llvm_context_uptr = {};
};

struct Context final : public requite::_ContextLlvmContext {
  std::string _executable_path;
  llvm::SmallString<256> _real_source_directory = {};
  llvm::SmallString<256> _real_build_directory = {};
  llvm::SmallString<256> _real_intermediate_directory = {};
  llvm::SourceMgr _source_mgr = {};
  mutable std::mutex _mutex = {};
  std::unique_ptr<llvm::ThreadPoolInterface> _scheduler_ptr = {};
  llvm::StringMap<requite::Opcode> _opcode_table = {};
  std::vector<std::unique_ptr<requite::Module>> _module_uptrs = {};
  llvm::StringMap<requite::Module *> _module_map = {};
  std::string _target_triple = {};
  llvm::TargetOptions _llvm_options = {};
  llvm::TargetMachine *_llvm_target_machine_ptr = {};
  const llvm::Target *_llvm_target_ptr = {};
  std::unique_ptr<llvm::DataLayout> _llvm_data_layout_uptr = {};
  std::unique_ptr<llvm::IRBuilder<>> _llvm_builder_uptr = {};

  // context.cpp
  Context(std::string &&executable_path);

  // filesystem.cpp
  [[nodiscard]]
  bool determineDirectoryPaths();
  [[nodiscard]]
  llvm::StringRef getExecutablePath() const;
  [[nodiscard]]
  llvm::StringRef getSourcePath() const;
  [[nodiscard]]
  llvm::StringRef getBuildPath() const;
  [[nodiscard]]
  llvm::StringRef getIntermediatePath() const;

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
  void writeAst(const requite::Module &module, llvm::StringRef sub_extension);

  // write_llvm_ir.cpp
  void writeLlvmIr(const requite::Module &module);

  // source_name.cpp
  [[nodiscard]] bool setupModuleNames();
  [[nodiscard]] bool determineModuleNames();
  [[nodiscard]] bool mapModules();

  // make_user_symbols.cpp
  [[nodiscard]] bool makeUserSymbols();

  // name_user_symbols.cpp
  [[nodiscard]] bool nameUserSymbols();

  // resolve_user_symbols.cpp
  [[nodiscard]] bool resolveUserSymbols();

  // build_user_symbols.cpp
  [[nodiscard]] bool buildUserSymbols();

  // build_ir.cpp
  [[nodiscard]]
  bool buildIr(requite::Module &module);

  // compile_objects.cpp
  [[nodiscard]]
  bool compileObject(requite::Module &module);

  // get_module.cpp
  [[nodiscard]]
  requite::Module *getModulePtr(llvm::StringRef name);
  [[nodiscard]]
  const requite::Module *getModulePtr(llvm::StringRef name) const;

  // llvm_target.cpp
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
  unsigned getPointerDepth() const;
  [[nodiscard]]
  unsigned getLargestLegalIntegerDepth() const;
  [[nodiscard]]
  unsigned getMaxIndexDepth() const;
  [[nodiscard]]
  unsigned getIndexDepth() const;

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

  // detail/log.hpp
  template <requite::Situation SITUATION_PARAM>
  void logNotAtLeastBranchCount(requite::Expression &expression,
                                unsigned count);
  template <requite::Situation SITUATION_PARAM>
  void logNotExactBranchCount(requite::Expression &expression, unsigned count);
  template <requite::Situation SITUATION_PARAM>
  void logInvalidBranchSituation(requite::Expression &branch,
                                 requite::Opcode outer_opcode,
                                 requite::Opcode branch_opcode,
                                 unsigned branch_i, llvm::Twine log_context);
};

} // namespace requite

#include <requite/detail/log.hpp>
#include <requite/detail/tasks.hpp>
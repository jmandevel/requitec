// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/import.hpp>
#include <requite/literal_text.hpp>
#include <requite/module.hpp>
#include <requite/options.hpp>
#include <requite/strings.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>

namespace requite {

bool Context::importModule(requite::Import &import) {
  requite::Expression &expression = import.getExpression();
  requite::Expression &name_expression = expression.getBranch();
  llvm::SmallString<128> relative_path;
  requite::TextResult result =
      requite::getTextValue(name_expression.getDataText().getString(), relative_path);
  if (result != requite::TextResult::OK) {
    this->logSourceMessage(name_expression, requite::LogType::ERROR,
                           llvm::Twine("failed to parse import name because ") +
                               requite::getDescription(result));
    return false;
  }
  if (relative_path.ends_with(requite::SOURCE_FILE_EXTENSION)) {
    this->logSourceMessage(
        expression, requite::LogType::ERROR,
        "file extension must be ommited from import file path string");
    return false;
  }
  relative_path += requite::SOURCE_FILE_EXTENSION;
  llvm::SmallString<128> path = relative_path;
  std::error_code ec = llvm::sys::fs::make_absolute(path);
  if (ec || !llvm::sys::fs::is_regular_file(path)) {
    llvm::SmallVector<llvm::SmallString<128>> candidates;
    for (const std::string &import_directory :
         requite::getImportDirectories()) {
      llvm::SmallString<128> import_path(import_directory);
      llvm::sys::path::append(import_path, relative_path);
      if (llvm::sys::fs::is_regular_file(import_path)) {
        candidates.emplace_back(std::move(import_path));
      }
    }
    if (candidates.empty()) {
      this->logSourceMessage(expression, requite::LogType::ERROR,
                             "import file not found");
      return false;
    } else if (candidates.size() > 1) {
      this->logSourceMessage(expression, requite::LogType::ERROR,
                             "ambiguous import");
      for (const llvm::SmallString<128> &candidate : candidates) {
        this->logSourceMessage(expression, requite::LogType::NOTE,
                               llvm::Twine("could be file at path: \"") +
                                   candidate + "\"");
      }
      return false;
    }
    path = std::move(candidates.front());
  }
  if (this->_module_map.contains(path)) {
    requite::Module &module = this->getModule(path);
    import.setModule(module);
    return true;
  }
  requite::Module &module = this->allocate<requite::Module>();
  import.setModule(module);
  if (!this->loadFileBuffer(module.getFile(), path)) {
    return false;
  }
  this->_module_map.insert(
      std::pair<llvm::StringRef, requite::Module *>(path, &module));
  if (!this->validateSourceFileText(module.getFile())) {
    return false;
  }
  std::vector<requite::Token> tokens = {};
  if (!this->tokenizeTokens(module, tokens)) {
    return false;
  }
  if (!this->parseAst(module, tokens)) {
    return false;
  }
  if (!this->situateAst(module)) {
    return false;
  }
  if (!this->tabulateModule(module)) {
    return false;
  }
  return true;
}

bool Context::getHasModule(llvm::StringRef name) const {
  return this->_module_map.contains(name);
}

requite::Module &Context::getModule(llvm::StringRef name) {
  REQUITE_ASSERT(this->_module_map.contains(name));
  requite::Module &module = requite::getRef(this->_module_map[name]);
  return module;
}

const requite::Module &Context::getModule(llvm::StringRef name) const {
  REQUITE_ASSERT(this->_module_map.contains(name));
  requite::Module &module = requite::getRef(this->_module_map.at(name));
  return module;
}

requite::Module &Context::getSourceModule() { return this->_source_module; }

const requite::Module &Context::getSourceModule() const {
  return this->_source_module;
}

requite::Module *Context::getModulePtr(llvm::StringRef name) {
  if (!this->_module_map.contains(name)) {
    return nullptr;
  }
  return this->_module_map.at(name);
}

const requite::Module *Context::getModulePtr(llvm::StringRef name) const {
  if (!this->_module_map.contains(name)) {
    return nullptr;
  }
  return this->_module_map.at(name);
}

void Module::addImport(requite::Import &import) {
  requite::Module &module = import.getModule();
  this->getImportModulePtrSet().insert(&module);
  if (import.getAttributeFlags().getHasAttribute(
          requite::AttributeType::EXPORT)) {
    this->getExportModulePtrs().push_back(&module);
  }
}

} // namespace requite
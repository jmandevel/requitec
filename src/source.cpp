// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/const_expression_iterator.hpp>
#include <requite/context.hpp>
#include <requite/expression_iterator.hpp>
#include <requite/options.hpp>
#include <requite/source.hpp>
#include <requite/source_location.hpp>
#include <requite/source_range.hpp>

#include <llvm/ADT/Twine.h>
#include <llvm/Support/FileSystem.h>

namespace requite {

Source::~Source() {
  if (this->_root_ptr != nullptr) {
    requite::Expression::deleteExpression(requite::getRef(this->_root_ptr));
  }
}

llvm::StringRef Source::getPath() const { return this->_path; }

llvm::StringRef Source::getIdentifier() const {
  return this->_buffer_ref.getBufferIdentifier();
}

llvm::StringRef Source::getText() const {
  return this->_buffer_ref.getBuffer();
}

const char *Source::getTextPtr() const {
  return this->_buffer_ref.getBufferStart();
}

std::uint_fast32_t Source::getBufferI() const { return this->_buffer_i; }

bool Source::getHasRoot() const { return this->_root_ptr != nullptr; }

const requite::Expression &Source::getRoot() const {
  return requite::getRef(this->_root_ptr);
}

requite::Expression &Source::getRoot() {
  return requite::getRef(this->_root_ptr);
}

const requite::Expression &Source::getLastRoot() const {
  return requite::getRef(this->_root_ptr).getLastNext();
}

requite::Expression &Source::getLastRoot() {
  return requite::getRef(this->_root_ptr).getLastNext();
}

void Source::setRoot(requite::Expression &expression) {
  requite::setSingleRef(this->_root_ptr, expression);
}

requite::Expression &Source::popRoot() {
  requite::Expression &root = requite::getRef(this->_root_ptr);
  this->_root_ptr = nullptr;
  return root;
}

requite::Expression &Source::replaceRoot(requite::Expression &expression) {
  requite::Expression &old_root = requite::getRef(this->_root_ptr);
  this->_root_ptr = &expression;
  return old_root;
}

bool Context::loadSourceBuffer(requite::Source &source, llvm::StringRef path) {
  llvm::SmallString<256> path_buffer = path;
  llvm::sys::fs::make_absolute(this->getSourcePath(), path_buffer);
  source._relative_path =
      path; // This is from a command line option so its lifetime is static.
  source._path = path_buffer.str();
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer_eo =
      llvm::MemoryBuffer::getFile(source.getPath(), true, true, false,
                                  std::nullopt);
  if (!buffer_eo) {
    this->logMessage(
        llvm::Twine(
            "error: failed to create read buffer for source file\n\tfile: ") +
        llvm::Twine(source.getPath()) + llvm::Twine("\n\treason: ") +
        llvm::Twine(buffer_eo.getError().message()));
    return false;
  }
  std::unique_ptr<llvm::MemoryBuffer> &buffer = buffer_eo.get();
  source._buffer_ref = buffer->getMemBufferRef();
  source._buffer_i =
      this->_source_mgr.AddNewSourceBuffer(std::move(buffer), llvm::SMLoc());
  return true;
}

bool Context::mapSources() {
  bool is_ok = true;
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    if (this->_module_map.contains(module.getTable().getName())) {
      this->logSourceMessage(
          module.getSource().getPath(), requite::LogType::ERROR,
          "module file has name that is already used by other module file");
      is_ok = false;
      continue;
    }
    this->_module_map.insert({module.getTable().getName(), &module});
  }
  return is_ok;
}

requite::SourceLocation Context::getSourceLocation(llvm::SMLoc loc) const {
  unsigned buffer_i = this->_source_mgr.FindBufferContainingLoc(loc);
  auto line_and_column = this->_source_mgr.getLineAndColumn(loc, buffer_i);
  requite::SourceLocation source_location = {};
  source_location.file =
      this->_source_mgr.getMemoryBuffer(buffer_i)->getBufferIdentifier();
  source_location.line = line_and_column.first;
  source_location.column = line_and_column.second;
  return source_location;
}

requite::SourceLocation
Context::getSourceStartLocation(const requite::Expression &expression) const {
  return this->getSourceLocation(expression.getSourceStartLlvmLocation());
}

requite::SourceLocation
Context::getSourceEndLocation(const requite::Expression &expression) const {
  return this->getSourceLocation(expression.getSourceEndLlvmLocation());
}

requite::SourceRange
Context::getSourceRange(const requite::Expression &expression) const {
  requite::SourceRange source_range = {};
  source_range.start =
      this->getSourceLocation(expression.getSourceStartLlvmLocation());
  source_range.end = source_range.start;
  llvm::StringRef source_text = expression.getSourceText();
  bool had_carriage_return = false;
  for (const char &cur_c : source_text) {
    if (had_carriage_return) {
      had_carriage_return = false;
      switch (cur_c) {
      case '\n':
        continue;
      default:
        break;
      }
    }
    switch (cur_c) {
    case '\r':
      had_carriage_return = true;
      [[fallthrough]];
    case '\v':
      [[fallthrough]];
    case '\n':
      source_range.end.column = 0;
      source_range.end.line++;
      break;
    default:
      source_range.end.column++;
    }
  }
  return source_range;
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

std::vector<std::unique_ptr<requite::Module>> &Context::getModuleUptrs() {
  return this->_module_uptrs;
}

const std::vector<std::unique_ptr<requite::Module>> &
Context::getModuleUptrs() const {
  return this->_module_uptrs;
}

std::ranges::subrange<requite::ExpressionIterator, requite::ExpressionIterator,
                      std::ranges::subrange_kind::unsized>
Source::getRootSubrange() {
  return std::ranges::subrange(requite::ExpressionIterator(*(this->_root_ptr)),
                               requite::ExpressionIterator());
}

std::ranges::subrange<requite::ConstExpressionIterator,
                      requite::ConstExpressionIterator,
                      std::ranges::subrange_kind::unsized>
Source::getRootSubrange() const {
  return std::ranges::subrange(
      requite::ConstExpressionIterator(*(this->_root_ptr)),
      requite::ConstExpressionIterator());
}

} // namespace requite
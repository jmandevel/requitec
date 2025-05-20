// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/options.hpp>
#include <requite/source.hpp>

#include <llvm/ADT/Twine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>

#include <string>
#include <utility>

namespace requite {

bool Context::determineDirectoryPaths() {
  std::error_code ec;
  bool is_ok = true;
  llvm::StringRef source_directory =
      requite::options::SOURCE_DIRECTORY.getValue();
  ec = llvm::sys::fs::real_path(source_directory, this->_real_source_directory,
                                true);
  if (ec) {
    is_ok = false;
    this->logMessage(
        llvm::Twine("error: failed to find source directory.\n\tpath: ") +
        llvm::Twine(source_directory) + llvm::Twine("\n\tmessage: ") +
        llvm::Twine(ec.message()));
  } else {
    this->logMessage(llvm::Twine("source directory: ") +
                     llvm::Twine(this->_real_source_directory));
  }
  if (!llvm::sys::fs::is_directory(this->_real_source_directory)) {
    is_ok = false;
    this->logMessage(
        llvm::Twine("error: source path is not directory.\n\tpath: ") +
        llvm::Twine(this->_real_source_directory));
  }
  llvm::StringRef build_directory =
      requite::options::BUILD_DIRECTORY.getValue();
  ec = llvm::sys::fs::create_directories(build_directory, true);
  if (ec) {
    is_ok = false;
    this->logMessage(
        llvm::Twine("error: failed to create build directory.\n\tpath: ") +
        llvm::Twine(build_directory) + llvm::Twine("\n\tmessage: ") +
        llvm::Twine(ec.message()));
  } else if (is_ok) {
    ec = llvm::sys::fs::real_path(build_directory, this->_real_build_directory,
                                  true);
    if (ec) {
      is_ok = false;
      this->logMessage(
          llvm::Twine("error: failed to find build directory.\n\tpath: ") +
          llvm::Twine(build_directory) + llvm::Twine("\n\tmessage: ") +
          llvm::Twine(ec.message()));
    } else {
      this->logMessage(llvm::Twine("Build Directory: ") +
                       llvm::Twine(this->_real_build_directory));
    }
  }
  if (!llvm::sys::fs::is_directory(this->_real_build_directory)) {
    is_ok = false;
    this->logMessage(
        llvm::Twine("error: build path is not directory.\n\tpath: ") +
        llvm::Twine(this->_real_build_directory));
  }
  llvm::StringRef intermediate_directory =
      requite::options::INTERMEDIATE_DIRECTORY.getValue();
  ec = llvm::sys::fs::create_directories(intermediate_directory, true);
  if (ec) {
    is_ok = false;
    this->logMessage(
        llvm::Twine(
            "error: failed to create intermediate directory.\n\tpath: ") +
        llvm::Twine(intermediate_directory) + llvm::Twine("\n\tmessage: ") +
        llvm::Twine(ec.message()));
  } else if (is_ok) {
    ec = llvm::sys::fs::real_path(intermediate_directory,
                                  this->_real_intermediate_directory, true);
    if (ec) {
      is_ok = false;
      this->logMessage(
          llvm::Twine(
              "error: failed to find intermediate directory.\n\tpath: ") +
          llvm::Twine(intermediate_directory) + llvm::Twine("\n\tmessage: ") +
          llvm::Twine(ec.message()));
    } else {
      this->logMessage(llvm::Twine("intermediate directory: ") +
                       llvm::Twine(this->_real_intermediate_directory));
    }
  }
  if (!llvm::sys::fs::is_directory(this->_real_intermediate_directory)) {
    is_ok = false;
    this->logMessage(
        llvm::Twine("error: intermediate path is not directory.\n\tpath: ") +
        llvm::Twine(this->_real_intermediate_directory));
  }
  return is_ok;
}

llvm::StringRef Context::getSourcePath() const {
  return this->_real_source_directory;
}

llvm::StringRef Context::getBuildPath() const {
  return this->_real_build_directory;
}

llvm::StringRef Context::getIntermediatePath() const {
  return this->_real_intermediate_directory;
}

bool Source::makeIntermediateFilePath(llvm::SmallString<256> &out_path,
                                      requite::Context &context,
                                      llvm::Twine extension) const {
  std::error_code ec;
  llvm::Twine relative_path_twine =
      llvm::Twine(this->_relative_path) + extension;
  llvm::SmallString<256> buffer;
  out_path = relative_path_twine.toStringRef(buffer);
  llvm::sys::fs::make_absolute(context.getIntermediatePath(), out_path);
  llvm::SmallString<256> directory_path = out_path;
  llvm::sys::path::remove_filename(directory_path);
  ec = llvm::sys::fs::create_directories(directory_path, true);
  if (ec) {
    context.logMessage(llvm::Twine("error: failed to create directory for "
                                   "intermediate file.\n\tpath: ") +
                       llvm::Twine(out_path) + llvm::Twine("\n\tmessage: ") +
                       llvm::Twine(ec.message()));
    return false;
  }
  return true;
}

} // namespace requite
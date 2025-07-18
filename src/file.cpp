// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/file.hpp>

namespace requite {

llvm::StringRef File::getPath() const { return this->_path; }

llvm::StringRef File::getIdentifier() const {
  return this->_buffer_ref.getBufferIdentifier();
}

llvm::StringRef File::getText() const { return this->_buffer_ref.getBuffer(); }

const char *File::getTextPtr() const {
  return this->_buffer_ref.getBufferStart();
}

std::uint_fast32_t File::getBufferI() const { return this->_buffer_i; }

bool Context::loadFileBuffer(requite::File &file, llvm::StringRef path) {
  REQUITE_ASSERT(file._path.empty());
  file._path = path;
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer_eo =
      llvm::MemoryBuffer::getFile(file.getPath(), true, true, false,
                                  std::nullopt);
  if (!buffer_eo) {
    this->logMessage(
        llvm::Twine("error: failed to create read buffer for file\n\tfile: ") +
        llvm::Twine(file.getPath()) + llvm::Twine("\n\treason: ") +
        llvm::Twine(buffer_eo.getError().message()));
    return false;
  }
  std::unique_ptr<llvm::MemoryBuffer> &buffer = buffer_eo.get();
  file._buffer_ref = buffer->getMemBufferRef();
  file._buffer_i =
      this->_source_mgr.AddNewSourceBuffer(std::move(buffer), llvm::SMLoc());
  return true;
}

} // namespace requite
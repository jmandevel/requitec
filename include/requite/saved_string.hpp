#pragma once

#include <llvm/ADT/StringRef.h>

namespace requite {

struct Context;

struct SavedString final {
  using Self = requite::SavedString;
  friend requite::Context;

  llvm::StringRef _string = {};

  // /detail/saved_string.hpp
  SavedString() = default;

private:
  inline SavedString(llvm::StringRef string);

public:
  SavedString(const Self &) = default;
  SavedString(Self &&) = default;
  ~SavedString() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] inline llvm::StringRef &getString();
  [[nodiscard]] inline const llvm::StringRef &getString() const;
};

} // namespace requite

#include <requite/detail/saved_string.hpp>
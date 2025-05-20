// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/signature_parameter.hpp>
#include <requite/symbol.hpp>

#include <llvm/ADT/SmallVector.h>

namespace requite {

struct Signature final {
  using Self = requite::Signature;

  std::vector<requite::SignatureParameter> _parameters;
  requite::Symbol _return_type;

  // signature.cpp
  Signature() = default;
  explicit Signature(const Self& that) = default;
  explicit Signature(Self &&that) = default;
  ~Signature() = default;
  [[nodiscard]] Self& operator=(const Self& rhs) = default;
  [[nodiscard]] Self& operator=(Self &&rhs) = default;  
  [[nodiscard]] bool operator==(const Self& rhs) const;
  [[nodiscard]] bool operator!=(const Self& rhs) const;
  [[nodiscard]] bool getIsEmpty() const;
  [[nodiscard]] std::vector<requite::SignatureParameter>& getParameters();
  [[nodiscard]] const std::vector<requite::SignatureParameter>& getParameters() const;
  [[nodiscard]] requite::Symbol& getReturnType();
  [[nodiscard]] const requite::Symbol& getReturnType() const;
};

} // namespace requite
// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/source_location.hpp>

namespace requite {

struct SourceRange final {
  requite::SourceLocation start = {};
  requite::SourceLocation end = {};

};

}
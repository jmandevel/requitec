// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <catch2/catch_all.hpp>

// this macro is necessary because exceptions are disabled in release builds.
#if defined(_NDEBUG)
#define CHECK_THROWS_AS_DEBUG(x, y)
#else
#define CHECK_THROWS_AS_DEBUG(x, y) CHECK_THROWS_AS((x), y)
#endif
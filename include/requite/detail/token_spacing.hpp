// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/codeunits.hpp>

namespace requite {

constexpr std::string_view getDescription(requite::TokenSpacing spacing)
{
    switch (spacing)
    {
        case requite::TokenSpacing::NONE:
            return "no space before or after";
        case requite::TokenSpacing::BEFORE:
            return "a space before and not after";
        case requite::TokenSpacing::AFTER:
            return "a space after and not before";
        case requite::TokenSpacing::BEFORE_AND_AFTER:
            return "spaces before and after";
    }    
    return "unknown spacing";
}

constexpr requite::TokenSpacing getSpacing(char before, char after)
{
    if (requite::getIsWhitespace(before))
    {
        if (requite::getIsWhitespace(after))
        {
            return requite::TokenSpacing::BEFORE_AND_AFTER;
        }
        return requite::TokenSpacing::BEFORE;
    }
    if (requite::getIsWhitespace(after))
    {
        return requite::TokenSpacing::AFTER;
    }
    return requite::TokenSpacing::NONE;
}

}
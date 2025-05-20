// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/anonymous_object.hpp>

namespace requite {

bool AnonymousObject::operator==(const Self &rhs) const
{
    return this->getProperties() == rhs.getProperties();
}

bool AnonymousObject::operator!=(const Self &rhs) const
{
    return !(*this == rhs);
}

std::vector<requite::AnonymousProperty> &AnonymousObject::getProperties()
{
    return this->_properties;
}

const std::vector<requite::AnonymousProperty> &
AnonymousObject::getProperties() const
{
    return this->_properties;
}

} // namespace requite
#pragma once

#include <requite/see/root_type.hpp>

#include <array>

namespace requite {

struct Root final {
    using Self = requite::Root;

    requite::RootType _type;
};

}
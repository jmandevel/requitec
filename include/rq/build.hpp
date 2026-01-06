#pragma once

namespace rq {

struct Module;

struct Builder final {
    using Self = rq::Builder;

    [[nodiscard]] bool buildLlvmIr(rq::Module& module);

    // TODO
};

}
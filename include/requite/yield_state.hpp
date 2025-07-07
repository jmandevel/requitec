#pragma once 

namespace requite {

enum class YieldState {
    NONE,
    DONE,
    ERROR,
    EXPAND_NAME,
    EVALUATE_NAME,
    EXPAND_BODY
};

}
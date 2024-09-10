#pragma once

#include "hardware.hpp"

namespace mubloq
{
    struct model
    {
        hardware::model hardware;
    };
    
    using actions = std::variant<
        hardware::actions
    >;

    model update(model m, actions a);
} // namespace hardware

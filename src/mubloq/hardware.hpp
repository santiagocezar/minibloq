#pragma once

#include <string>
#include <immer/vector.hpp>
#include <variant>

namespace hardware
{
    struct model
    {
        immer::vector<std::string> available_ports;
        int selected_port = -1;
    };


    struct reload_ports {};
    struct select_port {
        int idx;
    };
    
    using actions = std::variant<
        reload_ports,
        select_port
    >;

    model update(model m, actions a);
} // namespace hardware

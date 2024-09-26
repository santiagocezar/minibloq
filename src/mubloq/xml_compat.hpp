#pragma once

#include "hardware.hpp"

#include <vector>

namespace hardware {
    std::vector<board> load_all_boards(std::string path);
}

#include "model.hpp"

#include <utility>


namespace mubloq
{
    model _update(model m, hardware::actions a) {
        m.hardware = hardware::update(m.hardware, a);
        return m;
    }

    model update(model m, actions a) {
        return std::visit([&] (auto a) { return _update(m, a); }, a);
    }
} // namespace hardware

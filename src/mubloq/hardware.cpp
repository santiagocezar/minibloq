#include "hardware.hpp"
#include "xml_compat.hpp"

#include <vector>
#include <string>
#include <fmt/core.h>

namespace hardware
{
    //##Make the limit configurable, from an XML file (and see the Windows and Linux API documetation too
    //to avoid defining it smaller than possibly needed):
    const int MAX_PORTS = 256;


    model _update(model m, reload_ports a) {
        std::vector<std::string> ports;
    
    // TODO: probably remove this as CTB already supports Windows.
    #if defined (WIN32)
        // apparently Alan Kharsansky made this code, cool!
        for (int portNumber = 1; portNumber < MAX_PORTS; ++portNumber)
        {
            auto portPath = fmt::format("//./COM{}", portNumber);

            HANDLE hFile = ::CreateFile(portPath.c_str(), GENERIC_READ |
                                        GENERIC_WRITE, 0, NULL,
                                        OPEN_EXISTING, 0, NULL);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                comboBootPortName->append(strCommScreenReal);
                ports.push_back(std::string(portPath));
            }
            CloseHandle(hFile);
        }
    #else
        //Thanks to Juan Pizarro for this code!:
        ctb::GetAvailablePorts(ports);
    #endif
        if (m.selected_port >= 0) {
            auto prev_port = m.available_ports[m.selected_port];
            auto it = find(ports.begin(), ports.end(), prev_port);
            if (it != ports.end()) {
                m.selected_port = it - ports.begin();
            } else {
                m.selected_port = -1;
            }
        }
        
        m.available_ports = immer::vector<std::string>(ports.begin(), ports.end());

        return m;
    }
    model _update(model m, select_port a) {
        m.selected_port = a.idx;
        return m;
    }
    model _update(model m, load_hardware a) {
        auto boards = load_all_boards(a.path);
        m.boards = immer::vector<board>(boards.begin(), boards.end());
        return m;
    }

    model update(model m, actions a) {
        return std::visit([&] (auto a) { return _update(m, a); }, a);
    }
} // namespace hardware

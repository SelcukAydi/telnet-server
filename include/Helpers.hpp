#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace sia::lts::helper
{
    inline std::string convertToString(const std::vector<std::uint8_t>& bytes)
    {
        std::string str{bytes.begin(), bytes.end()};
        return str;
    }
}
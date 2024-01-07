#include "ParserModules/WindowSizeParserModule.hpp"
#include "detail/TelnetCommands.hpp"
#include <cstdint>
#include <iostream>
#include <memory>

namespace sia::lts
{
std::shared_ptr<detail::Command> WindowSizeParser::parse(const std::vector<std::uint8_t>& command)
{
    if(command.size() == 1)
    {
        return std::make_shared<detail::WindowSize>(0, 0);
    }

    if(command.size() != 5)
    {
        std::cerr << "Corrupted command bytes\n";
        return nullptr;
    }

    std::uint16_t width{0};
    width = width | command[1];
    width = width << 8;
    width = width | command[2];

    std::uint16_t height{0};
    height = height | command[3];
    height = height << 8;
    height = height | command[4];

    std::cout << "Width is " << width << " Height is " << height << '\n';

    return std::make_shared<detail::WindowSize>(width, height);
}
}  // namespace sia::lts
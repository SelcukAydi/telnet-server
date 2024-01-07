#include "ParserModules/TerminalSpeedParserModule.hpp"
#include "detail/TelnetCommands.hpp"
#include <cstdint>
#include <memory>
#include <iostream>
#include <Helpers.hpp>
#include <string>

namespace sia::lts
{
std::shared_ptr<detail::Command> TerminalSpeedParser::parse(const std::vector<std::uint8_t>& command)
{
    if(command.size() == 1)
    {
        return std::make_shared<detail::TerminalSpeed>();
    }

    if(command.size() < 3)
    {
        std::cerr << "This is not a terminal type command\n";
        return nullptr;
    }

    if (command[1] != 0x00)
    {
        std::cerr << "Could not find IS command\n";
        return nullptr;
    }

    auto terminal_speed_command = std::make_shared<detail::TerminalSpeed>();
    std::string speed_str = helper::convertToString({command.begin() + 2, command.end()});
    std::string transmit_speed_str = speed_str.substr(0, speed_str.find_first_of(','));
    std::string receive_speed_str = speed_str.substr(speed_str.find_first_of(',') + 1);
    std::uint64_t transmit_speed = std::stoll(transmit_speed_str);
    std::uint64_t receive_speed = std::stoll(receive_speed_str);

    terminal_speed_command->m_transmit_speed = transmit_speed;
    terminal_speed_command->m_receive_speed = receive_speed;

    return terminal_speed_command;
}
}  // namespace sia::lts
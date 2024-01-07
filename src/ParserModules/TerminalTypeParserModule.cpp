#include "ParserModules/TerminalTypeParserModule.hpp"
#include "detail/TelnetCommands.hpp"
#include "Helpers.hpp"
#include <cstdint>
#include <memory>
#include <iostream>

namespace sia::lts
{
std::shared_ptr<detail::Command> TerminalTypeParser::parse(const std::vector<std::uint8_t>& command)
{
    if(command.size() == 1)
    {
        return std::make_shared<detail::TerminalType>();
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

    auto terminal_type_command = std::make_shared<detail::TerminalType>();
    terminal_type_command->m_terminal_name = helper::convertToString({command.begin() + 2, command.end()});

    std::cout << "Hand shaked on terminal type => " << terminal_type_command->m_terminal_name << '\n';

    return terminal_type_command;
}
}  // namespace sia::lts
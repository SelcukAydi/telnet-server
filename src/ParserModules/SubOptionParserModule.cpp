#include "ParserModules/SubOptionParserModule.hpp"
#include "detail/NVTCommandParser.hpp"
#include "detail/TelnetCommands.hpp"
#include <iostream>
#include <memory>

namespace sia::lts
{
std::shared_ptr<detail::Command> SubOptionParser::parse(const std::vector<std::uint8_t>& command)
{
    auto parser = detail::NVTCommandParser::findParser(command[1]);
    if(parser == nullptr)
    {
        throw detail::ParserNotRegisteredException(command[1]);
    }

    if(*command.rbegin() != 0xF0)
    {
        std::cerr << "Corrupted sub option command. No end found\n";
        return nullptr;
    }

    auto suboption_command = std::make_shared<detail::SB>();
    std::vector<std::uint8_t> sub_command_bytes{command.begin() + 1, command.end() - 1};
    auto sub_command = parser->parse(sub_command_bytes);
    suboption_command->m_sub_command = sub_command;
    return suboption_command;
}
}  // namespace sia::lts
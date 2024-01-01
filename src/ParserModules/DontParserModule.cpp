#include "ParserModules/DontParserModule.hpp"
#include "detail/TelnetCommands.hpp"
#include <cstdint>
#include <memory>

namespace sia::lts
{
std::shared_ptr<detail::Command> DontParser::parse(const std::vector<std::uint8_t>& command)
{
    auto parser = detail::NVTCommandParser::findParser(command[1]);
    if (parser == nullptr)
    {
        throw detail::ParserNotRegisteredException(command[1]);
    }

    auto do_command = std::make_shared<detail::Dont>();
    std::vector<std::uint8_t> sub_command_bytes{command.begin() + 1, command.end()};
    auto sub_command = parser->parse(sub_command_bytes);
    do_command->m_sub_command = sub_command;
    return do_command;
}
}  // namespace sia::lts
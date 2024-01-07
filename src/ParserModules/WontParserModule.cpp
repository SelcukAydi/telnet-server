#include "ParserModules/WontParserModule.hpp"
#include "detail/TelnetCommands.hpp"
#include <cstdint>
#include <memory>

namespace sia::lts
{
std::shared_ptr<detail::Command> WontParser::parse(const std::vector<std::uint8_t>& command)
{
    auto parser = detail::NVTCommandParser::findParser(command[1]);
    if (parser == nullptr)
    {
        throw detail::ParserNotRegisteredException(command[1]);
    }

    auto wont_command = std::make_shared<detail::Wont>();
    std::vector<std::uint8_t> sub_command_bytes{command.begin() + 1, command.end()};
    auto sub_command = parser->parse(sub_command_bytes);
    wont_command->m_sub_command = sub_command;
    return wont_command;
}
}  // namespace sia::lts
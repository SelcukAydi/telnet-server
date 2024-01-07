#include "ParserModules/StatusParserModule.hpp"
#include "detail/TelnetCommands.hpp"
#include <cstdint>
#include <memory>

namespace sia::lts
{
std::shared_ptr<detail::Command> StatusParser::parse(const std::vector<std::uint8_t>& command)
{
    return std::make_shared<detail::Status>();
}
}  // namespace sia::lts
#include "ParserModules/LineModeParserModule.hpp"
#include "detail/TelnetCommands.hpp"
#include <cstdint>
#include <memory>

namespace sia::lts
{
std::shared_ptr<detail::Command> LineModeParser::parse(const std::vector<std::uint8_t>& command)
{
    return std::make_shared<detail::LineMode>();
}
}  // namespace sia::lts
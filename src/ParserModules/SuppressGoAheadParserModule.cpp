#include "ParserModules/SuppressGoAheadParserModule.hpp"
#include "detail/TelnetCommands.hpp"
#include <cstdint>
#include <memory>

namespace sia::lts
{
std::shared_ptr<detail::Command> SupressGoAheadParser::parse(const std::vector<std::uint8_t>& command)
{
    return std::make_shared<detail::SuppressGoAhead>();
}
}  // namespace sia::lts
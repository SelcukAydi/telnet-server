#pragma once

#include "detail/NVTCommandParser.hpp"
#include "detail/TelnetCommands.hpp"
#include <memory>

namespace sia::lts
{
class XDisplayLocationParser : public detail::CommandParserModuleBase
{
    public:
    XDisplayLocationParser() = default;

    std::shared_ptr<detail::Command> parse(const std::vector<std::uint8_t>& command) override;

    private:
};
}  // namespace sia::lts
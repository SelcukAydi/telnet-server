#pragma once

#include "detail/NVTCommandParser.hpp"
#include "detail/TelnetCommands.hpp"
#include <memory>

namespace sia::lts
{
class WontParser : public detail::CommandParserModuleBase
{
    public:
    WontParser() = default;

    std::shared_ptr<detail::Command> parse(const std::vector<std::uint8_t>& command) override;

    private:
};
}  // namespace sia::lts
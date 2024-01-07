#pragma once

#include "detail/NVTCommandParser.hpp"
#include "detail/TelnetCommands.hpp"
#include <memory>

namespace sia::lts
{
class NewEnvOptionParser : public detail::CommandParserModuleBase
{
    public:
    NewEnvOptionParser() = default;

    std::shared_ptr<detail::Command> parse(const std::vector<std::uint8_t>& command) override;

    private:
};
}  // namespace sia::lts
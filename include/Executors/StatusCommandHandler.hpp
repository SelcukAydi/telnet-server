#pragma once

#include "Executors/NVTCommandExecutor.hpp"
#include "detail/TelnetCommands.hpp"

namespace sia::lts
{
struct StatusCommandHandler : executor::Handler<detail::Status>
{
    explicit StatusCommandHandler(std::shared_ptr<executor::NVTCommandExecutor> executor) : Handler(std::move(executor))
    {
    }

    Bytes Will(const CommandT& command) override;
    Bytes Wont(const CommandT& command) override;
    Bytes Do(const CommandT& command) override;
    Bytes Dont(const CommandT& command) override;
    Bytes SB(const CommandT& command) override;
};
}  // namespace sia::lts
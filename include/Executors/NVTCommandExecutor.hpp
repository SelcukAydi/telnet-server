#pragma once

#include "detail/TelnetCommands.hpp"
#include "detail/ExecutorBase.hpp"
#include <memory>

namespace sia::lts::executor
{

class TelnetSession;

class NVTCommandExecutor : public detail::ExecutorBase
{
    public:
    explicit NVTCommandExecutor(const std::shared_ptr<detail::TelnetSession>& session);

    void execute(const std::shared_ptr<detail::Command>& command) override;
    void processCommand(const std::shared_ptr<detail::Command>& parent_command,
                        const std::shared_ptr<detail::Command>& command);
    void handleDoCommand(const std::shared_ptr<detail::Do>& command);
    void handleDontCommand(const std::shared_ptr<detail::Dont>& command);
    void handleWillCommand(const std::shared_ptr<detail::Will>& command);
    void handleWontCommand(const std::shared_ptr<detail::Wont>& command);
    void handleEchoCommand(const std::shared_ptr<detail::Command>& parent_command);
    void handleSBCommand(const std::shared_ptr<detail::SB>& command);
    void handleWindowSizeCommand(const std::shared_ptr<detail::Command>& parent_command,
                                 const std::shared_ptr<detail::WindowSize>& command);
};
}  // namespace sia::lts::executor
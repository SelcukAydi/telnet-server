#include "Executors/NVTCommandExecutor.hpp"
#include "detail/TelnetCommands.hpp"
#include "detail/TelnetSession.hpp"
#include <iostream>
#include <memory>

namespace sia::lts::executor
{
NVTCommandExecutor::NVTCommandExecutor(const std::shared_ptr<detail::TelnetSession>& session) : ExecutorBase(session)
{
}

void NVTCommandExecutor::execute(const std::shared_ptr<detail::Command>& command)
{
    processCommand(nullptr, command);
    commit();
}

void NVTCommandExecutor::processCommand(const std::shared_ptr<detail::Command>& parent_command,
                                        const std::shared_ptr<detail::Command>& command)
{
    if (command->getName() == "DO")
    {
        auto do_command = std::dynamic_pointer_cast<detail::Do>(command);
        handleDoCommand(do_command);
    }
    else if (command->getName() == "DONT")
    {
        auto dont_command = std::dynamic_pointer_cast<detail::Dont>(command);
        handleDontCommand(dont_command);
    }
    else if (command->getName() == "WILL")
    {
        auto will_command = std::dynamic_pointer_cast<detail::Will>(command);
        handleWillCommand(will_command);
    }
    else if (command->getName() == "WONT")
    {
        auto wont_command = std::dynamic_pointer_cast<detail::Wont>(command);
        handleWontCommand(wont_command);
    }
    else if ((command->getName() == "SB"))
    {
        auto sub_option_command = std::dynamic_pointer_cast<detail::SB>(command);
        handleSBCommand(sub_option_command);
    }
    else if (command->getName() == "NAWS")
    {
        auto window_size_command = std::dynamic_pointer_cast<detail::WindowSize>(command);
        handleWindowSizeCommand(parent_command, window_size_command);
    }
    else if(command->getName() == "SuppressGoAhead")
    {
        
    }
    else
    {
        std::cerr << "Unknown command: " << command->getName() << '\n';
    }
}

void NVTCommandExecutor::handleDoCommand(const std::shared_ptr<detail::Do>& command)
{
    if (command->m_sub_command->getName() == "ECHO")
    {
        handleEchoCommand(command);
    }
}

void NVTCommandExecutor::handleDontCommand(const std::shared_ptr<detail::Dont>& command)
{
    if (command->m_sub_command->getName() == "ECHO")
    {
        handleEchoCommand(command);
    }
}

void NVTCommandExecutor::handleWillCommand(const std::shared_ptr<detail::Will>& command)
{
    if (command->m_sub_command->getName() == "ECHO")
    {
        handleEchoCommand(command);
    }
}

void NVTCommandExecutor::handleWontCommand(const std::shared_ptr<detail::Wont>& command)
{
    if (command->m_sub_command->getName() == "ECHO")
    {
        handleEchoCommand(command);
    }
}

void NVTCommandExecutor::handleEchoCommand(const std::shared_ptr<detail::Command>& parent_command)
{
    if (parent_command->getName() == "DO")
    {
        send({0xFF, 0xFB, 0x01});
    }
    else if (parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFC, 0x01});
    }
}

void NVTCommandExecutor::handleSBCommand(const std::shared_ptr<detail::SB>& command)
{
    processCommand(command, command->m_sub_command);
}

void NVTCommandExecutor::handleWindowSizeCommand(const std::shared_ptr<detail::Command>& parent_command,
                                                 const std::shared_ptr<detail::WindowSize>& command)
{
    if (parent_command == nullptr)
    {
        std::cerr << "Window size command must have a parent command\n";
        return;
    }

    if (parent_command->getName() == "DO")
    {
        send({0xFF, 0xFB, 0x1F});
    }
    else if (parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFC, 0x1F});
    }
    else if (parent_command->getName() == "WILL")
    {
        send({0xFF, 0xFD, 0x1F});
    }
    else if (parent_command->getName() == "WONT")
    {
        send({0xFF, 0xFE, 0x1F});
    }
    else if (parent_command->getName() == "SB")
    {
        std::cout << "Negotiate on window size. Width: " << command->m_width << " height:" << command->m_height << '\n';
    }
}

}  // namespace sia::lts::executor
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
    // commit();
}

void NVTCommandExecutor::processCommand(const std::shared_ptr<detail::Command>& parent_command,
                                        const std::shared_ptr<detail::Command>& command)
{
    if (command->m_sub_command == nullptr)
    {
        std::cerr << "Sub command cannot be nullptr\n";
        return;
    }

    auto itr = m_handlers.find(command->m_sub_command->getCode());
    if (itr == m_handlers.end())
    {
        std::cout << "No handler registered at code " << command->m_sub_command->getCode() << '\n';
        return;
    }

    Bytes bytes_to_sent;

    if (command->getName() == "DO")
    {
        bytes_to_sent = itr->second.Do(command->m_sub_command);
    }
    else if (command->getName() == "DONT")
    {
        bytes_to_sent = itr->second.Dont(command->m_sub_command);
    }
    else if (command->getName() == "WILL")
    {
        bytes_to_sent = itr->second.Will(command->m_sub_command);
    }
    else if (command->getName() == "WONT")
    {
        bytes_to_sent = itr->second.Wont(command->m_sub_command);
    }
    else if (command->getName() == "SB")
    {
        bytes_to_sent = itr->second.SB(command->m_sub_command);
    }
    else
    {
        std::cerr << "Unknown command detected\n";
        return;
    }

    send(bytes_to_sent);

    // --------------------------------------------------------------

    // if (command->getName() == "DO" || command->getName() == "DONT" || command->getName() == "WILL" ||
    //     command->getName() == "WONT")
    // {
    //     if (command->m_sub_command == nullptr)
    //     {
    //         std::cerr << "Sub command cannot be nullptr\n";
    //         return;
    //     }

    //     if (command->m_sub_command->getName() == "SuppressGoAhead")
    //     {
    //         handleSuppressGoAheadCommand(command);
    //     }
    //     else if (command->m_sub_command->getName() == "TerminalType")
    //     {
    //         handleTerminalTypeCommand(command);
    //     }
    //     else if (command->m_sub_command->getName() == "NAWS")
    //     {
    //         handleWindowSizeCommand(command);
    //     }
    //     else if (command->m_sub_command->getName() == "TerminalSpeed")
    //     {
    //         handleTerminalSpeedCommand(command);
    //     }
    //     else if (command->m_sub_command->getName() == "RemoteFlowControl")
    //     {
    //         handleRemoteFlowControlCommand(command);
    //     }
    //     else if (command->m_sub_command->getName() == "LineMode")
    //     {
    //         handleLineModeCommand(command);
    //     }
    //     else if (command->m_sub_command->getName() == "NewEnvOption")
    //     {
    //         handleNewEnvOptionCommand(command);
    //     }
    //     else if (command->m_sub_command->getName() == "Status")
    //     {
    //         handleStatusCommand(command);
    //     }
    // }
    // else if ((command->getName() == "SB"))
    // {
    //     auto sub_option_command = std::dynamic_pointer_cast<detail::SB>(command);
    //     handleSBCommand(sub_option_command);
    // }
    // else if (command->getName() == "NAWS")
    // {
    //     handleWindowSizeCommand(parent_command);
    // }
    // else if (command->getName() == "TerminalSpeed")
    // {
    //     handleTerminalSpeedCommand(parent_command);
    // }
    // else if (command->getName() == "TerminalType")
    // {
    //     handleTerminalTypeCommand(parent_command);
    // }
    // else if (command->getName() == "NewEnvOption")
    // {
    //     handleNewEnvOptionCommand(parent_command);
    // }
    // else
    // {
    //     std::cerr << "Unknown command: " << command->getName() << '\n';
    // }
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

void NVTCommandExecutor::handleWindowSizeCommand(const std::shared_ptr<detail::Command>& parent_command)
{
    if (parent_command == nullptr)
    {
        std::cerr << "Window size command must have a parent command\n";
        return;
    }

    if (parent_command->getName() == "DO" || parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFC, 0x1F});
        return;
    }
    else if (parent_command->getName() == "WILL" || parent_command->getName() == "WONT")
    {
        send({0xFF, 0xFD, 0x1F});
        return;
    }
    else if (parent_command->getName() == "SB")
    {
        auto window_size_command = std::dynamic_pointer_cast<detail::WindowSize>(parent_command->m_sub_command);
        if (window_size_command == nullptr)
        {
            std::cerr << "Window size command is nullptr under SB parent command\n";
            return;
        }
        std::cout << "Negotiate on window size. Width: " << window_size_command->m_width
                  << " height:" << window_size_command->m_height << '\n';

        auto session_data = getSession()->getSessionData();
        session_data->m_width = window_size_command->m_width;
        session_data->m_height = window_size_command->m_height;

        send({0xFF, 0xFA, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0});
    }
}

void NVTCommandExecutor::handleSuppressGoAheadCommand(const std::shared_ptr<detail::Command>& parent_command)
{
    if (parent_command->getName() == "DO" || parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFB, 0x03});
    }
    else if (parent_command->getName() == "WILL" || parent_command->getName() == "WONT")
    {
        send({0xFF, 0xFD, 0x03});
    }
}

void NVTCommandExecutor::handleTerminalTypeCommand(const std::shared_ptr<detail::Command>& parent_command)
{
    if (parent_command->getName() == "DO")
    {
        send({0xFF, 0xFC, 0x18});
    }
    else if (parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFC, 0x18});
    }
    else if (parent_command->getName() == "WILL")
    {
        send({0xFF, 0xFD, 0x18});
    }
    else if (parent_command->getName() == "WONT")
    {
        send({0xFF, 0xFE, 0x18});
    }
    else if (parent_command->getName() == "SB")
    {
        auto terminal_type_command = std::dynamic_pointer_cast<detail::TerminalType>(parent_command->m_sub_command);
        if (terminal_type_command == nullptr)
        {
            std::cerr << "Terminal type command is nullptr\n";
            return;
        }
        std::cout << "Terminal type negotiate on => " << terminal_type_command->m_terminal_name << '\n';
        auto session_data = getSession()->getSessionData();
        session_data->m_terminal_name = terminal_type_command->m_terminal_name;
        return;
    }

    send({0xFF, 0xFA, 0x18, 0x01, 0xFF, 0xF0});
}

void NVTCommandExecutor::handleTerminalSpeedCommand(const std::shared_ptr<detail::Command>& parent_command)
{
    if (parent_command->getName() == "DO")
    {
        send({0xFF, 0xFC, 0x20});
    }
    else if (parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFC, 0x20});
    }
    else if (parent_command->getName() == "WILL")
    {
        send({0xFF, 0xFD, 0x20});
    }
    else if (parent_command->getName() == "WONT")
    {
        send({0xFF, 0xFE, 0x20});
    }
    else if (parent_command->getName() == "SB")
    {
        auto terminal_speed_command = std::dynamic_pointer_cast<detail::TerminalSpeed>(parent_command->m_sub_command);
        if (terminal_speed_command == nullptr)
        {
            std::cerr << "Terminal speed command is nullptr\n";
            return;
        }
        std::cout << "Terminal speed negotiate on => " << terminal_speed_command->m_transmit_speed << ','
                  << terminal_speed_command->m_receive_speed << '\n';
        return;
    }

    send({0xFF, 0xFA, 0x20, 0x01, 0xFF, 0xF0});
}

void NVTCommandExecutor::handleRemoteFlowControlCommand(const std::shared_ptr<detail::Command>& parent_command)
{
    if (parent_command->getName() == "DO")
    {
        send({0xFF, 0xFC, 0x21});
    }
    else if (parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFC, 0x21});
    }
    else if (parent_command->getName() == "WILL")
    {
        send({0xFF, 0xFD, 0x21});
    }
    else if (parent_command->getName() == "WONT")
    {
        send({0xFF, 0xFE, 0x21});
    }
    else if (parent_command->getName() == "SB")
    {
        auto flow_control_command = std::dynamic_pointer_cast<detail::RemoteFlowControl>(parent_command->m_sub_command);
        if (flow_control_command == nullptr)
        {
            std::cerr << "Flow control command is nullptr\n";
            return;
        }

        std::cout << "Flow control command processed successfully\n";
        return;
    }

    send({0xFF, 0xFA, 0x21, 0x01, 0xFF, 0xF0});
}

void NVTCommandExecutor::handleLineModeCommand(const std::shared_ptr<detail::Command>& parent_command)
{
    if (parent_command->getName() == "DO" || parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFC, 0x22});
    }
    else if (parent_command->getName() == "WILL" || parent_command->getName() == "WONT")
    {
        send({0xFF, 0xFE, 0x22});
    }
}

void NVTCommandExecutor::handleNewEnvOptionCommand(const std::shared_ptr<detail::Command>& parent_command)
{
    if (parent_command->getName() == "DO")
    {
        send({0xFF, 0xFC, 0x27});
    }
    else if (parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFC, 0x27});
    }
    else if (parent_command->getName() == "WILL")
    {
        send({0xFF, 0xFD, 0x27});
    }
    else if (parent_command->getName() == "WONT")
    {
        send({0xFF, 0xFE, 0x27});
    }
    else if (parent_command->getName() == "SB")
    {
        auto new_env_option_command = std::dynamic_pointer_cast<detail::NewEnvOption>(parent_command->m_sub_command);
        if (new_env_option_command == nullptr)
        {
            std::cerr << "New env option command is nullptr\n";
            return;
        }

        std::cout << "New env option command processed successfully\n";
        return;
    }

    send({0xFF, 0xFA, 0x27, 0x01, 0xFF, 0xF0});
}

void NVTCommandExecutor::handleStatusCommand(const std::shared_ptr<detail::Command>& parent_command)
{
    if (parent_command->getName() == "DO" || parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFB, 0x05});
    }
    else if (parent_command->getName() == "WILL" || parent_command->getName() == "WONT")
    {
        send({0xFF, 0xFD, 0x05});
    }
}

void NVTCommandExecutor::handleXDisplayLocationCommand(const std::shared_ptr<detail::Command>& parent_command)
{
    if (parent_command->getName() == "DO" || parent_command->getName() == "DONT")
    {
        send({0xFF, 0xFB, 0x23});
    }
    else if (parent_command->getName() == "WILL" || parent_command->getName() == "WONT")
    {
        send({0xFF, 0xFD, 0x23});
    }

    send({0xFF, 0xFA, 0x23, 0x01, 0xFF, 0xF0});
}

}  // namespace sia::lts::executor
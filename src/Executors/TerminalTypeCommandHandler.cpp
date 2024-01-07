#include "detail/TelnetSession.hpp"
#include "Executors/TerminalTypeCommandHandler.hpp"
#include <iostream>
#include <memory>

namespace sia::lts
{
Bytes TerminalTypeCommandHandler::Will(const CommandT& command)
{
    send({0xFF, 0xFD, 0x18});
    send({0xFF, 0xFA, 0x18, 0x01, 0xFF, 0xF0});
    return commit();
}
Bytes TerminalTypeCommandHandler::Wont(const CommandT& command)
{
    send({0xFF, 0xFE, 0x18});
    return commit();
}
Bytes TerminalTypeCommandHandler::Do(const CommandT& command)
{
    send({0xFF, 0xFB, 0x18});
    send({0xFF, 0xFA, 0x18, 0x01, 0xFF, 0xF0});
    return commit();
}
Bytes TerminalTypeCommandHandler::Dont(const CommandT& command)
{
    send({0xFF, 0xFC, 0x18});
    return commit();
}
Bytes TerminalTypeCommandHandler::SB(const CommandT& command)
{
    auto terminal_type_command = std::dynamic_pointer_cast<detail::TerminalType>(command);
    if (terminal_type_command == nullptr)
    {
        std::cerr << "Terminal type command is nullptr\n";
        return commit();
    }
    std::cout << "Terminal type negotiate on => " << terminal_type_command->m_terminal_name << '\n';
    std::shared_ptr<detail::TelnetSessionData> session_data = m_executor.lock()->getSession()->getSessionData();
    session_data->m_terminal_name = terminal_type_command->m_terminal_name;
    return commit();
}
}  // namespace sia::lts
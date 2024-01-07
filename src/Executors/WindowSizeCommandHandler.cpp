#include "detail/TelnetSession.hpp"
#include "Executors/WindowSizeCommandHandler.hpp"
#include <iostream>
#include <memory>

namespace sia::lts
{
Bytes WindowSizeCommandHandler::Will(const CommandT& command)
{
    send({0xFF, 0xFD, 0x1F});
    send({0xFF, 0xFA, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0});
    return commit();
}
Bytes WindowSizeCommandHandler::Wont(const CommandT& command)
{
    send({0xFF, 0xFD, 0x1F});
    return commit();
}
Bytes WindowSizeCommandHandler::Do(const CommandT& command)
{
    send({0xFF, 0xFC, 0x1F});
    send({0xFF, 0xFA, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0});
    return commit();
}
Bytes WindowSizeCommandHandler::Dont(const CommandT& command)
{
    send({0xFF, 0xFC, 0x1F});
    return commit();
}
Bytes WindowSizeCommandHandler::SB(const CommandT& command)
{
    auto window_size_command = std::dynamic_pointer_cast<detail::WindowSize>(command);
    if (window_size_command == nullptr)
    {
        std::cerr << "Window size command is nullptr under SB parent command\n";
        return commit();
    }
    std::cout << "Negotiate on window size. Width: " << window_size_command->m_width
              << " height:" << window_size_command->m_height << '\n';

    std::shared_ptr<detail::TelnetSessionData> session_data = m_executor.lock()->getSession()->getSessionData();
    session_data->m_width = window_size_command->m_width;
    session_data->m_height = window_size_command->m_height;
    return commit();
}
}  // namespace sia::lts
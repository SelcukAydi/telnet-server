#include "Executors/RemoteFlowControlCommandHandler.hpp"
#include <iostream>

namespace sia::lts
{
Bytes RemoteFlowControlCommandHandler::Will(const CommandT& command)
{
    send({0xFF, 0xFD, 0x21});
    send({0xFF, 0xFA, 0x21, 0x01, 0xFF, 0xF0});
    return commit();
}
Bytes RemoteFlowControlCommandHandler::Wont(const CommandT& command)
{
    send({0xFF, 0xFE, 0x21});
    return commit();
}
Bytes RemoteFlowControlCommandHandler::Do(const CommandT& command)
{
    send({0xFF, 0xFB, 0x21});
    send({0xFF, 0xFA, 0x21, 0x01, 0xFF, 0xF0});
    return commit();
}
Bytes RemoteFlowControlCommandHandler::Dont(const CommandT& command)
{
    send({0xFF, 0xFC, 0x21});
    return commit();
}
Bytes RemoteFlowControlCommandHandler::SB(const CommandT& command)
{
    auto flow_control_command = std::dynamic_pointer_cast<detail::RemoteFlowControl>(command);
    if (flow_control_command == nullptr)
    {
        std::cerr << "Flow control command is nullptr\n";
        return commit();
    }

    std::cout << "Flow control command processed successfully\n";
    return commit();
}
}  // namespace sia::lts
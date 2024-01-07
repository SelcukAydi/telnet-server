#include "Executors/TerminalSpeedCommandHandler.hpp"

namespace sia::lts
{
Bytes TerminalSpeedCommandHandler::Will(const CommandT& command)
{
    send({0xFF, 0xFD, 0x20});
    send({0xFF, 0xFA, 0x20, 0x01, 0xFF, 0xF0});
    return commit();
}
Bytes TerminalSpeedCommandHandler::Wont(const CommandT& command)
{
    send({0xFF, 0xFE, 0x20});
    return commit();
}
Bytes TerminalSpeedCommandHandler::Do(const CommandT& command)
{
    send({0xFF, 0xFB, 0x20});
    send({0xFF, 0xFA, 0x20, 0x01, 0xFF, 0xF0});
    return commit();
}
Bytes TerminalSpeedCommandHandler::Dont(const CommandT& command)
{
    send({0xFF, 0xFC, 0x20});
    return commit();
}
Bytes TerminalSpeedCommandHandler::SB(const CommandT& command)
{
    return commit();
}
}  // namespace sia::lts
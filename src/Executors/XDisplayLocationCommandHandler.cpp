#include "Executors/XDisplayLocationCommandHandler.hpp"

namespace sia::lts
{
Bytes XDisplayLocationCommandHandler::Will(const CommandT& command)
{
    send({0xFF, 0xFD, 0x23});
    send({0xFF, 0xFA, 0x23, 0x01, 0xFF, 0xF0});
    return commit();
}
Bytes XDisplayLocationCommandHandler::Wont(const CommandT& command)
{
    send({0xFF, 0xFE, 0x23});
    return commit();
}
Bytes XDisplayLocationCommandHandler::Do(const CommandT& command)
{
    send({0xFF, 0xFB, 0x23});
    send({0xFF, 0xFA, 0x23, 0x01, 0xFF, 0xF0});
    return commit();
}
Bytes XDisplayLocationCommandHandler::Dont(const CommandT& command)
{
    send({0xFF, 0xFC, 0x23});
    return commit();
}
Bytes XDisplayLocationCommandHandler::SB(const CommandT& command)
{
    return commit();
}
}  // namespace sia::lts
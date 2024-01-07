#include "Executors/EchoCommandHandler.hpp"

namespace sia::lts
{
Bytes EchoCommandHandler::Will(const CommandT& command)
{
    return commit();
}
Bytes EchoCommandHandler::Wont(const CommandT& command)
{
    return commit();
}
Bytes EchoCommandHandler::Do(const CommandT& command)
{
    send({0xFF, 0xFB, 0x01});
    return commit();
}
Bytes EchoCommandHandler::Dont(const CommandT& command)
{
    send({0xFF, 0xFC, 0x01});
    return commit();
}
Bytes EchoCommandHandler::SB(const CommandT& command)
{
    return commit();
}
}  // namespace sia::lts
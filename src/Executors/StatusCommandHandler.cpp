#include "Executors/StatusCommandHandler.hpp"

namespace sia::lts
{
Bytes StatusCommandHandler::Will(const CommandT& command)
{
    send({0xFF, 0xFD, 0x05});
    return commit();
}
Bytes StatusCommandHandler::Wont(const CommandT& command)
{
    send({0xFF, 0xFE, 0x05});
    return commit();
}
Bytes StatusCommandHandler::Do(const CommandT& command)
{
    send({0xFF, 0xFB, 0x05});
    return commit();
}
Bytes StatusCommandHandler::Dont(const CommandT& command)
{
    send({0xFF, 0xFC, 0x05});
    return commit();
}
Bytes StatusCommandHandler::SB(const CommandT& command)
{
    return commit();
}
}  // namespace sia::lts
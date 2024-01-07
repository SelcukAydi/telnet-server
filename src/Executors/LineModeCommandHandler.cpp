#include "Executors/LineModeCommandHandler.hpp"

namespace sia::lts
{
Bytes LineModeCommandHandler::Will(const CommandT& command)
{
    send({0xFF, 0xFE, 0x22});
    return commit();
}
Bytes LineModeCommandHandler::Wont(const CommandT& command)
{
    send({0xFF, 0xFE, 0x22});
    return commit();
}
Bytes LineModeCommandHandler::Do(const CommandT& command)
{
    send({0xFF, 0xFC, 0x22});
    return commit();
}
Bytes LineModeCommandHandler::Dont(const CommandT& command)
{
    send({0xFF, 0xFC, 0x22});
    return commit();
}
Bytes LineModeCommandHandler::SB(const CommandT& command)
{
    return commit();
}
}  // namespace sia::lts
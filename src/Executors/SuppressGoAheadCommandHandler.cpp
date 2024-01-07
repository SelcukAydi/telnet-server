#include "Executors/SuppressGoAheadCommandHandler.hpp"

namespace sia::lts
{
Bytes SuppressGoAheadCommandHandler::Will(const CommandT& command)
{
    send({0xFF, 0xFD, 0x03});
    return commit();
}
Bytes SuppressGoAheadCommandHandler::Wont(const CommandT& command)
{
    send({0xFF, 0xFD, 0x03});
    return commit();
}
Bytes SuppressGoAheadCommandHandler::Do(const CommandT& command)
{
    send({0xFF, 0xFB, 0x03});
    return commit();
}
Bytes SuppressGoAheadCommandHandler::Dont(const CommandT& command)
{
    send({0xFF, 0xFB, 0x03});
    return commit();
}
Bytes SuppressGoAheadCommandHandler::SB(const CommandT& command)
{
    return commit();
}
}  // namespace sia::lts
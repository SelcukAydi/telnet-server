#include "Executors/NewEnvOptionCommandHandler.hpp"
#include <iostream>

namespace sia::lts
{
Bytes NewEnvOptionCommandHandler::Will(const CommandT& command)
{
    send({0xFF, 0xFD, 0x27});
    send({0xFF, 0xFA, 0x27, 0x01, 0xFF, 0xF0});
    return commit();
}
Bytes NewEnvOptionCommandHandler::Wont(const CommandT& command)
{
    send({0xFF, 0xFE, 0x27});
    return commit();
}
Bytes NewEnvOptionCommandHandler::Do(const CommandT& command)
{
    send({0xFF, 0xFB, 0x27});
    send({0xFF, 0xFA, 0x27, 0x01, 0xFF, 0xF0});
    return commit();
}
Bytes NewEnvOptionCommandHandler::Dont(const CommandT& command)
{
    send({0xFF, 0xFC, 0x27});
    return commit();
}
Bytes NewEnvOptionCommandHandler::SB(const CommandT& command)
{
    auto new_env_option_command = std::dynamic_pointer_cast<detail::NewEnvOption>(command);
    if (new_env_option_command == nullptr)
    {
        std::cerr << "New env option command is nullptr\n";
        return commit();
    }

    std::cout << "New env option command processed successfully\n";
    return commit();
}
}  // namespace sia::lts
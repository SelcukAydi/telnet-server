#pragma once

#include "detail/TelnetCommands.hpp"
#include "detail/ExecutorBase.hpp"
#include "Executors/Caller.hpp"
#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace sia::lts::executor
{

class TelnetSession;

class NVTCommandExecutor : public std::enable_shared_from_this<NVTCommandExecutor>, public detail::ExecutorBase
{
    public:
    explicit NVTCommandExecutor(const std::shared_ptr<detail::TelnetSession>& session);

    void execute(const std::shared_ptr<detail::Command>& command) override;
    void processCommand(const std::shared_ptr<detail::Command>& parent_command,
                        const std::shared_ptr<detail::Command>& command);
    void handleDoCommand(const std::shared_ptr<detail::Do>& command);
    void handleDontCommand(const std::shared_ptr<detail::Dont>& command);
    void handleWillCommand(const std::shared_ptr<detail::Will>& command);
    void handleWontCommand(const std::shared_ptr<detail::Wont>& command);
    void handleSBCommand(const std::shared_ptr<detail::SB>& command);

    void handleEchoCommand(const std::shared_ptr<detail::Command>& parent_command);
    void handleWindowSizeCommand(const std::shared_ptr<detail::Command>& parent_command);
    void handleSuppressGoAheadCommand(const std::shared_ptr<detail::Command>& parent_command);
    void handleTerminalTypeCommand(const std::shared_ptr<detail::Command>& parent_command);
    void handleTerminalSpeedCommand(const std::shared_ptr<detail::Command>& parent_command);
    void handleRemoteFlowControlCommand(const std::shared_ptr<detail::Command>& parent_command);
    void handleLineModeCommand(const std::shared_ptr<detail::Command>& parent_command);
    void handleNewEnvOptionCommand(const std::shared_ptr<detail::Command>& parent_command);
    void handleStatusCommand(const std::shared_ptr<detail::Command>& parent_command);
    void handleXDisplayLocationCommand(const std::shared_ptr<detail::Command>& parent_command);

    template<typename CommandT, typename CommandHandlerT>
    void registerHandler(std::uint8_t code);

    private:
    std::unordered_map<std::uint8_t, Caller> m_handlers;
};

template <typename T>
struct Handler
{
    explicit Handler(std::shared_ptr<executor::NVTCommandExecutor> executor);

    using CommandT = std::shared_ptr<T>;

    virtual Bytes Will(const CommandT& command);
    virtual Bytes Wont(const CommandT& command);
    virtual Bytes Do(const CommandT& command);
    virtual Bytes Dont(const CommandT& command);
    virtual Bytes SB(const CommandT& command);

    protected:
    void send(const Bytes& msg);
    Bytes commit();
    std::weak_ptr<executor::NVTCommandExecutor> m_executor;

    private:
    Bytes m_to_be_sent_msgs;
};

template <typename T>
using CommandT = typename Handler<T>::CommandT;

template <typename T>
Handler<T>::Handler(std::shared_ptr<executor::NVTCommandExecutor> executor) : m_executor(executor)
{
}

template <typename T>
Bytes Handler<T>::Will(const CommandT& command)
{
    std::cout << "No action implemented for command " << command->getName() << '\n';
    return {};
}

template <typename T>
Bytes Handler<T>::Wont(const CommandT& command)
{
    std::cout << "No action implemented for command " << command->getName() << '\n';
    return {};
}

template <typename T>
Bytes Handler<T>::Do(const CommandT& command)
{
    std::cout << "No action implemented for command " << command->getName() << '\n';
    return {};
}

template <typename T>
Bytes Handler<T>::Dont(const CommandT& command)
{
    std::cout << "No action implemented for command " << command->getName() << '\n';
    return {};
}

template <typename T>
Bytes Handler<T>::SB(const CommandT& command)
{
    std::cout << "No action implemented for command " << command->getName() << '\n';
    return {};
}

template <typename T>
void Handler<T>::send(const std::vector<std::uint8_t>& msg)
{
    if (!msg.empty())
        m_to_be_sent_msgs.insert(m_to_be_sent_msgs.end(), msg.begin(), msg.end());
}

template <typename T>
Bytes Handler<T>::commit()
{
    auto rs = m_to_be_sent_msgs;
    m_to_be_sent_msgs.clear();
    return rs;
}

template <typename CommandT, typename CommandHandlerT>
void NVTCommandExecutor::registerHandler(std::uint8_t code)
{
    auto handler = std::make_shared<CommandHandlerT>(shared_from_this());

    auto func_will = [handler](const std::shared_ptr<detail::Command>& command) -> Bytes
    { return handler->Will(std::dynamic_pointer_cast<CommandT>(command)); };

    auto func_wont = [handler](const std::shared_ptr<detail::Command>& command) -> Bytes
    { return handler->Wont(std::dynamic_pointer_cast<CommandT>(command)); };

    auto func_do = [handler](const std::shared_ptr<detail::Command>& command) -> Bytes
    { return handler->Do(std::dynamic_pointer_cast<CommandT>(command)); };

    auto func_dont = [handler](const std::shared_ptr<detail::Command>& command) -> Bytes
    { return handler->Dont(std::dynamic_pointer_cast<CommandT>(command)); };

    auto func_sb = [handler](const std::shared_ptr<detail::Command>& command) -> Bytes
    { return handler->SB(std::dynamic_pointer_cast<CommandT>(command)); };

    Caller caller{func_will, func_wont, func_do, func_dont, func_sb};
    m_handlers.insert({code, std::move(caller)});
}

}  // namespace sia::lts::executor
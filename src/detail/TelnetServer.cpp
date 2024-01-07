#include "detail/TelnetServer.hpp"
#include "Executors/EchoCommandHandler.hpp"
#include "Executors/LineModeCommandHandler.hpp"
#include "Executors/NVTCommandExecutor.hpp"
#include "Executors/NewEnvOptionCommandHandler.hpp"
#include "Executors/RemoteFlowControlCommandHandler.hpp"
#include "Executors/StatusCommandHandler.hpp"
#include "Executors/SuppressGoAheadCommandHandler.hpp"
#include "Executors/TerminalSpeedCommandHandler.hpp"
#include "Executors/TerminalTypeCommandHandler.hpp"
#include "Executors/WindowSizeCommandHandler.hpp"
#include "Executors/XDisplayLocationCommandHandler.hpp"
#include "detail/TelnetCommands.hpp"
#include "detail/TelnetSession.hpp"

#include <iostream>
#include <memory>

namespace sia::lts::detail
{
TelnetServer::TelnetServer() : m_server(m_context, 23)
{
}

void TelnetServer::start()
{
    sia::lts::EventCallbackPack pack;

    pack.m_accept_callback = [this](std::int32_t conn_id) { this->createNewSession(conn_id); };

    pack.m_cb_on_read = [this](std::int32_t conn_id, std::string data)
    {
        std::cout << "Onread => Connection " << conn_id << ": " << data << '\n';
        this->processIncomingMsg(conn_id, data);
    };

    pack.m_cb_on_write = [](std::int32_t conn_id, std::string data)
    { std::cout << "OnWrite => Connection " << conn_id << ": " << data << '\n'; };

    m_server.start(pack);
    m_context.run();
}

void TelnetServer::send(std::int32_t connection_id, const std::string& data)
{
    m_server.send(connection_id, data);
}

std::shared_ptr<TelnetSession> TelnetServer::createNewSession(std::int32_t session_id)
{
    auto itr = m_session_map.find(session_id);
    if (itr != m_session_map.end())
    {
        std::cerr << "Session " << session_id << " already exist!\n";
        return itr->second;
    }
    std::cout << "Creating a new session with id " << session_id << '\n';
    auto session = std::make_shared<TelnetSession>(session_id, shared_from_this());
    auto executor = std::make_shared<sia::lts::executor::NVTCommandExecutor>(session);
    session->m_nvt_executor = executor;

    executor->registerHandler<detail::Echo, EchoCommandHandler>(0x01);
    executor->registerHandler<detail::LineMode, LineModeCommandHandler>(0x22);
    executor->registerHandler<detail::NewEnvOption, NewEnvOptionCommandHandler>(0x27);
    executor->registerHandler<detail::RemoteFlowControl, RemoteFlowControlCommandHandler>(0x21);
    executor->registerHandler<detail::Status, StatusCommandHandler>(0x05);
    executor->registerHandler<detail::SuppressGoAhead, SuppressGoAheadCommandHandler>(0x03);
    executor->registerHandler<detail::TerminalSpeed, TerminalSpeedCommandHandler>(0x20);
    executor->registerHandler<detail::TerminalType, TerminalTypeCommandHandler>(0x18);
    executor->registerHandler<detail::WindowSize, WindowSizeCommandHandler>(0x1F);
    executor->registerHandler<detail::XDisplayLocation, XDisplayLocationCommandHandler>(0x23);

    m_session_map.insert({session_id, session});
    return session;
}

std::shared_ptr<TelnetSession> TelnetServer::getSessionByID(std::int32_t session_id) const
{
    auto itr = m_session_map.find(session_id);
    return itr == m_session_map.end() ? nullptr : itr->second;
}

void TelnetServer::processIncomingMsg(std::int32_t session_id, const std::string& data)
{
    auto session = getSessionByID(session_id);
    if (session == nullptr)
    {
        std::cerr << "Could not find a session object against session id " << session_id << '\n';
        return;
    }
    session->processIncomingMsg(data);
}

}  // namespace sia::lts::detail
#pragma once

#include "detail/AsioBase/TCPServer.hpp"

namespace sia::lts::detail
{

class TelnetSession;

class TelnetServer : public std::enable_shared_from_this<TelnetServer>
{
    public:
    TelnetServer();

    void start();
    void send(std::int32_t connection_id, const std::string& data);

    private:
    std::shared_ptr<TelnetSession> createNewSession(std::int32_t session_id);
    std::shared_ptr<TelnetSession> getSessionByID(std::int32_t session_id) const;
    void processIncomingMsg(std::int32_t session_id, const std::string& data);

    boost::asio::io_context m_context;
    sia::lts::TCPServer m_server;
    std::unordered_map<std::int32_t, std::shared_ptr<TelnetSession>> m_session_map;
};
}  // namespace sia::lts::detail
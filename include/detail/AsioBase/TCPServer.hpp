#pragma once

#include "detail/AsioBase/Connection.hpp"

namespace sia::lts::detail
{
struct EventCallbackPack
{
    Connection::CallbackOnRead m_cb_on_read;
    Connection::CallbackOnWrite m_cb_on_write;
    std::function<void(std::int32_t)> m_accept_callback;
};

class TCPServer
{
    public:
    using acceptor_type = boost::asio::ip::tcp::acceptor;
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using protocol_type = boost::asio::ip::tcp;
    using connection_accept_callback = std::function<void(std::int32_t)>;

    explicit TCPServer(boost::asio::io_context& context, std::uint16_t port);

    void start(EventCallbackPack pack);
    void send(std::int32_t conn_id, const std::string& data);

    private:
    void start();
    void onAccept(const boost::system::error_code& err, boost::asio::ip::tcp::socket&& socket);

    acceptor_type m_acceptor;
    EventCallbackPack m_event_callbacks;
    std::unordered_map<std::int32_t, boost::shared_ptr<Connection>> m_connections;
};
}  // namespace sia::lts::detail

namespace sia::lts
{
using TCPServer = detail::TCPServer;
using EventCallbackPack = detail::EventCallbackPack;
}  // namespace sia::lts
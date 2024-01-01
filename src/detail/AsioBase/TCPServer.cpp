#include "detail/AsioBase/TCPServer.hpp"

#include <iostream>

namespace sia::lts::detail
{
TCPServer::TCPServer(boost::asio::io_context& context, std::uint16_t port)
    : m_acceptor(context, endpoint_type(protocol_type::v4(), port))
{
}

void TCPServer::start(EventCallbackPack pack)
{
    m_event_callbacks = std::move(pack);
    start();
}

void TCPServer::send(std::int32_t conn_id, const std::string& data)
{
    auto itr = m_connections.find(conn_id);
    if (itr != m_connections.end())
    {
        itr->second->send(data);
    }
}

void TCPServer::start()
{
    m_acceptor.async_accept([this](boost::system::error_code error_code, Socket::socket_type&& socket)
                            { this->onAccept(error_code, std::move(socket)); });

    std::cout << "Waiting for a new connection on port 9080...\n";
}

void TCPServer::onAccept(const boost::system::error_code& err, boost::asio::ip::tcp::socket&& socket)
{
    static std::atomic_uint32_t count = 0;

    if (!err)
    {
        std::cout << "A new connection accepted\n";
        auto socket_obj = boost::make_shared<Socket>(std::move(socket));
        auto connection = boost::make_shared<Connection>(++count, socket_obj);
        m_connections.insert({connection->getID(), connection});
        connection->registerCallbackOnRead(m_event_callbacks.m_cb_on_read);
        connection->registerCallbackOnWrite(m_event_callbacks.m_cb_on_write);
        m_event_callbacks.m_accept_callback(connection->getID());
    }
    else
    {
        std::cerr << err.what() << '\n';
    }
    this->start();
}

}  // namespace sia::lts::detail
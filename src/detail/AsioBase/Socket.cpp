#include "detail/AsioBase/Socket.hpp"

#include <array>
#include <boost/asio/registered_buffer.hpp>
#include <type_traits>
#include <iostream>

namespace sia::lts::detail
{
Socket::Socket(socket_type&& socket) : m_socket(std::move(socket))
{
    static_assert(!std::is_copy_constructible_v<socket_type>, "socket type should not be copy constructive!");
    static_assert(!std::is_copy_assignable_v<socket_type>, "socket type should not be copy assignable!");
}

Socket::socket_type& Socket::getSocket()
{
    return m_socket;
}

void Socket::readSome(const std::function<void(std::string)>& func)
{
    m_socket.async_read_some(boost::asio::buffer(m_read_buff, kMaxReadBufferLength),
                             boost::bind(&Socket::onRead, shared_from_this(), boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred, func));
}

void Socket::writeSome(const std::function<void(std::string)>& func, const std::string& data)
{
    m_socket.async_write_some(boost::asio::buffer(data, kMaxReadBufferLength),
                              boost::bind(&Socket::onWrite, shared_from_this(), boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred, func));
}

void Socket::onRead(const boost::system::error_code& err, size_t bytes_transferred,
                    const std::function<void(std::string)>& func)
{
    if (!err)
    {
        // std::cout << "onRead New message received => " << m_read_buff.data() << '\n';
        std::string str{m_read_buff.begin(), m_read_buff.begin() + bytes_transferred};
        func(str);
    }
    else
    {
        std::cerr << "onRead Error received => " << err.what() << '\n';
    }

    m_read_buff.fill('\0');
    this->readSome(func);
}

void Socket::onWrite(const boost::system::error_code& err, size_t bytes_transferred,
                     const std::function<void(std::string)>& func)
{
    if (!err)
    {
        // std::cout << "onWrite New message sent => " << m_write_buff.data() << '\n';
        // func(m_write_buff.data());
    }
    else
    {
        std::cerr << "onWrite Error received => " << err.what() << '\n';
    }

    // m_write_buff.fill('\0');
    // this->writeSome(func);
}

}  // namespace sia::lts::detail
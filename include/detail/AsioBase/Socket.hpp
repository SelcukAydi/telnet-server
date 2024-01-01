#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <cstdint>
#include <functional>

namespace sia::lts::detail
{
class Socket : public boost::enable_shared_from_this<Socket>
{
    public:
    using socket_type = boost::asio::ip::tcp::socket;
    using endpoint_type = boost::asio::ip::tcp::endpoint;

    static constexpr std::int32_t kMaxReadBufferLength = 255;
    static constexpr std::int32_t kMaxWriteBufferLength = 255;

    // Expect only movable sockets.
    //
    explicit Socket(socket_type&& socket);
    
    socket_type& getSocket();

    void readSome(const std::function<void(std::string)>& func);
    void writeSome(const std::function<void(std::string)>& func, const std::string& data);

    private:
    void onRead(const boost::system::error_code& err, size_t bytes_transferred,
                const std::function<void(std::string)>& func);
    void onWrite(const boost::system::error_code& err, size_t bytes_transferred,
                 const std::function<void(std::string)>& func);

    socket_type m_socket;
    std::array<char, kMaxReadBufferLength> m_read_buff;
    std::array<char, kMaxWriteBufferLength> m_write_buff;
};

}  // namespace sia::lts::detail
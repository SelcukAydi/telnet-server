#include "detail/AsioBase/Connection.hpp"

namespace sia::lts::detail
{
Connection::Connection(std::int32_t conn_id, boost::shared_ptr<Socket> socket)
    : m_id(conn_id), m_socket(std::move(socket))
{
    m_socket->readSome([this](const std::string& data) { this->onRead(data); });
}

void Connection::onRead(const std::string& data) const
{
    if (m_callback_on_read)
    {
        m_callback_on_read(m_id, data);
    }
}

void Connection::onWrite(const std::string& data) const
{
    if (m_callback_on_write)
    {
        m_callback_on_write(m_id, data);
    }
}

void Connection::send(const std::string& data) const
{
    m_socket->writeSome([this](const std::string& data) { this->onWrite(data); }, data);
}

[[nodiscard]] std::int32_t Connection::getID() const
{
    return m_id;
}

void Connection::registerCallbackOnRead(const CallbackOnRead& func)
{
    m_callback_on_read = func;
}

void Connection::registerCallbackOnWrite(const CallbackOnWrite& func)
{
    m_callback_on_write = func;
}

}  // namespace sia::lts::detail
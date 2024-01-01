#pragma once

#include "detail/AsioBase/Socket.hpp"

namespace sia::lts::detail
{
class Connection
{
    public:
    Connection(std::int32_t conn_id, boost::shared_ptr<Socket> socket);

    void onRead(const std::string& data) const;
    void onWrite(const std::string& data) const;
    void send(const std::string& data) const;

    using CallbackOnRead = std::function<void(std::int32_t, std::string)>;
    using CallbackOnWrite = std::function<void(std::int32_t, std::string)>;

    [[nodiscard]] std::int32_t getID() const;

    void registerCallbackOnRead(const CallbackOnRead& func);
    void registerCallbackOnWrite(const CallbackOnWrite& func);

    private:
    const std::int32_t m_id{};
    boost::shared_ptr<Socket> m_socket;
    CallbackOnRead m_callback_on_read;
    CallbackOnWrite m_callback_on_write;
};
}  // namespace sia::lts::detail
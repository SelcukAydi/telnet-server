#pragma once

#include <cstdint>
#include <memory>

namespace sia::lts::detail
{
class TelnetServer;

class TelnetSession
{
    public:
    explicit TelnetSession(std::int32_t id, std::shared_ptr<TelnetServer> server);

    [[nodiscard]] std::int32_t getID() const;

    void send(const std::string& data);

    void processIncomingMsg(const std::string& data);

    private:
    std::weak_ptr<TelnetServer> m_server;
    const std::int32_t m_id;

    // NVT command executor.
    // Custom command executor. This must be generic.
};
}  // namespace sia::lts::detail
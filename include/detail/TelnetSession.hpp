#pragma once

#include "detail/ExecutorBase.hpp"
#include <cstdint>
#include <memory>

namespace sia::lts::detail
{
    struct TelnetSessionData
    {
        virtual ~TelnetSessionData() = default;
    };
}

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

    // NVT command executor.
    std::shared_ptr<ExecutorBase> m_nvt_executor;

    private:
    std::weak_ptr<TelnetServer> m_server;
    const std::int32_t m_id;

    
    // Custom command executor. This must be generic.
};
}  // namespace sia::lts::detail
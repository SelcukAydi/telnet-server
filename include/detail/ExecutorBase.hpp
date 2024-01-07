#pragma once

#include "detail/TelnetCommands.hpp"
#include <cstdint>
#include <memory>
#include <vector>

namespace sia::lts::detail
{
class TelnetSession;

struct ExecutorBase
{
    explicit ExecutorBase(const std::shared_ptr<TelnetSession> &session);
    virtual void execute(const std::shared_ptr<Command>& command) = 0;
    void send(const std::vector<std::uint8_t> &msg);
    void commit();
    virtual ~ExecutorBase() = default;
    [[nodiscard]] std::shared_ptr<TelnetSession> getSession() const;

    private:
    std::weak_ptr<TelnetSession> m_session;
    std::vector<std::string> m_to_be_sent_msgs;
};
}  // namespace sia::lts::detail
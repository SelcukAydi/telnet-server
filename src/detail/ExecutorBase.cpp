#include "detail/ExecutorBase.hpp"
#include "Helpers.hpp"
#include "detail/TelnetSession.hpp"
#include <sstream>

namespace sia::lts::detail
{
ExecutorBase::ExecutorBase(const std::shared_ptr<TelnetSession>& session) : m_session(session)
{
}

void ExecutorBase::send(const std::vector<std::uint8_t>& msg)
{
    if (!msg.empty())
        m_to_be_sent_msgs.push_back(helper::convertToString(msg));
}

void ExecutorBase::commit()
{
    std::stringstream ss;

    for(const auto& msg : m_to_be_sent_msgs)
    {
        ss << msg;
    }

    m_session.lock()->send(ss.str());
    m_to_be_sent_msgs.clear();
}

[[nodiscard]] std::shared_ptr<TelnetSession> ExecutorBase::getSession() const
{
    return m_session.lock();
}

}  // namespace sia::lts::detail
#include "detail/TelnetSession.hpp"
#include "detail/TelnetServer.hpp"
#include "detail/NVTCommandParser.hpp"
#include <memory>

namespace sia::lts::detail
{
TelnetSession::TelnetSession(std::int32_t id, std::shared_ptr<TelnetServer> server) : m_server(server), m_id(id)
{
    m_session_data = std::make_shared<TelnetSessionData>();
}

void TelnetSession::send(const std::string& data)
{
    m_server.lock()->send(m_id, data);
}

std::int32_t TelnetSession::getID() const
{
    return m_id;
}

[[nodiscard]] std::shared_ptr<TelnetSessionData> TelnetSession::getSessionData() const
{
    return m_session_data;
}

void TelnetSession::processIncomingMsg(const std::string& data)
{
    std::string_view msg(data);

    if (msg.empty())
    {
    }

    std::vector<std::uint8_t> bytes;
    for (const auto& item : msg)
    {
        bytes.push_back(item);
    }

    // Dummy send back response.
    //
    // std::vector<std::uint8_t> to_be_sent = {0xFF, 0xFB, 0x03, 0xFF, 0xFB, 0x05};
    // send(helper::convertToString(to_be_sent));

    std::uint8_t val = msg.at(0);

    if (val == 0xFF)
    {
        NVTCommandParser parser;
        auto commands = parser.parseCommands(bytes);

        if (commands.empty())
        {
        }

        // Let the executor handle the sequence of commands.
        //
        for (const auto& command : commands)
        {
            m_nvt_executor->execute(command);
        }

        m_nvt_executor->commit();
    }
}

}  // namespace sia::lts::detail
#include <cstdint>
#include <gtest/gtest.h>
#include <memory>
#include "ParserModules/DoParserModule.hpp"
#include "ParserModules/DontParserModule.hpp"
#include "ParserModules/EchoParserModule.hpp"
#include "ParserModules/LineModeParserModule.hpp"
#include "ParserModules/NewEnvOptionParserModule.hpp"
#include "ParserModules/RemoteFlowControlParserModule.hpp"
#include "ParserModules/StatusParserModule.hpp"
#include "ParserModules/SubOptionParserModule.hpp"
#include "ParserModules/SuppressGoAheadParserModule.hpp"
#include "ParserModules/TerminalSpeedParserModule.hpp"
#include "ParserModules/TerminalTypeParserModule.hpp"
#include "ParserModules/WillParserModule.hpp"
#include "ParserModules/WindowSizeParserModule.hpp"
#include "ParserModules/WontParserModule.hpp"
#include "ParserModules/XDisplayLocationParserModule.hpp"
#include "detail/NVTCommandParser.hpp"
#include "detail/TelnetServer.hpp"

bool operator==(const std::vector<std::uint8_t>& lhs, const std::vector<std::uint8_t>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (std::size_t i = 0; i < lhs.size(); ++i)
    {
        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }

    return true;
}

bool operator==(const std::vector<std::vector<std::uint8_t>>& lhs, const std::vector<std::vector<std::uint8_t>>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (std::size_t i = 0; i < lhs.size(); ++i)
    {
        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }

    return true;
}

TEST(Basic, First)
{
    std::vector<std::uint8_t> bytes = {0xFF, 0xFA, 0x1F, 0x00, 0xAD, 0x00, 0x2B, 0xFF, 0xF0};
    sia::lts::detail::NVTCommandParser parser;
    auto actual = parser.extractCommands(bytes);
    std::vector<std::vector<std::uint8_t>> expected = {{0xFA, 0x1F, 0x00, 0xAD, 0x00, 0x2B, 0xF0}};

    EXPECT_EQ(actual, expected);
}

TEST(Basic, Second)
{
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0xFA,
                                                                    std::make_shared<sia::lts::SubOptionParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x1F,
                                                                    std::make_shared<sia::lts::WindowSizeParser>());

    std::vector<std::uint8_t> bytes = {0xFF, 0xFA, 0x1F, 0x00, 0xAD, 0x00, 0x2B, 0xFF, 0xF0};
    sia::lts::detail::NVTCommandParser parser;
    auto actual = parser.parseCommands(bytes);
}

TEST(Basic, Echo)
{
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0xFD, std::make_shared<sia::lts::DoParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x01, std::make_shared<sia::lts::EchoParser>());

    std::vector<std::uint8_t> bytes = {0xFF, 0xFD, 0x01};
    sia::lts::detail::NVTCommandParser parser;
    auto commands = parser.parseCommands(bytes);
}

TEST(Basic, TerminalType)
{
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x18,
                                                                    std::make_shared<sia::lts::TerminalTypeParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0xFA,
                                                                    std::make_shared<sia::lts::SubOptionParser>());

    std::vector<std::uint8_t> bytes = {0xFF, 0xFA, 0x18, 0x00, 0x78, 0x74, 0x65, 0x72, 0x6d, 0x2d,
                                       0x32, 0x35, 0x36, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0xFF, 0xF0};
    sia::lts::detail::NVTCommandParser parser;
    auto commands = parser.parseCommands(bytes);
}

TEST(Basic, Testttt)
{
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0xFE, std::make_shared<sia::lts::DontParser>());

    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0xFD, std::make_shared<sia::lts::DoParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x01, std::make_shared<sia::lts::EchoParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x22, std::make_shared<sia::lts::LineModeParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x27,
                                                                    std::make_shared<sia::lts::NewEnvOptionParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(
        0x21, std::make_shared<sia::lts::RemoteFlowControlParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x05, std::make_shared<sia::lts::StatusParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0xFA,
                                                                    std::make_shared<sia::lts::SubOptionParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x03,
                                                                    std::make_shared<sia::lts::SupressGoAheadParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x18,
                                                                    std::make_shared<sia::lts::TerminalTypeParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x20,
                                                                    std::make_shared<sia::lts::TerminalSpeedParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0xFB, std::make_shared<sia::lts::WillParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0x1F,
                                                                    std::make_shared<sia::lts::WindowSizeParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(0xFC, std::make_shared<sia::lts::WontParser>());
    sia::lts::detail::NVTCommandParser::registerCommandParserModule(
        0x23, std::make_shared<sia::lts::XDisplayLocationParser>());

    auto server = std::make_shared<sia::lts::detail::TelnetServer>();
    server->start();
}
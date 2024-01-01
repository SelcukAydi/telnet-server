#include <cstdint>
#include <gtest/gtest.h>
#include <memory>
#include "ParserModules/DoParserModule.hpp"
#include "ParserModules/EchoParserModule.hpp"
#include "ParserModules/SubOptionParserModule.hpp"
#include "ParserModules/WindowSizeParserModule.hpp"
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

TEST(Basic, Testttt)
{
    auto  server = std::make_shared<sia::lts::detail::TelnetServer>();
    server->start();
}
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

namespace sia::lts::detail
{
struct Command
{
    Command(std::string name, std::uint8_t code) : m_name(std::move(name)), m_code(code)
    {
    }

    [[nodiscard]] std::string getName() const
    {
        return m_name;
    }

    [[nodiscard]] std::uint8_t getCode() const
    {
        return m_code;
    }

    private:
    const std::string m_name;
    const std::uint8_t m_code;
};

struct IAC : Command
{
    IAC() : Command("IAC", 0xFF)
    {
    }
};

struct GoAhead : Command
{
    GoAhead() : Command("", 0x00){};
};

struct Will : Command
{
    Will() : Command("WILL", 0xFB)
    {
    }

    std::shared_ptr<Command> m_sub_command;
};

struct Wont : Command
{
    Wont() : Command("WONT", 0xFC)
    {
    }

    std::shared_ptr<Command> m_sub_command;
};

struct Do : Command
{
    Do() : Command("DO", 0xFD)
    {
    }

    std::shared_ptr<Command> m_sub_command;
};

struct Dont : Command
{
    Dont() : Command("DONT", 0xFE)
    {
    }

    std::shared_ptr<Command> m_sub_command;
};

struct SuppressGoAhead : Command
{
    SuppressGoAhead() : Command("SuppressGoAhead", 0x03)
    {
    }
};

struct SB : Command
{
    SB() : Command("SB", 0xFA)
    {
    }

    std::shared_ptr<Command> m_sub_command;
};

struct Echo : Command
{
    Echo() : Command("ECHO", 0x01)
    {
    }
};

struct WindowSize : Command
{
    WindowSize(std::uint16_t width, std::uint16_t height) : Command("NAWS", 0x1F), m_width(width), m_height(height)
    {
    }

    const std::uint16_t m_width;
    const std::uint16_t m_height;
};

}  // namespace sia::lts::detail
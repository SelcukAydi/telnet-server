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

    virtual ~Command() = default;

    std::shared_ptr<Command> m_sub_command;

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
};

struct Wont : Command
{
    Wont() : Command("WONT", 0xFC)
    {
    }
};

struct Do : Command
{
    Do() : Command("DO", 0xFD)
    {
    }
};

struct Dont : Command
{
    Dont() : Command("DONT", 0xFE)
    {
    }
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

struct TerminalType : Command
{
    TerminalType() : Command("TerminalType", 0x18)
    {
    }

    std::string m_terminal_name;
};

struct TerminalSpeed : Command
{
    TerminalSpeed() : Command("TerminalSpeed", 0x20)
    {
    }

    std::uint64_t m_transmit_speed;
    std::uint64_t m_receive_speed;
};

struct LineMode : Command
{
    LineMode() : Command("LineMode", 0x22)
    {
    }
};

struct RemoteFlowControl : Command
{
    RemoteFlowControl() : Command("RemoteFlowControl", 0x21)
    {
    }
};

struct NewEnvOption : Command
{
    NewEnvOption() : Command("NewEnvOption", 0x27)
    {
    }
};

struct Status : Command
{
    Status() : Command("Status", 0x05)
    {
    }
};

struct XDisplayLocation : Command
{
    XDisplayLocation() : Command("XDisplayLocation", 0x23)
    {
    }
};

}  // namespace sia::lts::detail
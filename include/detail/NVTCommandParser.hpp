#pragma once

#include <cstdint>
#include <exception>
#include <sstream>
#include <unordered_map>
#include <memory>
#include <vector>
#include "detail/TelnetCommands.hpp"

namespace sia::lts::detail
{
    struct ParserNotRegisteredException : std::exception
    {
        explicit ParserNotRegisteredException(std::uint8_t code) : m_code(code)
        {
            std::stringstream ss;
            ss << "Command " << static_cast<std::uint32_t>(m_code) << " is not registered";
            m_error_msg = ss.str();
        }

        [[nodiscard]] const char* what() const noexcept override
        {
            return m_error_msg.c_str();
        }

        private:
        const std::uint8_t m_code;
        std::string m_error_msg;
    };

    struct CommandParserModuleBase
    {
        virtual std::shared_ptr<Command> parse(const std::vector<std::uint8_t>& command) = 0;
        virtual ~CommandParserModuleBase() = default;
    };

    class NVTCommandParser final
    {
        public:
        static void registerCommandParserModule(std::uint8_t parser_code, const std::shared_ptr<CommandParserModuleBase>& parser);

        std::vector<std::shared_ptr<Command>> parseCommands(const std::vector<std::uint8_t>& data);
        std::vector<std::vector<std::uint8_t>> extractCommands(const std::vector<std::uint8_t>& data);

        static std::shared_ptr<CommandParserModuleBase> findParser(std::uint8_t parser_code);

        private:
        using CommandParserModuleMap = std::unordered_map<std::uint8_t, std::shared_ptr<CommandParserModuleBase>>;
        static inline CommandParserModuleMap m_parser_map;
    };
}
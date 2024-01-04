#include "detail/NVTCommandParser.hpp"
#include <iostream>
#include <memory>
#include "detail/TelnetCommands.hpp"

namespace sia::lts::detail
{

void NVTCommandParser::registerCommandParserModule(std::uint8_t parser_code,
                                                   const std::shared_ptr<CommandParserModuleBase>& parser)
{
    m_parser_map.insert({parser_code, parser});
}

std::shared_ptr<CommandParserModuleBase> NVTCommandParser::findParser(std::uint8_t parser_code)
{
    auto itr = m_parser_map.find(parser_code);
    return itr == m_parser_map.end() ? nullptr : itr->second;
}

std::vector<std::vector<std::uint8_t>> NVTCommandParser::extractCommands(const std::vector<std::uint8_t>& data)
{
    enum State : std::uint8_t
    {
        kPendingIAC,
        kIAC,
        kSubOptionBegin
    };

    State current_state = State::kPendingIAC;
    std::vector<std::vector<std::uint8_t>> commands;
    std::vector<std::uint8_t> command;

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        std::uint8_t byte = data[i];

        switch (current_state)
        {
            case State::kPendingIAC:
            {
                if (byte == 0xFF)
                {
                    std::cout << "Transit from kPendingIAC to kIAC\n";
                    current_state = kIAC;
                }
                else
                {
                    std::cerr << "Throwing parse exception\n";
                }
            }
            break;

            case State::kIAC:
            {
                if (byte == 0xFA)
                {
                    std::cout << "Transit from kIAC to kSubOptionBegin\n";
                    current_state = kSubOptionBegin;
                    command.push_back(byte);
                }
                else if(byte != 0xFF)
                {
                    command.push_back(byte);
                    // commands.push_back(command);
                    // command.clear();

                    if(i == data.size() - 1)
                    {
                        std::cout << "Transit from kIAC to kPendingIAC\n";
                        current_state = kPendingIAC;
                        commands.push_back(command);
                    }
                }
            }
            break;

            case State::kSubOptionBegin:
            {
                if (byte == 0xFF)
                {
                    std::cout << "Transit from kSubOptionBegin to kSubOptionBegin\n";
                    current_state = kSubOptionBegin;
                    break;
                }

                if (byte == 0xF0)
                {
                    command.push_back(byte);
                    commands.push_back(command);
                    command.clear();
                    std::cout << "Transit from kSubOptionBegin to kPendingIAC\n";
                    current_state = kPendingIAC;
                }
                else
                {
                    command.push_back(byte);
                }
            }
            break;
        }
    }

    if (current_state != State::kPendingIAC)
    {
        // throw an exception here.
        //
    }

    return commands;
}

std::vector<std::shared_ptr<Command>> NVTCommandParser::parseCommands(const std::vector<std::uint8_t>& data)
{
    std::vector<std::shared_ptr<Command>> commands;
    auto command_list = extractCommands(data);

    for (const auto& command : command_list)
    {
        if (command.empty())
            continue;

        std::uint8_t byte = command[0];

        auto itr = m_parser_map.find(byte);
        if (itr == m_parser_map.end())
        {
            throw ParserNotRegisteredException(byte);
        }

        auto nvt_command = itr->second->parse(command);
        if(nvt_command == nullptr)
        {
            std::cerr << "Parser returned nullptr\n";
            continue;;
        }
        commands.push_back(nvt_command);
    }

    return commands;
}

}  // namespace sia::lts::detail
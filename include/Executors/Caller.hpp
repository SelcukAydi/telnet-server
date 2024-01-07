#pragma once

#include "detail/TelnetCommands.hpp"
#include <functional>

namespace sia::lts
{

using Bytes = std::vector<std::uint8_t>;

struct Caller
{
    using FuncT = std::function<Bytes(const std::shared_ptr<detail::Command>&)>;

    Caller(FuncT f_will, FuncT f_wont, FuncT f_do, FuncT f_dont, FuncT f_sb);

    Bytes Will(const std::shared_ptr<detail::Command>&);
    Bytes Wont(const std::shared_ptr<detail::Command>&);
    Bytes Do(const std::shared_ptr<detail::Command>&);
    Bytes Dont(const std::shared_ptr<detail::Command>&);
    Bytes SB(const std::shared_ptr<detail::Command>&);

    private:
    FuncT m_will;
    FuncT m_wont;
    FuncT m_do;
    FuncT m_dont;
    FuncT m_sb;
};
}  // namespace sia::lts
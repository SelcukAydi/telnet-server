#include "Executors/Caller.hpp"
#include <iostream>
#include <utility>

namespace sia::lts
{
using FuncT = Caller::FuncT;

Caller::Caller(FuncT f_will, FuncT f_wont, FuncT f_do, FuncT f_dont, FuncT f_sb)

    : m_will(std::move(f_will)), m_wont(std::move(f_wont)), m_do(std::move(f_do)), m_dont(std::move(f_dont)), m_sb(std::move(f_sb))
{
}

Bytes Caller::Will(const std::shared_ptr<detail::Command>& command)
{
    return m_will(command);
}
Bytes Caller::Wont(const std::shared_ptr<detail::Command>& command)
{
    return m_wont(command);
}
Bytes Caller::Do(const std::shared_ptr<detail::Command>& command)
{
    return m_do(command);
}
Bytes Caller::Dont(const std::shared_ptr<detail::Command>& command)
{
    return m_dont(command);
}

Bytes Caller::SB(const std::shared_ptr<detail::Command>& command)
{
    return m_sb(command);
}

}  // namespace sia::lts
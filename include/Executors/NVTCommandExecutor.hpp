#pragma once

namespace sia::lts::executor
{
class NVTCommandExecutor
{
    public:
    void processCommand();
    void handleDoCommand();
    void handleDontCommand();
    void handleWillCommand();
    void handleWontCommand();
};
}  // namespace sia::lts::executor
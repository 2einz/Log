
#include "log_inl.h"

int main() {
    auto logger = REIN_ROOT_LOGGER();
    // std::cout << "Root Logger's current level is: "
    //           << logger->level().string() << std::endl;

    // // 默认级别应该是 INFO 或 DEBUG，如果这里输出了 WARN 或更高，问题就找到了

    REIN_LOG_D("This is a debug message."); // 这条可能不会显示
    REIN_LOG_I("This is an info message.");

    REIN_LOG_W("This is a warning message.");
    return 0;

    }
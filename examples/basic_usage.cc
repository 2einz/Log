// 文件: rein_log/examples/basic_usage.cc

// 包含你的库的“唯一入口”头文件。
// 因为 CMake 已经帮我们设置好了包含路径，
// 我们可以直接使用从 'log/' 开始的路径。
#include <log/logging.h>

void some_function() {
    // 获取一个之前已经创建好的 logger
    auto db_logger = REIN_GET_LOGGER("database");
    if (db_logger) {
        REIN_LOG_DEBUG(db_logger, "Executing a query...");
    } else {
        // 如果 logger 不存在，打印一条 root logger 的错误
        REIN_LOG_E("Could not find 'database' logger!");
    }
}

int main() {
    // ---- 配置日志系统 ----
    // 在程序启动时，通过宏来配置你需要的 logger

    // 1. 添加一个名为 "database" 的 logger，它会把日志写入 "queries.log" 文件
    REIN_ADD_FILE_LOGGER("database", "queries.log");

    // 2. 添加一个名为 "network" 的 logger，它会输出到控制台
    REIN_ADD_CONSOLE_LOGGER("network");

    // ---- 使用日志系统 ----

    // 3. 使用默认的 root logger 打印一条信息
    // root logger 默认会输出到控制台
    REIN_LOG_I("Application initialized successfully. Loggers are configured.");

    // 4. 获取并使用 "network" logger
    auto net_logger = REIN_GET_LOGGER("network");
    if (net_logger) {
        // 这条日志只会出现在控制台
        REIN_LOG_INFO(net_logger, "Connection to server established.");
    }

    // 5. 在其他函数中使用 logger
    some_function();

    // 6. 使用一条 FATAL 级别的 root logger 日志
    // 注意：FATAL 日志通常意味着严重错误，但在这里只是示例
    REIN_LOG_F("Application is shutting down.");

    return 0;
}
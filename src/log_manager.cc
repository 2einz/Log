#include "log/log_manager.h"

#include "log/appender.h"
#include "log/logger.h"
#include "log/layout.h"

namespace rein {
namespace log {

LogManager& LogManager::instance() {
    static LogManager manager_instance;
    return manager_instance;
}

// 私有构造函数，在 instance() 第一次被调用时执行一次
LogManager::LogManager() {
    // 注意：由于 Logger 的构造函数是私有的，LogManager 作为其友元类，
    // 不能直接使用 std::make_shared<Logger>(...)。
    // 我们必须直接调用 new，然后将其包装在 shared_ptr 中。
    root_logger_ = std::shared_ptr<Logger>(new Logger("root"));

    // 为了让日志系统“开箱即用”，我们为 root logger 添加一个默认的 Appender。
    // 最常见的默认设置是输出到控制台。
    // 如果不添加任何 Appender，root logger 将不会输出任何内容。
    auto console_appender = std::make_shared<ConsoleAppender>();
    console_appender->SetLayout(std::make_shared<Layout>());
    root_logger_->add_appender(console_appender);
}

// 获取 root logger
std::shared_ptr<Logger> LogManager::get_root_logger() {
    // root_logger_ 在构造时就已经创建，之后是只读的，所以直接返回是线程安全的。
    return root_logger_;
}

// 根据名称获取 logger
std::shared_ptr<Logger> LogManager::get_logger(const std::string& name) {
    if (name == "root") {
        return root_logger_;
    }

    // loggers_ map 是共享资源，必须加锁保护以防止多线程同时读写导致的数据竞争。
    std::lock_guard<std::mutex> lock(mutex_);

    // 1. 查找是否已存在同名 logger
    auto it = loggers_.find(name);
    if (it != loggers_.end()) {
        return it->second;
    }

    std::shared_ptr<Logger> new_logger(new Logger(name));

    // 将新创建的 logger 存入 map 中
    loggers_[name] = new_logger;

    // 返回新创建的 logger
    return new_logger;
}

}  // namespace log
}  // namespace rein
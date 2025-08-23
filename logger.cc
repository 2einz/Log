#include "logger.h"

#include <algorithm>

namespace rein {
namespace log {
Logger::Logger(std::string name, Level level)
    : name_(std::move(name)),
      level_(level) {}

void Logger::log(
    Level level, const char* file, uint32_t line, const char* func, const std::string& message) {
    // 创建 LogEvent 对象
    // 使用 shared_from_this() 来获取当前 Logger 对象的 shared_ptr
    auto event = std::make_shared<LogEvent>(level, file, line, func, message, shared_from_this());

    // 使用互斥锁保护 appenders_ 列表，因为其他线程可能正在添加或删除 appender
    std::lock_guard<std::mutex> lock(mutex_);
    if (!appenders_.empty()) {
        for (auto& appender : appenders_) {
            // 将事件分发给每个 appender
            // Appender 内部会再进行一次级别检查
            appender->Log(event, appender->level());
        }
    }
}

void Logger::add_appender(std::shared_ptr<Appender> appender) {
    std::lock_guard<std::mutex> lock(mutex_);
    // 检查是否已存在
    auto it = std::find(appenders_.begin(), appenders_.end(), appender);
    if (it == appenders_.end()) {
        appenders_.push_back(appender);
    }
}


void Logger::remove_appender(std::shared_ptr<Appender> appender) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find(appenders_.begin(), appenders_.end(), appender);
    if (it != appenders_.end()) {
        appenders_.erase(it);
    }
}

void Logger::clear_appenders() {
    std::lock_guard<std::mutex> lock(mutex_);
    appenders_.clear();
}

void Logger::set_level(Level level) {
    std::lock_guard<std::mutex> lock(mutex_);
    level_ = level;
}

Level Logger::level() const {
    // 读取 level 也需要加锁，以保证可见性
    std::lock_guard<std::mutex> lock(mutex_);
    return level_;
}

const std::string& Logger::name() const {
    return name_;
}

}  // namespace log
}  // namespace rein
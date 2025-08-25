#include "log/logger.h"

#include <algorithm>
#include <exception>
#include <memory>
#include <mutex>
#include <stdexcept>
#include "fmt/base.h"
#include "log/appender.h"

namespace rein {
namespace log {
Logger::Logger(const std::string& name, Level level)
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

void Logger::AddAppender(AppenderType type, const std::string& out) {
    try {
        auto app = AppenderFactory::CreateAppender(type, out);

        AddAppender(app);

    } catch (const std::exception& e) {
        fmt::println("Cannot add appender '{}' to logger {}: {}",
                     AppenderFactory::TypeToString(type), name_, e.what());
    }
}

void Logger::AddAppender(std::shared_ptr<Appender> appender) {
    if (!appender) {
        throw std::invalid_argument("Cannot add null Appender to logger");
    }
    std::lock_guard<std::mutex> lock(mutex_);
    // 检查是否已存在
    auto it = std::find(appenders_.begin(), appenders_.end(), appender);
    if (it == appenders_.end()) {
        appenders_.push_back(appender);
    } else {
        throw std::runtime_error(
            fmt::format("Appender '{}' already exists in logger", appender->type_str()));
    }
}

void Logger::RemoveAppender(AppenderType type, const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);

    for (auto it = appenders_.begin(); it != appenders_.end(); it++) {
        if ((*it)->type() == type && (*it)->name() == name) {
            it = appenders_.erase(it);
            return;
        }
    }
}

void Logger::RemoveAppender(AppenderType type) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto new_end = std::remove_if(appenders_.begin(), appenders_.end(),
                                  [&](const std::shared_ptr<Appender>& appender) {
                                      if (appender->type() == type) {
                                          return true;
                                      }
                                      return false;
                                  });

    appenders_.erase(new_end, appenders_.end());
}

void Logger::RemoveAppender(std::shared_ptr<Appender> appender) {
    if (!appender) {
        throw std::invalid_argument("Cannot del null Appender from logger");
    }
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find(appenders_.begin(), appenders_.end(), appender);
    if (it != appenders_.end()) {
        appenders_.erase(it);
    }
}

void Logger::ClearAppenders() {
    std::lock_guard<std::mutex> lock(mutex_);
    appenders_.clear();
}

void Logger::SetLevel(Level level) {
    std::lock_guard<std::mutex> lock(mutex_);
    level_ = level;
}

std::shared_ptr<Appender> Logger::appender(AppenderType type, const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto it : appenders_) {
        if (it->name() == name && it->type() == type) {
            return it;
        }
    }
    return nullptr;
}

Level Logger::level() const {
    // 读取 level 也需要加锁，以保证可见性
    std::lock_guard<std::mutex> lock(mutex_);
    return level_;
}

const std::string& Logger::name() const { return name_; }

}  // namespace log
}  // namespace rein
#include "log/log_manager.h"
#include <cstdio>
#include <exception>
#include <stdexcept>

#include "fmt/base.h"
#include "fmt/format.h"
#include "log/appender.h"
#include "log/log_constants.h"
#include "log/logger.h"
#include "log/util.hpp"

namespace rein {
namespace log {

LogManager& LogManager::instance() {
    static LogManager manager_instance;
    return manager_instance;
}

LogManager::LogManager() {
    // 注意：由于 Logger 的构造函数是私有的，LogManager 作为其友元类，
    // 不能直接使用 std::make_shared<Logger>(...)。
    // 我们必须直接调用 new，然后将其包装在 shared_ptr 中。
    root_logger_ = std::shared_ptr<Logger>(new Logger(kRootLoggerName));

    root_logger_->AddAppender(AppenderType::CONSOLE);

    loggers_[kRootLoggerName] = root_logger_;
}

// 获取 root logger
std::shared_ptr<Logger> LogManager::root_logger() { return root_logger_; }

// 根据名称获取 logger
std::shared_ptr<Logger> LogManager::logger(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = loggers_.find(name);

    return (it != loggers_.end()) ? it->second : nullptr;
}

void LogManager::AddLogger(const std::string& logger_name) {
    AddLogger(logger_name, AppenderType::CONSOLE, kConsole);
}

void LogManager::AddLogger(const std::string& logger_name,
                           AppenderType appender_type,
                           const std::string& out,
                           const std::string& layout) {
    if (logger_name == kRootLoggerName) {
        throw std::invalid_argument("Cannot add logger with reserved name 'root'.");
    }

    std::lock_guard<std::mutex> lock(mutex_);

    if (loggers_.count(logger_name) != 0) {
        throw std::invalid_argument(fmt::format("Logger '{}' already exists", logger_name));
    }

    auto new_logger = std::shared_ptr<Logger>(new Logger(logger_name));

    std::shared_ptr<Appender> appender = AppenderFactory::CreateAppender(appender_type, out);

    if (!appender) {
        throw std::runtime_error(fmt::format("Failed to create appender (type: '{}', out: '{}')",
                                             AppenderFactory::TypeToString(appender_type), out));
    }
    appender->SetLayout(layout);
    new_logger->AddAppender(appender);
    loggers_[logger_name] = new_logger;
}

void LogManager::RemoveLogger(std::shared_ptr<Logger> logger) {
    if (!logger || logger->name() == kRootLoggerName) {
        // 不允许移除 root logger 或 nullptr
        return;
    }

    RemoveLogger(logger->name());
}

void LogManager::RemoveLogger(const std::string& logger_name) {
    if (logger_name == kRootLoggerName) {
        // throw std::invalid_argument("Root logger cannot be removed.");
        fmt::println(stderr, "Warning: Attempted to remove the root logger, which is not allowed.");
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    if (loggers_.erase(logger_name) == 0) {
        // 如果 erase 返回 0，说明没有元素被删除，即 logger 不存在
        fmt::println(stderr, "Warning: Logger '{}' not found for removal.", logger_name);
    }
}

void LogManager::AddAppender(std::shared_ptr<Logger> logger,
                             AppenderType type,
                             const std::string& appender_out) {
    if (!logger) {
        throw std::invalid_argument("Cannot add appender to a null logger.");
    }

    auto appender = AppenderFactory::CreateAppender(type, appender_out);

    if (!appender) {
        throw std::runtime_error(fmt::format("Failed to create appender (type: '{}', out: '{}')",
                                             AppenderFactory::TypeToString(type), appender_out));
    }

    logger->AddAppender(appender);
}

void LogManager::AddAppender(const std::string& logger_name,
                             AppenderType appender_type,
                             const std::string& appender_out) {
    auto target_logger = logger(logger_name);

    if (!target_logger) {
        fmt::println(stderr, "Error: Cannot add appender, logger '{}' not found.", logger_name);
        return;
    }

    AddAppender(target_logger, appender_type, appender_out);
}

void LogManager::RemoveAppender(const std::string& logger_name,
                                AppenderType appender_type,
                                const std::string& appender_out) {
    auto target_logger = logger(logger_name);

    if (!target_logger) {
        fmt::println(stderr, "Error: Cannot remove appender, logger '{}' not found.", logger_name);
        return;
    }

    target_logger->RemoveAppender(appender_type, appender_out);
}

void LogManager::SetLayout(const std::string& logger_name,
                           AppenderType appender_type,
                           const std::string& appender_out,
                           const std::string& layout) {
    auto target_logger = logger(logger_name);

    if (!target_logger) {
        fmt::println(stderr, "Error: Cannot set layout, logger '{}' not found.", logger_name);
        return;
    }

    auto target_appender = target_logger->appender(appender_type, appender_out);

    if (!target_appender) {
        fmt::println(
            stderr,
            "Error: Cannot set layout, appender (type: '{}', out: '{}') not found in logger '{}'.",
            AppenderFactory::TypeToString(appender_type), appender_out, logger_name);
        return;
    }

    target_appender->SetLayout(layout);
}

}  // namespace log
}  // namespace rein
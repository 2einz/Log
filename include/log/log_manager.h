#ifndef REIN_LOG_MANAGER_H_
#define REIN_LOG_MANAGER_H_

#include <memory>
#include <map>
#include <mutex>
#include <string>

#include <fmt/core.h>

#include "appender.h"
#include "log_constants.h"

namespace rein {
namespace log {

class Logger;

class LogManager {
public:
    static LogManager& instance();

    std::shared_ptr<Logger> logger(const std::string& name);
    std::shared_ptr<Logger> root_logger();

    // 添加一个控制台logger
    void AddLogger(const std::string& logger_name = kConsole);

    void AddLogger(const std::string& logger_name,
                   AppenderType appender_type,
                   const std::string& appender_out = kConsole,
                   const std::string& layout = kDefaultLayout);

    void RemoveLogger(const std::string& logger_name);

    void RemoveLogger(std::shared_ptr<Logger> logger);

    /**
     * @brief 通过指针添加Appender
     */
    void AddAppender(std::shared_ptr<Logger> Logger,
                     AppenderType type,
                     const std::string& appender_out);

    void AddAppender(const std::string& logger_name,
                     AppenderType appender_type,
                     const std::string& appender_out);

    void RemoveAppender(const std::string& logger_name,
                        AppenderType appender,
                        const std::string& appender_out = kConsole);

    void SetLayout(const std::string& logger_name,
                   AppenderType appender_type,
                   const std::string& appender_out = kConsole,
                   const std::string& layout = kDefaultLayout);

private:
    LogManager();  // 私有构造

    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    std::map<std::string, std::shared_ptr<Logger>> loggers_;
    std::shared_ptr<Logger> root_logger_;
    std::mutex mutex_;
};
}  // namespace log
}  // namespace rein
#endif
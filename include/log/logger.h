#ifndef REIN_LOG_LOGGER_H_
#define REIN_LOG_LOGGER_H_

#include <memory>
#include <string>
#include <vector>
#include "appender.h"
#include "event.h"
namespace rein {
namespace log {

class LogEvent;
class Appender;
class LogManager;
class Logger : public std::enable_shared_from_this<Logger> {
    friend class LogManager;

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    template <typename... Args>
    void debug(
        const char* file, uint32_t line, const char* func, const std::string& fmt, Args&&... args);

    template <typename... Args>
    void info(
        const char* file, uint32_t line, const char* func, const std::string& fmt, Args&&... args);

    template <typename... Args>
    void warn(
        const char* file, uint32_t line, const char* func, const std::string& fmt, Args&&... args);

    template <typename... Args>
    void error(
        const char* file, uint32_t line, const char* func, const std::string& fmt, Args&&... args);

    template <typename... Args>
    void fatal(
        const char* file, uint32_t line, const char* func, const std::string& fmt, Args&&... args);

    void AddAppender(std::shared_ptr<Appender> appender);
    void AddAppender(AppenderType type, const std::string& out = kConsole);

    void RemoveAppender(AppenderType type, const std::string& name);
    void RemoveAppender(AppenderType type);
    void RemoveAppender(std::shared_ptr<Appender> appender);

    void ClearAppenders();

    std::shared_ptr<Appender> appender(AppenderType type, const std::string& name = kConsole);

    void SetLevel(Level level);

    Level level() const;
    const std::string& name() const;

private:
    // 私有构造函数，强制通过 LogManager 创建
    explicit Logger(const std::string& name, Level level = Level(LevelType::kDebug));

    void log(
        Level level, const char* file, uint32_t line, const char* func, const std::string& message);

private:
    std::string name_;
    mutable std::mutex mutex_;
    Level level_;
    std::vector<std::shared_ptr<Appender>> appenders_;
};

template <typename... Args>
void Logger::debug(
    const char* file, uint32_t line, const char* func, const std::string& fmt, Args&&... args) {
    if (level_.cmp(LevelType::kDebug)) {
        log(Level(LevelType::kDebug), file, line, func,
            fmt::format(fmt, std::forward<Args>(args)...));
    }
}

template <typename... Args>
void Logger::info(
    const char* file, uint32_t line, const char* func, const std::string& fmt, Args&&... args) {
    if (level_.cmp(LevelType::kInfo)) {
        log(Level(LevelType::kInfo), file, line, func,
            fmt::format(fmt, std::forward<Args>(args)...));
    }
}

template <typename... Args>
void Logger::warn(
    const char* file, uint32_t line, const char* func, const std::string& fmt, Args&&... args) {
    if (level_.cmp(LevelType::kWarn)) {
        log(Level(LevelType::kWarn), file, line, func,
            fmt::format(fmt, std::forward<Args>(args)...));
    }
}

template <typename... Args>
void Logger::error(
    const char* file, uint32_t line, const char* func, const std::string& fmt, Args&&... args) {
    if (level_.cmp(LevelType::kError)) {
        log(Level(LevelType::kError), file, line, func,
            fmt::format(fmt, std::forward<Args>(args)...));
    }
}

template <typename... Args>
void Logger::fatal(
    const char* file, uint32_t line, const char* func, const std::string& fmt, Args&&... args) {
    if (level_.cmp(LevelType::kFatal)) {
        log(Level(LevelType::kFatal), file, line, func,
            fmt::format(fmt, std::forward<Args>(args)...));
    }
}

}  // namespace log
}  // namespace rein


#endif // REIN_LOG_LOGGER_H_
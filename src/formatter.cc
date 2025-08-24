#include "log/formatter.h"

#include <ctime>
#include <iterator>  // for std::back_inserter
#include <chrono>

#include <fmt/chrono.h>

#include "fmt/base.h"
#include "fmt/format.h"
#include "log/logger.h"

namespace rein {
namespace log {

void NameFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "{}", event->logger()->name());
}

DateTimeFormatter::DateTimeFormatter(const std::string &format)
    : format_(std::move(format)) {}

void DateTimeFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    auto timestamp = event->timestamp();
    std::time_t time = std::chrono::system_clock::to_time_t(timestamp);  // 转换秒级时间
    std::tm local_tm;
    localtime_r(&time, &local_tm);

    // 计算毫秒
    auto tp_sec = std::chrono::system_clock::from_time_t(time);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp - tp_sec).count();

    size_t dot_f_pos = format_.find(".%f");
    if (dot_f_pos == std::string::npos) {
        // 没有找到毫秒占位符，直接格式化
        fmt::format_to(std::back_inserter(buffer), fmt::runtime("{" + format_ + "}"), local_tm);
        return;
    }

    // 如果有.%f，就使用字符串
    std::string prefix_format = "{" + format_.substr(0, dot_f_pos) + "}";
    std::string time_str = fmt::format(fmt::runtime(prefix_format), local_tm);

    fmt::format_to(std::back_inserter(buffer), "{}{}", time_str, fmt::format(".{:03}", ms));
}

void FileNameFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "{}", event->file());
}

void LineFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "{}", event->line());
}

void MessageFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "{}", event->message());
}

void NewLineFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "\n");
}

void ThreadIdFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "{}", event->pthread_id());
}

void ThreadNameFormatter::format(fmt::memory_buffer &buffer,
                                 const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "{}", event->thread_name());
}

void LevelFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "{}", event->level().string());
}

void ElapseFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "{}", event->elapse());
}

void TabFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "\t");
}

StringFormatter::StringFormatter(const std::string &str)
    : str_(std::move(str)) {}

void StringFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "{}", str_);
}
}  // namespace log
}  // namespace rein
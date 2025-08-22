#include "layout.h"

#include <chrono>
#include <cstring>
#include <ctime>
#include <iterator>  // for std::back_inserter

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/format.h>

namespace rein {
namespace log {

// 模板工厂函数：创建无参数的FormatterItem
template <typename Type>
Layout::FormatterFactory make_factory() {
    return []() { return std::make_shared<Type>(); };
}

// 模板工厂函数：创建带参数的FormatterItem
template <typename Type>
Layout::FormatterFactory make_factory(const std::string &param) {
    return [param]() { return std::make_shared<Type>(param); };
}

const std::map<std::string, Layout::FormatterFactory> Layout::formatters_ = {
    {"c", make_factory<NameFormatter>()},
    {"d", make_factory<DateTimeFormatter>("")},  // 实际参数在解析时提取
    {"f", make_factory<FileNameFormatter>()},
    {"l", make_factory<LineFormatter>()},
    {"m", make_factory<MessageFormatter>()},
    {"n", make_factory<NewLineFormatter>()},
    {"N", make_factory<ThreadNameFormatter>()},
    {"p", make_factory<LevelFormatter>()},
    {"r", make_factory<ElapseFormatter>()},
    {"T", make_factory<TabFormatter>()},
    {"s", make_factory<StringFormatter>("")},
    {"t", make_factory<ThreadIdFormatter>()}};

Layout::Layout(const std::string &pattern)
    : pattern_(std::move(pattern)) {
    parse_pattern();
}

std::string Layout::format(const std::shared_ptr<LogEvent> &event, Level level) {
    fmt::memory_buffer buffer;
    for (const auto &item : items_) {
        item->format(buffer, event);
    }
    return fmt::to_string(buffer);
}

void NameFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    fmt::format_to(std::back_inserter(buffer), "{}", event->logger()->name());
}

DateTimeFormatter::DateTimeFormatter(const std::string &format)
    : format_(std::move(format)) {}

void DateTimeFormatter::format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) {
    // 转换为秒级时间
    std::time_t time = std::chrono::system_clock::to_time_t(event->timestamp());

    // 转换为本地时间
    std::tm local_tm;
    std::memset(&local_tm, 0, sizeof(local_tm));

    // linux系统使用
    localtime_r(&time, &local_tm);

    // 计算毫秒部分（0-999）
    auto tp_sec = std::chrono::system_clock::from_time_t(time);  // 秒级时间点
    auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(event->timestamp() - tp_sec).count();

    // 先处理基础时间部分（年月日时分秒）
    std::string time_str;

    fmt::format_to(std::back_inserter(time_str), fmt::runtime(format_), local_tm);

    // 处理毫秒（如果格式中包含%f）
    if (format_.find("%f") != std::string::npos) {
        // 替换%f为实际毫秒值
        fmt::format_to(std::back_inserter(time_str), ".{:03d}", ms);
    }

    fmt::format_to(std::back_inserter(buffer), "{}", time_str);
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
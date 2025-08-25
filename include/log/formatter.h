#ifndef REIN_LOG_FORMATTERS_H_
#define REIN_LOG_FORMATTERS_H_
#include "layout.h"

namespace rein {
namespace log {

// 日志器名称
class NameFormatter final : public Layout::FormatterItem {
public:
    explicit NameFormatter() = default;

    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;
};

/**
时间戳
直接使用fmt原生占位符支持
%Y：四位年份（2024）
%m：两位月份（08）
%d：两位日期（22）
%H：24 小时制小时（19）
%M：分钟（45）
%S：秒（30）
其他占位符：%A（星期全名）、%b（月份缩写）
新增：%f 毫秒数
*/
class DateTimeFormatter final : public Layout::FormatterItem {
public:
    explicit DateTimeFormatter(const std::string &format = kDefaultDateTimeParam);

    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;

private:
    std::string format_;
};

class FileNameFormatter final : public Layout::FormatterItem {
public:
    explicit FileNameFormatter() = default;

    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;

private:
    std::string name_;
};

// 行数
class LineFormatter final : public Layout::FormatterItem {
public:
    explicit LineFormatter() = default;
    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;
};

// 内容
class MessageFormatter final : public Layout::FormatterItem {
public:
    explicit MessageFormatter() = default;
    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;
};

// 换行
class NewLineFormatter final : public Layout::FormatterItem {
public:
    explicit NewLineFormatter() = default;
    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;
};

// 线程id
class ThreadIdFormatter final : public Layout::FormatterItem {
public:
    explicit ThreadIdFormatter() = default;
    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;
};

class ThreadNameFormatter final : public Layout::FormatterItem {
public:
    explicit ThreadNameFormatter() = default;
    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;
};

class LevelFormatter final : public Layout::FormatterItem {
public:
    explicit LevelFormatter() = default;
    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;
};

class ElapseFormatter final : public Layout::FormatterItem {
public:
    explicit ElapseFormatter() = default;
    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;
};

class TabFormatter final : public Layout::FormatterItem {
public:
    explicit TabFormatter() = default;
    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;
};

// 字符串 param
class StringFormatter final : public Layout::FormatterItem {
public:
    explicit StringFormatter(const std::string &str = "");
    void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) override;

private:
    std::string str_;
};
}  // namespace log
}  // namespace rein

#endif
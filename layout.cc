#include "layout.h"

#include <cstddef>
#include <cstring>
#include <ctime>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include "event.h"
#include "formatter.h"

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

std::string Layout::format(const std::shared_ptr<LogEvent> &event) {
    return format(event, Level());
}

std::string Layout::format(const std::shared_ptr<LogEvent> &event, Level level) {
    if (!event->level().cmp(level)) {
        return "";
    }
    fmt::memory_buffer buffer;
    for (const auto &item : items_) {
        item->format(buffer, event);
    }
    return fmt::to_string(buffer);
}

// %d{%Y-%m-%d %H:%M%S.%f} [%p] %f:%l%m%n
void Layout::parse_pattern() {
    std::string buffer;

    for (int i = 0; i < pattern_.size(); ++i) {
        // 处理普通字符串
        if (pattern_[i] != '%') {
            buffer.append(1, pattern_[i]);
            continue;
        }

        // 处理转义字符
        if ((i + 1) < pattern_.size() && pattern_[i + 1] == '%') {
            buffer.append(1, '%');
            continue;
        }

        // 处理格式说明
        if (!buffer.empty()) {
            items_.push_back(std::make_shared<StringFormatter>(buffer));
            buffer.clear();
        }

        i++;  // 移动到%后面
        if (i >= pattern_.size()) {
            throw std::logic_error("Invalid pattern: pattern ends with a single '%'");
        }

        std::string specifier_str(1, pattern_[i]);
        std::string param;

        // 检查是否携带参数 {...}
        if ((i + 1) < pattern_.size() && pattern_[i + 1] == '{') {
            i += 2;  // 跳过{
            size_t end_brace_pos = pattern_.find('}', i);
            if (end_brace_pos == std::string::npos) {
                throw std::logic_error("Invalid pattern: missing '}' in specifier");
            }
            param = pattern_.substr(i, end_brace_pos - i);  // 正确计算长度
            i = end_brace_pos;
        }

        auto it = formatters_.find(specifier_str);
        if (it == formatters_.end()) {
            std::string error_msg = "Invalid pattern: unknown format specifier - " + specifier_str;
            throw std::logic_error(error_msg);
        }

        if (!param.empty()) {
            // 特殊处理带有参数的类型
            if (specifier_str == "d") {
                items_.push_back(std::make_shared<DateTimeFormatter>(param));
            } else if (specifier_str == "s") {
                items_.push_back(std::make_shared<StringFormatter>(param));
            } else {
                items_.push_back(it->second());
            }
        } else {
            items_.push_back(it->second());
        }

        // 处理最后的普通字符串
        if (!buffer.empty()) {
            items_.push_back(std::make_shared<StringFormatter>(buffer));
        }
    }

    if (!buffer.empty()) {
    items_.push_back(std::make_shared<StringFormatter>(buffer));
}
}

}  // namespace log
}  // namespace rein
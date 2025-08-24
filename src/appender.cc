#include "log/appender.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <memory>
#include <mutex>

#include "log/color.h"
#include "log/event.h"
#include "log/layout.h"
#include "log/level.h"
namespace rein {
namespace log {

void Appender::SetLayout(std::shared_ptr<Layout> layout) {
    std::lock_guard<std::mutex> lock(mutex_);
    layout_ = layout;
}

template <typename T>
void Appender::SetLevel(T level) {
    std::lock_guard<std::mutex> lock(mutex_);
    level_.SetLevel(level);
}

Level Appender::level() const { return level_; }

std::shared_ptr<Layout> Appender::layout() const { return layout_; }

FileAppender::FileAppender(std::string name)
    : file_name_(name) {
    if (!Open()) {
        fmt::println("FileAppender failed!");
    }
}
FileAppender::~FileAppender() { Close(); }

bool FileAppender::Log(const std::shared_ptr<LogEvent> event, Level level) {
    if (!IsOpen()) {
        fmt::println("File is not opened! Please open file...");
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        fmt::print(file_, layout_->format(event, level));
    }

    return true;
}

bool FileAppender::Open() {
    // std::lock_guard<std::mutex> lock(mutex_);
    // 使用的两个地方：构造（不需要加锁），Reopen 已经加锁
    file_ = std::fopen(file_name_.c_str(), "a+");  // 追加+创建文件

    if (!file_) {
        fmt::println(stderr, "Could not open or create file '{}', error: {}", file_name_,
                     std::strerror(errno));
        return false;
    }
    return true;
}

bool FileAppender::Close() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!file_) {
        return true;
    }
    std::fclose(file_);
    file_ = nullptr;
    return true;
}

bool FileAppender::ReOpen() {
    std::lock_guard<std::mutex> lock(mutex_);
    Close();
    return Open();
}

bool FileAppender::IsOpen() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return file_ != nullptr;
}

bool ConsoleAppender::Log(const std::shared_ptr<LogEvent> event, Level level) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (event->level().cmp(level_)) {
        fmt::print(stdout, "{}{}{}",
                   rein::color::get_level_color(event->level()),  // 级别对应颜色
                   layout_->format(event),                  // 日志内容
                   rein::color::Color::RESET                      // 重置颜色
        );
    }
    return true;
}

}  // namespace log
}  // namespace rein
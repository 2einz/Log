#include "appender.h"

#include <cerrno>
#include <cstdio>
#include <memory>
#include <mutex>
#include <system_error>
#include <fstream>

#include "layout.h"
#include "level.h"
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

Level Appender::GetLevel() const { return level_; }

std::shared_ptr<Layout> Appender::GetLayout() const { return layout_; }

FileAppender::FileAppender(std::string name)
    : file_name_(name) {
    if (!Open()) {
        fmt::println("FileAppender failed!");
    }
}
FileAppender::~FileAppender() { Close(); }

bool FileAppender::Log(LogEvent) {
    if (!IsOpen()) {
        fmt::println("File is not opened! Please open file...");
        return false;
    }




    return true;
}

bool FileAppender::Open() {
    file_ = std::fopen(file_name_.c_str(), "a+");  // 追加+创建文件

    if (!file_) {
        fmt::println(stderr, "Could not open or create file '{}', error: {}", file_name_,
                     std::errc(errno));
        return false;
    }
    return true;
}

bool FileAppender::Close() {
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

}  // namespace log
}  // namespace rein
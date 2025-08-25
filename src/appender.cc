#include "log/appender.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <memory>
#include <mutex>
#include <stdexcept>

#include "fmt/base.h"
#include "log/color.h"
#include "log/event.h"
#include "log/layout.h"
#include "log/level.h"

namespace rein {
namespace log {

Appender::Appender(AppenderType type, const std::string& name)
    : type_(type),
      name_(name),
      level_(LevelType::kDebug) {}

void Appender::SetLayout(std::shared_ptr<Layout> layout) {
    std::lock_guard<std::mutex> lock(mutex_);
    layout_ = layout;
}

void Appender::SetLayout(const std::string& param) {
    layout_ = param.empty() ? std::make_shared<Layout>() : std::make_shared<Layout>(param);
}

void Appender::SetLayout() { layout_ = std::make_shared<Layout>(); }

void Appender::SetName(const std::string& name) { name_ = name; }

const std::string& Appender::name() const { return name_; }

Level Appender::level() const { return level_; }

AppenderType Appender::type() const { return type_; };

std::string Appender::type_str() const { return AppenderFactory::TypeToString(type_); }
std::shared_ptr<Layout> Appender::layout() const { return layout_; }

FileAppender::FileAppender(const std::string& name)
    : Appender(AppenderType::FILE, name),  // 调用基类构造，初始化 type_ 为 FILE
      file_name_(name) {
    SetLayout();
    Open();
    if (!IsOpen()) {
        fmt::println("FileAppender construction failed!");
    }
}
FileAppender::~FileAppender() { Close(); }

void FileAppender::Log(const std::shared_ptr<LogEvent> event, Level level) {
    if (!IsOpen()) {
        fmt::println("File is not opened! Please open file...");
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    fmt::print(file_, layout_->format(event));
}

void FileAppender::Open() {
    // std::lock_guard<std::mutex> lock(mutex_);
    // 使用的两个地方：构造（不需要加锁），Reopen 已经加锁
    file_ = std::fopen(file_name_.c_str(), "a+");  // 追加+创建文件

    if (!file_) {
        // fmt::println(stderr, "Could not open or create file '{}', error: {}", file_name_,
        //              std::strerror(errno));

        throw std::runtime_error(fmt::format("Could not open or create file '{}', error: {}",
                                             file_name_, std::strerror(errno)));
    }
}

void FileAppender::Close() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!file_) {
        return;
    }
    std::fclose(file_);
    file_ = nullptr;
}

void FileAppender::ReOpen() {
    std::lock_guard<std::mutex> lock(mutex_);
    Close();
    Open();
}

bool FileAppender::IsOpen() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return file_ != nullptr;
}

ConsoleAppender::ConsoleAppender()
    : Appender(AppenderType::CONSOLE, kConsole) {
    SetLayout();
}

ConsoleAppender::ConsoleAppender(const std::string& name)
    : Appender(AppenderType::CONSOLE, name) {
    SetLayout();
}

void ConsoleAppender::Log(const std::shared_ptr<LogEvent> event, Level level) {
    std::lock_guard<std::mutex> lock(mutex_);
    fmt::print(stdout, "{}{}{}",
               rein::color::get_level_color(event->level()),  // 级别对应颜色
               layout_->format(event),                        // 日志内容
               rein::color::Color::RESET                      // 重置颜色
    );
}

NetAppender::NetAppender(const std::string& address)
    : Appender(AppenderType::NET, address) {
    SetLayout();
}

void NetAppender::Log(const std::shared_ptr<LogEvent> event, Level level) {
    fmt::println("This is NetAppender Log");
}

std::shared_ptr<Appender> AppenderFactory::CreateAppender(AppenderType type,
                                                          const std::string& name) {
    switch (type) {
        case AppenderType::CONSOLE:
            // 控制台输出器不需要名称参数
            return std::make_shared<ConsoleAppender>(name);
        case AppenderType::FILE:
            // 文件输出器需要文件名作为参数
            if (name.empty()) {
                return nullptr;
            }
            return std::make_shared<FileAppender>(name);
        case AppenderType::NET:
            // 网络输出器可根据名称参数配置地址等信息
            return std::make_shared<NetAppender>(name);
        default:
            return nullptr;
    }
}

AppenderType AppenderFactory::StringToType(const std::string& type_name) {
    if (type_name == "console") {
        return AppenderType::CONSOLE;
    } else if (type_name == "file") {
        return AppenderType::FILE;
    } else if (type_name == "net") {
        return AppenderType::NET;
    } else {
        // throw std::invalid_argument(
        //     fmt::format("Appender with name '{}' is not exists.", type_name));
        return AppenderType::UNKNOWN;
    }
}

std::string AppenderFactory::TypeToString(AppenderType type) {
    switch (type) {
        case AppenderType::CONSOLE:
            return "console";
        case AppenderType::FILE:
            return "file";
        case AppenderType::NET:
            return "net";
        default:
            throw std::invalid_argument(fmt::format("AppenderType is not exists"));
    }
}

}  // namespace log
}  // namespace rein
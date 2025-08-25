#ifndef REIN_LOG_APPENDER_H_
#define REIN_LOG_APPENDER_H_

#include <memory>
#include <mutex>
#include <string>

#include "event.h"
#include "level.h"

// #include "logger.h"

constexpr const char* kConsole = "console";

namespace rein {
namespace log {
// TODO:  暂时两个输出地，控制台，文件； （网络）
class LogEvent;
class Layout;
class Logger;

/*
  Appender只是单纯的作为一个输出的实例存在于系统之中。 单例模式
*/

enum class AppenderType {
    UNKNOWN,  /// < 未知
    CONSOLE,  ///< 控制台输出器
    FILE,     ///< 文件输出器
    NET       ///< 网络输出器
};

class Appender {
    friend class Logger;

public:
    Appender() = delete;
    explicit Appender(AppenderType type, const std::string& name);
    virtual ~Appender() = default;

    Appender(const Appender&) = delete;
    Appender operator=(const Appender&) = delete;

    virtual void Log(const std::shared_ptr<LogEvent> event, Level level) = 0;
    // virtual bool ToYamlString() = 0;

    template <typename T>
    void SetLevel(T level) {
        std::lock_guard<std::mutex> lock(mutex_);
        level_.SetLevel(level);
    }

    void SetName(const std::string& name);

    void SetLayout(std::shared_ptr<Layout> layout);
    void SetLayout(const std::string& layout);
    void SetLayout();

    const std::string& name() const;
    Level level() const;
    AppenderType type() const;
    std::string type_str() const;
    std::shared_ptr<Layout> layout() const;

protected:
    std::string name_;
    AppenderType type_;  // 基类成员由基类构造初始化
    Level level_;
    std::shared_ptr<Layout> layout_;
    mutable std::mutex mutex_;  // 允许在const中使用
};

// 具体实现类：文件输出地
class FileAppender final : public Appender {
public:
    explicit FileAppender(const std::string& name);
    ~FileAppender() override;
    void Log(const std::shared_ptr<LogEvent> event, Level level) override;
    void ReOpen();
    bool SetFile(std::string name);

    bool IsOpen() const;
    std::string file() const;

private:
    void Open();
    void Close();

private:
    std::string file_name_;
    FILE* file_ = nullptr;
};

// 具体实现类：控制台输出地
class ConsoleAppender final : public Appender {
public:
    ConsoleAppender();
    explicit ConsoleAppender(const std::string& name = kConsole);
    void Log(const std::shared_ptr<LogEvent> event, Level level) override;
};

class NetAppender final : public Appender {
public:
    explicit NetAppender(const std::string& address);
    void Log(const std::shared_ptr<LogEvent> event, Level level) override;

private:
    NetAppender() = delete;
};

class AppenderFactory {
public:
    /// 禁止实例化，所有方法均为静态
    AppenderFactory() = delete;
    AppenderFactory(const AppenderFactory&) = delete;
    AppenderFactory& operator=(const AppenderFactory&) = delete;

    static std::shared_ptr<Appender> CreateAppender(AppenderType type,
                                                    const std::string& name = kConsole);

    /// 将字符串转换为Type枚举
    static AppenderType StringToType(const std::string& type_name);
    static std::string TypeToString(AppenderType type);

private:
};

}  // namespace log
}  // namespace rein

#endif  // REIN_LOG_APPENDER_H_
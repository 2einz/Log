#ifndef REIN_LOG_APPENDER_H_
#define REIN_LOG_APPENDER_H_

#include <ctime>
#include <memory>
#include <mutex>

#include "event.h"
#include "level.h"
#include "logger.h"


namespace rein {
namespace log {
// TODO:  暂时两个输出地，控制台，文件； （网络）
class LogEvent;
class Layout;
class Logger;


/*
  Appender只是单纯的作为一个输出的实例存在于系统之中。 单例模式
*/
class Appender {
    friend class Logger;

public:
    Appender() = default;
    virtual ~Appender() = default;

    virtual bool Log(LogEvent) = 0;
    // virtual bool ToYamlString() = 0;

    template <typename T>
    void SetLevel(T level);

    Level GetLevel() const;

    void SetLayout(std::shared_ptr<Layout> layout);

    std::shared_ptr<Layout> GetLayout() const;

protected:
    Level level_;
    std::shared_ptr<Layout> layout_;
    mutable std::mutex mutex_;  // 允许在const中使用
};

class FileAppender final : public Appender {
public:
    explicit FileAppender(std::string name);
    ~FileAppender() override;
    bool Log(LogEvent) override;
    bool ReOpen();
    bool IsOpen() const;

private:
    bool Open();
    bool Close();

private:
    std::string file_name_;
    FILE* file_ = nullptr;
};

class ConsoleAppender final : public Appender {
    bool Log(LogEvent) override;
};

class NetAppendere final : public Appender {
    bool Log(LogEvent) override;
};

}  // namespace log
}  // namespace rein

#endif  // REIN_LOG_APPENDER_H_
#ifndef REIN_LOG_EVENT_H_
#define REIN_LOG_EVENT_H_

#include <pthread.h>      // Linux 线程接口
#include <sys/syscall.h>  // for SYS_gettid

#include <cstdio>
#include <memory>
#include <string>
#include <thread>

#include "level.h"

namespace rein {
namespace log {

class Logger;

class LogEvent {
public:
    LogEvent(Level level,
             const char* file,
             uint32_t line,
             const char* function,
             std::string message,
             std::shared_ptr<Logger> logger);

  // 获取日志级别
  Level level() const { return level_; }

  // 获取文件名
  const char* file() const { return file_; }

  // 获取行号
  uint32_t line() const { return line_; }

  // 获取程序启动到当前的毫秒数
  uint32_t elapse() const { return elapse_; }

  // 获取pthread线程ID（用户态）
  pthread_t pthread_id() const { return pthread_id_; }

  // 获取协程ID
  uint64_t fiber_id() const { return fiber_id_; }

  // 获取线程名
  const std::string& thread_name() const { return thread_name_; }

  // 获取函数名
  const char* function() const { return function_; }

  // 获取日志消息内容
  const std::string& message() const { return message_; }

  // 获取日志产生的时间戳
  const std::chrono::system_clock::time_point& timestamp() const { return timestamp_; }

  // 获取关联的日志器
  std::shared_ptr<Logger> logger() const { return logger_; }

private:
private:
    Level level_;                                      // 日志等级
    const char* file_;                                 // 文件名（来自 __FILE__）
    uint32_t line_;                                    // 行号（来自 __LINE__）
    uint32_t elapse_;                                  // 程序启动到现在的毫秒数
    uint64_t fiber_id_;                                // 协程ID
    std::string thread_name_;                          // 线程名
    pthread_t pthread_id_;                             // pthread 线程ID（用户态）
    const char* function_;                             // 函数名（__func__）
    std::string message_;                              // 日志消息内容
    std::chrono::system_clock::time_point timestamp_;  // 精确时间戳

    std::shared_ptr<Logger> logger_;  // 关联的日志器
};
}  // namespace log
}  // namespace rein
#endif  // REIN_LOG_EVENT_H_
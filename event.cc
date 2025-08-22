#include "event.h"
rein::log::LogEvent::LogEvent(Level level,
                              const char* file,
                              uint32_t line,
                              const char* function,
                              std::string message,
                              std::shared_ptr<Logger> logger)
    : level_(level),
      file_(file),
      line_(line),
      function_(function),
      message_(std::move(message)),
      logger_(std::move(logger)),
      timestamp_(std::chrono::system_clock::now()),
      pthread_id_(pthread_self()) {
    // 计算程序启动到现在的毫秒数
    static const auto start_time = std::chrono::system_clock::now();
    elapse_ =
        std::chrono::duration_cast<std::chrono::milliseconds>(timestamp_ - start_time).count();

    // 获取线程名（Linux 限制15字符）
    char thread_name_buf[16] = {0};
    pthread_getname_np(pthread_id_, thread_name_buf, sizeof(thread_name_buf));
    thread_name_ = thread_name_buf;
}
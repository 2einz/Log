#ifndef REIN_LOG_MANAGER_H_
#define REIN_LOG_MANAGER_H_

#include <memory>
#include <map>
#include <mutex>

namespace rein {
namespace log {

class Logger;

class LogManager {
public:
    static LogManager& instance();

    std::shared_ptr<Logger> get_logger(const std::string& name);
    std::shared_ptr<Logger> get_root_logger();

private:
    LogManager();  // 私有构造

    std::map<std::string, std::shared_ptr<Logger>> loggers_;
    std::shared_ptr<Logger> root_logger_;
    std::mutex mutex_;
};
}  // namespace log
}  // namespace rein
#endif
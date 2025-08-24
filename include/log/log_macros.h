// 日志宏定义文件，包含所有公开的日志记录宏（如 REIN_LOG_DEBUG）
// 和获取日志器的便捷接口（如 REIN_ROOT_LOGGER()）
// 使用时直接包含：#include <rein/log_macros.h>

#ifndef REIN_LOG_MACROS_H_
#define REIN_LOG_MACROS_H_

#include "log_manager.h"
#include "logger.h"

// 获取 root logger 的便捷宏
#define REIN_ROOT_LOGGER() rein::log::LogManager::instance().get_root_logger()

// 日志宏
#define REIN_LOG_DEBUG(logger, fmt, ...) \
    logger->debug(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define REIN_LOG_INFO(logger, fmt, ...) \
    logger->info(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define REIN_LOG_WARN(logger, fmt, ...) \
    logger->warn(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define REIN_LOG_ERROR(logger, fmt, ...) \
    logger->error(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define REIN_LOG_FATAL(logger, fmt, ...) \
    logger->fatal(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define REIN_LOG_D(fmt, ...) REIN_LOG_DEBUG(REIN_ROOT_LOGGER(), fmt, ##__VA_ARGS__)
#define REIN_LOG_I(fmt, ...) REIN_LOG_INFO(REIN_ROOT_LOGGER(), fmt, ##__VA_ARGS__)
#define REIN_LOG_W(fmt, ...) REIN_LOG_WARN(REIN_ROOT_LOGGER(), fmt, ##__VA_ARGS__)
#define REIN_LOG_E(fmt, ...) REIN_LOG_ERROR(REIN_ROOT_LOGGER(), fmt, ##__VA_ARGS__)
#define REIN_LOG_F(fmt, ...) REIN_LOG_FATAL(REIN_ROOT_LOGGER(), fmt, ##__VA_ARGS__)

#endif  // REIN_LOG_MACROS_H_
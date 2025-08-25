// 日志宏定义文件，包含所有公开的日志记录宏（如 REIN_LOG_DEBUG）
// 和获取日志器的便捷接口（如 REIN_ROOT_LOGGER()）
// 使用时直接包含：#include <rein/log_macros.h>

#ifndef REIN_LOG_MACROS_H_
#define REIN_LOG_MACROS_H_

#include "log_manager.h"
#include "logger.h"

// 获取 root logger 的便捷宏
#define REIN_ROOT_LOGGER() rein::log::LogManager::instance().root_logger()

// 通用日志宏
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

// Root Logger 便捷宏
#define REIN_LOG_D(fmt, ...) REIN_LOG_DEBUG(REIN_ROOT_LOGGER(), fmt, ##__VA_ARGS__)
#define REIN_LOG_I(fmt, ...) REIN_LOG_INFO(REIN_ROOT_LOGGER(), fmt, ##__VA_ARGS__)
#define REIN_LOG_W(fmt, ...) REIN_LOG_WARN(REIN_ROOT_LOGGER(), fmt, ##__VA_ARGS__)
#define REIN_LOG_E(fmt, ...) REIN_LOG_ERROR(REIN_ROOT_LOGGER(), fmt, ##__VA_ARGS__)
#define REIN_LOG_F(fmt, ...) REIN_LOG_FATAL(REIN_ROOT_LOGGER(), fmt, ##__VA_ARGS__)

// 配置宏
/**
 * @brief 添加一个新的 Logger，并为其配置一个 Appender。
 * @param logger_name 要创建的 Logger 的名称 (字符串)。
 * @param appender_type Appender 的类型 (rein::log::AppenderType 枚举值)。
 * @param appender_out Appender 的输出目标 (e.g., 文件名)。
 * @param ... layout (可选, 字符串)。
 * @return 如果成功创建则返回 true，否则返回 false。
 */
#define REIN_ADD_LOGGER(logger_name, appender_type, appender_out, ...)                    \
    rein::log::LogManager::instance().AddLogger(logger_name, appender_type, appender_out, \
                                                ##__VA_ARGS__)

/**
 * @brief 添加一个带默认控制台输出的 Logger。
 * @param logger_name 要创建的 Logger 的名称 (字符串)。
 * @return 如果成功创建则返回 true，否则返回 false。
 */
#define REIN_ADD_CONSOLE_LOGGER(logger_name) \
    rein::log::LogManager::instance().AddLogger(logger_name)

/**
 * @brief 添加一个带文件输出的 Logger。
 * @param logger_name 要创建的 Logger 的名称 (字符串)。
 * @param file_path 日志文件的路径 (字符串)。
 * @return 如果成功创建则返回 true，否则返回 false。
 */
#define REIN_ADD_FILE_LOGGER(logger_name, file_path)                                        \
    rein::log::LogManager::instance().AddLogger(logger_name, rein::log::AppenderType::FILE, \
                                                file_path)

/**
 * @brief 添加一个带网络输出的 Logger。
 * @param logger_name 要创建的 Logger 的名称 (字符串)。
 * @param destination 网络目标地址 (e.g., "127.0.0.1:8080")。
 * @return 如果成功创建则返回 true，否则返回 false。
 */
#define REIN_ADD_NET_LOGGER(logger_name, destination)                                      \
    rein::log::LogManager::instance().AddLogger(logger_name, rein::log::AppenderType::NET, \
                                                destination)

/**
 * @brief 移除一个已存在的 Logger。
 * @param logger_name 要移除的 Logger 的名称 (字符串)。
 * @return 如果成功移除则返回 true，否则返回 false。
 */
#define REIN_REMOVE_LOGGER(logger_name) rein::log::LogManager::instance().RemoveLogger(logger_name)

/**
 * @brief 为一个已存在的 Logger 添加一个新的 Appender。
 * @param logger_name Logger 的名称 (字符串)。
 * @param appender_type Appender 的类型 (rein::log::AppenderType 枚举值)。
 * @param appender_out Appender 的输出目标 (e.g., 文件名)。
 * @return 如果成功添加则返回 true，否则返回 false。
 */
#define REIN_ADD_APPENDER(logger_name, appender_type, appender_out) \
    rein::log::LogManager::instance().AddAppender(logger_name, appender_type, appender_out)

/**
 * @brief 为一个已存在的 Logger 添加一个新的 Console Appender。
 * @param logger_name Logger 的名称 (字符串)。
 * @return 如果成功添加则返回 true，否则返回 false。
 */
#define REIN_ADD_CONSOLE_APPENDER(logger_name) \
    rein::log::LogManager::instance().AddAppender(logger_name, rein::log::AppenderType::CONSOLE, kConsole)

/**
 * @brief 为一个已存在的 Logger 添加一个新的 File Appender。
 * @param logger_name Logger 的名称 (字符串)。
 * @param file_path 日志文件的路径 (字符串)。
 * @return 如果成功添加则返回 true，否则返回 false。
 */
#define REIN_ADD_FILE_APPENDER(logger_name, file_path)                                        \
    rein::log::LogManager::instance().AddAppender(logger_name, rein::log::AppenderType::FILE, \
                                                  file_path)

/**
 * @brief 获取一个已存在的 Logger 实例的 shared_ptr。
 * @param logger_name Logger 的名称 (字符串)。
 * @return std::shared_ptr<rein::log::Logger>
 */
#define REIN_GET_LOGGER(logger_name) rein::log::LogManager::instance().logger(logger_name)

#endif  // REIN_LOG_MACROS_H_
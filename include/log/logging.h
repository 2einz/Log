#ifndef REIN_LOG_LOGGING_H_
#define REIN_LOG_LOGGING_H_

// 包含所有用户需要直接使用的类的定义
#include "log/log_manager.h"
#include "log/logger.h"
#include "log/level.h"
#include "log/appender.h"
// layout 和 event 通常是间接使用，但为了配置方便，也可以暴露
#include "log/layout.h"
#include "log/event.h"
#include "log/log_constants.h"

// 包含所有用户需要使用的宏
#include "log/log_macros.h"

#endif
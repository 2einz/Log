cmake_minimum_required(VERSION 3.11)

# 使用大写驼峰命名法，这是一个常见的约定
project(ReinLog)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 使用 FetchContent 自动管理 fmt 依赖 ---
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        11.2.0 # 使用一个更新、更稳定的版本
)
FetchContent_MakeAvailable(fmt)
# ---------------------------------------------

# --- 定义可执行文件目标 ---
# 1. 使用变量来存储目标名称，避免硬编码
set(EXECUTABLE_TARGET ${PROJECT_NAME}) # 例如，目标名将是 "ReinLog_run"

# 2. 将源文件组织在变量中
set(LOG_SOURCES
    main.cc
    level.cc
    appender.cc
    event.cc
    formatter.cc # 请确认你的文件名是 formatter.cc 还是 formatters.cc
    layout.cc
    log_manager.cc
    logger.cc
    color.cc
)
# 3. 创建可执行文件
add_executable(${EXECUTABLE_TARGET} ${LOG_SOURCES})

# --- 链接库 ---
# 使用变量引用目标名称
target_link_libraries(${EXECUTABLE_TARGET} PRIVATE fmt::fmt)
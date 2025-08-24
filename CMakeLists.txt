cmake_minimum_required(VERSION 3.11)

project(Log)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_EXECUTABLE "Build as executable instead of library" OFF)

# --- 使用 FetchContent 自动管理 fmt 依赖 ---
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        11.2.0
)
FetchContent_MakeAvailable(fmt)



set(EXECUTABLE_TARGET ${PROJECT_NAME}) # 例如，目标名将是 "ReinLog_run"

# 2. 将源文件组织在变量中
set(LOG_SOURCES
    src/appender.cc
    src/color.cc
    src/event.cc
    src/formatter.cc
    src/layout.cc
    src/level.cc
    src/log_manager.cc
    src/logger.cc
)

if(BUILD_EXECUTABLE)
    # 生成可执行文件（需包含main.cc）
    add_executable(${PROJECT_NAME}
        src/main.cc  # 仅可执行文件需要main入口
        ${LOG_SOURCES}
    )
    # 链接依赖和设置头文件目录
    target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
    target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
else()
    # 生成库（默认选项：静态库，可改为SHARED）
    add_library(${PROJECT_NAME} STATIC ${LOG_SOURCES})
    # --- 配置头文件搜索路径（让编译器找到 include/rein/...） ---
    target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    # 链接依赖和设置头文件目录（库需要公开头文件给外部引用）
    target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

endif()
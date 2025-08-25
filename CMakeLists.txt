cmake_minimum_required(VERSION 3.15)

project(rein_log VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_AS_EXECUTABLE "Build a standalone executable for testing instead of a library" OFF)

# --- 依赖管理 ---
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        11.2.0 # 使用一个稳定且存在的版本
)
FetchContent_MakeAvailable(fmt)

# --- 定义统一的目标名称和源文件 ---
set(MAIN_TARGET ${PROJECT_NAME})
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

if(BUILD_AS_EXECUTABLE)
    # --- 构建为可执行文件 ---
    message(STATUS "Building as an executable: ${MAIN_TARGET}")
    add_executable(${MAIN_TARGET} src/main.cc ${LOG_SOURCES})

    # 可执行文件是最终产品，它的依赖和包含目录都是私有的
    target_include_directories(${MAIN_TARGET} PRIVATE include)
    target_link_libraries(${MAIN_TARGET} PRIVATE fmt::fmt)

else()
    # --- 构建为库 ---
    message(STATUS "Building as a STATIC library: ${MAIN_TARGET}")
    add_library(${MAIN_TARGET} STATIC ${LOG_SOURCES})

    # 库需要将它的接口（包含目录和依赖）暴露给使用者
    target_include_directories(${MAIN_TARGET}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            src
    )
    target_link_libraries(${MAIN_TARGET} PUBLIC fmt::fmt)

    # --- 安装规则 ---
    include(CMakePackageConfigHelpers)

    install(TARGETS ${MAIN_TARGET}
        EXPORT ${MAIN_TARGET}_targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )

    install(DIRECTORY include/ DESTINATION include)

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_TARGET}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${MAIN_TARGET}Config.cmake"
        INSTALL_DESTINATION lib/cmake/${MAIN_TARGET}
    )

    install(EXPORT ${MAIN_TARGET}_targets
        FILE ${MAIN_TARGET}Targets.cmake
        NAMESPACE rein::
        DESTINATION lib/cmake/${MAIN_TARGET}
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${MAIN_TARGET}Config.cmake"
        DESTINATION lib/cmake/${MAIN_TARGET}
    )
endif()

# --- 构建示例代码 (总是在库模式下构建) ---
if(NOT BUILD_AS_EXECUTABLE AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
    add_subdirectory(examples)
endif()
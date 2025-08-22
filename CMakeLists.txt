cmake_minimum_required(VERSION 3.11)
project(log)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 直接查找本地fmt库（核心语句）
find_package(fmt REQUIRED)


# 生成可执行文件并链接fmt
add_executable(log main.cc level.cc util.hpp)
target_link_libraries(log PRIVATE fmt::fmt)

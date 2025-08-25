cmake_minimum_required(VERSION 3.15)
project(rein_log_examples)

add_executable(basic_usage basic_usage.cc)

# 链接我们刚刚在主项目中定义的库目标 rein_log
target_link_libraries(basic_usage PRIVATE rein_log)
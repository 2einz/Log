#include <fmt/core.h>
int main() {
    fmt::print("{}", FMT_VERSION);
    // // 基本格式化
    // fmt::print("Hello, {}!\n", "world");

    // // 数字格式化
    // int answer = 42;
    // fmt::print("The answer is {}\n", answer);

    // // 格式化日期时间
    // // fmt::print("Current time: {:%Y-%m-%d %H:%M:%S}\n",
    // //            fmt::localtime(std::time(nullptr)));

    // // 格式化到字符串
    // std::string message =
    //     fmt::format("This is a formatted string with number: {}", 123);
    // fmt::print("{}\n", message);

    // rein::log::Level level1;

    // rein::log::Level level;
    // std::cout << level << std::endl;

    // level.SetLevel("WARN");
    // std::cout << level << std::endl;

    // level.SetLevel(rein::log::LevelType::kError);
    // std::cout << level << std::endl;

    // level.SetLevel(std::string("Fatal"));  // 直接传入 string
    // std::cout << level << std::endl;
    return 0;
}
#include "log/color.h"

#include "log/level.h"

namespace rein {
namespace color {

// C++14 下，必须给 constexpr 静态成员一个 ODR 定义
constexpr const char* Color::RESET;
constexpr const char* Color::BLACK;
constexpr const char* Color::RED;
constexpr const char* Color::GREEN;
constexpr const char* Color::YELLOW;
constexpr const char* Color::BLUE;
constexpr const char* Color::MAGENTA;
constexpr const char* Color::CYAN;
constexpr const char* Color::WHITE;
constexpr const char* Color::BRIGHT_RED;
constexpr const char* Color::BRIGHT_GREEN;
constexpr const char* Color::BRIGHT_YELLOW;
constexpr const char* Color::BRIGHT_BLUE;
constexpr const char* Color::BRIGHT_MAGENTA;
constexpr const char* Color::BRIGHT_CYAN;
constexpr const char* Color::BRIGHT_WHITE;

// 根据日志级别选择颜色
const char* get_level_color(const rein::log::Level& level) {
    switch (level.level()) {
        case log::LevelType::kDebug:
            return Color::CYAN;
        case log::LevelType::kInfo:
            return Color::GREEN;
        case log::LevelType::kWarn:
            return Color::YELLOW;
        case log::LevelType::kError:
            return Color::RED;
        case log::LevelType::kFatal:
            return Color::BRIGHT_RED;
        default:
            return Color::WHITE;
    }
}

}  // namespace color
}  // namespace rein

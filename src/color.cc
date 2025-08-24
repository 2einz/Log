#include "log/color.h"

#include "log/level.h"

namespace rein {
namespace color {
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
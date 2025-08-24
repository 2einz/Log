#ifndef REIN_COLOR_H_  // 使用唯一的宏名
#define REIN_COLOR_H_
#include "level.h"

class Level;

namespace rein {
namespace color {
class Color {
public:
    // 重置颜色
    static constexpr const char* RESET = "\033[0m";

    // 前景色
    static constexpr const char* BLACK = "\033[30m";
    static constexpr const char* RED = "\033[31m";
    static constexpr const char* GREEN = "\033[32m";
    static constexpr const char* YELLOW = "\033[33m";
    static constexpr const char* BLUE = "\033[34m";
    static constexpr const char* MAGENTA = "\033[35m";
    static constexpr const char* CYAN = "\033[36m";
    static constexpr const char* WHITE = "\033[37m";

    // 亮色前景色
    static constexpr const char* BRIGHT_RED = "\033[91m";
    static constexpr const char* BRIGHT_GREEN = "\033[92m";
    static constexpr const char* BRIGHT_YELLOW = "\033[93m";
    static constexpr const char* BRIGHT_BLUE = "\033[94m";
    static constexpr const char* BRIGHT_MAGENTA = "\033[95m";
    static constexpr const char* BRIGHT_CYAN = "\033[96m";
    static constexpr const char* BRIGHT_WHITE = "\033[97m";
};

const char* get_level_color(const rein::log::Level& level);
;

}  // namespace color
}  // namespace rein

#endif
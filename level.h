#ifndef REIN_LOG_LEVEL_H_
#define REIN_LOG_LEVEL_H_

#include <fmt/core.h>

#include <ostream>
#include <string>
#include <type_traits>  // for std::enable_if
#include <unordered_map>
#include <utility>  // for std::forward

namespace rein {
namespace log {

enum class LevelType { kUnknown = 0, kDebug = 1, kInfo = 2, kWarn = 3, kError = 4, kFatal = 5 };

class Level {
    friend std::ostream& operator<<(std::ostream& os, const Level& l);

public:
    Level();
    explicit Level(LevelType level);  // 显式构造
    explicit Level(int level);
    explicit Level(const std::string& level);

    ~Level() = default;
    Level(const Level&) = default;
    Level& operator=(const Level&) = default;

    bool cmp(const std::string& value);
    bool cmp(int value);
    bool cmp(LevelType value);
    bool cmp(Level value);

    // SFINAE技术
    template <
        typename T,
        typename = typename std::enable_if<std::is_enum<typename std::decay<T>::type>::value ||
                                           std::is_integral<typename std::decay<T>::type>::value ||
                                           std::is_convertible<T, std::string>::value>::type>
    void SetLevel(T&& level) {
        SetLevelImpl(std::forward<T>(level));
    }

    std::string string();

    static LevelType FromString(const std::string& level);

    static std::string ToString(const LevelType& level);

    LevelType level() const;

private:
    // 私有的 SetLevelImpl 模板，使用 SFINAE 进行重载决议。
    // 这是实际处理不同类型的实现细节。

    // 处理 LevelType 和其他枚举类型。
    template <typename T>
    auto SetLevelImpl(T&& level) ->
        typename std::enable_if<std::is_enum<typename std::decay<T>::type>::value>::type {
        setLevelFromEnum(static_cast<LevelType>(level));
    }

    // 处理整型，同时排除枚举类型以避免歧义。
    template <typename T>
    auto SetLevelImpl(T&& level) ->
        typename std::enable_if<std::is_integral<typename std::decay<T>::type>::value &&
                                !std::is_enum<typename std::decay<T>::type>::value>::type {
        setLevelFromInt(static_cast<int>(level));
    }

    // 处理可转换为 std::string 的类型 (例如 const char*)。
    template <typename T>
    auto SetLevelImpl(T&& level) ->
        typename std::enable_if<std::is_convertible<T, std::string>::value &&
                                !std::is_integral<typename std::decay<T>::type>::value>::type {
        setLevelFromString(std::string(std::forward<T>(level)));
    }

    void setLevelFromEnum(LevelType level);
    void setLevelFromString(const std::string& level);
    void setLevelFromInt(int level);

private:
    LevelType level_;

    static const std::unordered_map<LevelType, std::string> level_map_;
    static const std::unordered_map<std::string, LevelType> str_map_;
};

std::ostream& operator<<(std::ostream& os, const Level& l);  // 重载运算符

}  // namespace log
}  // namespace rein

#endif  // REIN_LOG_LEVEL_H_
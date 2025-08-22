#include "level.h"

#include "util.hpp"

namespace rein {
namespace log {

const std::unordered_map<LevelType, std::string> Level::level_map_ = {
    {LevelType::kUnknown, "UNKNOW"}, {LevelType::kDebug, "DEBUG"},   {LevelType::kInfo, "INFO"},
    {LevelType::kWarn, "WARN"},      {LevelType::kWarn, "WARNNING"}, {LevelType::kError, "ERROR"},
    {LevelType::kFatal, "FATAL"}};

const std::unordered_map<std::string, LevelType> Level::str_map_ = {
    {"UNKNOW", LevelType::kUnknown}, {"DEBUG", LevelType::kDebug},   {"INFO", LevelType::kInfo},
    {"WARN", LevelType::kWarn},      {"WARNNING", LevelType::kWarn}, {"ERROR", LevelType::kError},
    {"FATAL", LevelType::kFatal}};

Level::Level()
    : level_(LevelType::kInfo) {}

Level::Level(int level)
    : level_(LevelType::kUnknown) {
    SetLevel(level);
}

Level::Level(LevelType level)
    : level_(LevelType::kUnknown) {
    SetLevel(level);
}

Level::Level(const std::string& level) { level_ = FromString(level); }

LevelType Level::FromString(const std::string& level) {
    auto it = str_map_.find(level);
    return (it != str_map_.end()) ? it->second : LevelType::kUnknown;
}

std::string Level::ToString(const LevelType& level) {
    auto it = level_map_.find(level);
    return (it != level_map_.end()) ? it->second : "UNKNOWN";
}

LevelType Level::level() const { return level_; }

void Level::setLevelFromEnum(LevelType level) {
    level_ =
        (level > LevelType::kUnknown && level <= LevelType::kFatal) ? level : LevelType::kUnknown;
}

void Level::setLevelFromString(const std::string& level) {
    std::string upper_level = util::ToUpper(level);
    level_ = FromString(upper_level);
}

void Level::setLevelFromInt(int level) { SetLevel(static_cast<LevelType>(level)); }

bool Level::cmp(const std::string& value) { return cmp(FromString(value)); }

bool Level::cmp(int value) { return cmp(static_cast<LevelType>(value)); }

bool Level::cmp(Level value) { return cmp(value.level()); }

bool Level::cmp(LevelType value) { return level_ < value; }

std::string Level::string() { return ToString(level_); }

std::ostream& operator<<(std::ostream& os, const Level& l) {
    os << Level::ToString(l.level());
    return os;
}

}  // namespace log
}  // namespace rein

#ifndef REIN_LOG_CONSTANTS_H_
#define REIN_LOG_CONSTANTS_H_

#include <string>
#include <vector>

constexpr const char* kDefaultLayout = "%d{:%Y-%m-%d %H:%M:%S.%f} [%p] [%c] %f:%l %m%n";
constexpr const char* kRootLoggerName = "root";
constexpr const char* kDefaultDateTimeParam = ":%Y-%m-%d %H:%M:%S";

const std::vector<std::string> kAppenders = {"console", "file", "netw"};

#endif  // REIN_LOG_CONSTANTS_H_
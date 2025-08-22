#ifndef REIN_LOG_UTIL_H_
#define REIN_LOG_UTIL_H_

#include <algorithm>
#include <cctype>
#include <string>

namespace rein {
namespace util {
inline std::string ToUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::toupper(c); });

    return str;
}

}  // namespace util
}  // namespace rein

#endif
#ifndef REIN_LOG_UTIL_H_
#define REIN_LOG_UTIL_H_

#include <algorithm>
#include <cctype>
#include <cstring>
#include <string>

namespace rein {
namespace util {

// Converts all characters in the string to uppercase.
// Returns a new string with the result.
inline std::string ToUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return str;
}

// Converts all characters in the string to lowercase.
// Returns a new string with the result.
inline std::string ToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return str;
}

// Extracts the filename from a full path.
// For example, "/a/b/c.txt" becomes "c.txt".
// Returns the original path if no '/' is found.
inline const char* GetFileName(const char* full_path) {
    const char* last_slash = strrchr(full_path, '/');
    return (last_slash != nullptr) ? (last_slash + 1) : full_path;
}

// Extracts the last two levels of a path.
// For example, "/a/b/c.txt" becomes "b/c.txt".
// Returns appropriate portions for shorter paths.
inline std::string GetReal2File(const char* full_path) {
    std::string path(full_path);
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        return path;
    }

    // Look for the previous slash
    size_t prev_pos = path.find_last_of('/', pos - 1);
    if (prev_pos == std::string::npos) {
        return path;  // Only one level in path
    }

    return path.substr(prev_pos + 1);
}

}  // namespace util
}  // namespace rein

// Macros for file path handling
#define REIN_UTIL_FILE ::rein::util::GetFileName(__FILE__)
#define REIN_UTIL_RELA_FILE ::rein::util::GetReal2File(__FILE__).c_str()

#endif  // REIN_LOG_UTIL_H_

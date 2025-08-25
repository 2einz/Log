
#include <exception>
#include <iostream>

#include "log/log_macros.h"

int main() {
    auto logger = REIN_ROOT_LOGGER();

    REIN_ADD_CONSOLE_LOGGER("console");

    auto console_log = REIN_GET_LOGGER("console");

    REIN_ADD_FILE_APPENDER(console_log, "log.txt");

    if (console_log) {
        REIN_LOG_INFO(console_log, "Console Logger established.");
    }

    try {
        REIN_ADD_FILE_LOGGER("database", "db.txt");
        auto db_log = REIN_GET_LOGGER("database");

        if (db_log) {
            REIN_LOG_ERROR(db_log, "This is a error message");
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
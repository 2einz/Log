#ifndef REIN_LOG_LAYOUT_H_
#define REIN_LOG_LAYOUT_H_

#include <map>
#include <memory>
#include <vector>
#include <functional>

#include "appender.h"
#include "event.h"
#include "level.h"
namespace rein {
namespace log {
class Layout {
public:
    class FormatterItem {
    public:
        virtual ~FormatterItem() = default;

        virtual void format(fmt::memory_buffer &buffer, const std::shared_ptr<LogEvent> &event) = 0;
    };

    using FormatterFactory = std::function<std::shared_ptr<FormatterItem>()>;

    explicit Layout(const std::string &pattern = "%d{:%Y-%m-%d %H:%M:%S.%f} [%p] %f:%l %m%n");
    ~Layout() = default;

    std::string format(const std::shared_ptr<LogEvent> &event);
    std::string format(const std::shared_ptr<LogEvent> &event, Level level);

private:
    void parse_pattern();

private:
    std::string pattern_;
    std::vector<std::shared_ptr<FormatterItem>> items_;
    static const std::map<std::string, FormatterFactory> formatters_;
};

}  // namespace log
}  // namespace rein
#endif  // REIN_LOG_LAYOUT_H_
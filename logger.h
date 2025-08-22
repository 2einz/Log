#include <string>
namespace rein {
namespace log {

class Logger {
public:
    Logger();
    ~Logger();

    std::string name() const { return name_; }

private:
    std::string name_;
};
}  // namespace log
}  // namespace rein
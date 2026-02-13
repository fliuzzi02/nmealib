#include <string>
#include <stdexcept>

namespace nmealib {

class NmeaException : public std::runtime_error {
public:
    explicit NmeaException(const std::string& context, const std::string& message, const std::string& details = "") : 
    std::runtime_error(context + ": " + message + (details.empty() ? "" : " (" + details + ")")) {}
};

} // namespace nmealib
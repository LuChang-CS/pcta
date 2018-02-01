#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>

namespace utils {
    time_t str2time(const std::string &date) {
        std::tm tm = {};
        std::stringstream ss(date);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        return std::mktime(&tm);
    }

    int daysbetween(long start, long end) {
        return (int) ((end - start) / (24 * 60 * 60));
    }
}
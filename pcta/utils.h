#ifndef __UTILS_H__
#define __UTILS_H__

#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

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

    std::string fileInputContent(fs::path p) {
        std::ifstream in(p.string());
        std::stringstream buffer;
        buffer << in.rdbuf();
        return buffer.str();
    }
}

#endif /* __UTILS_H__ */
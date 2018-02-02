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

    int daysbetween(time_t start, time_t end) {
        return (int) ((end - start) / (24 * 60 * 60));
    }

    time_t nextDate(time_t date) {
        return date + 24 * 60 * 60;
    }

    std::string fileInputContent(const fs::path &p) {
        std::ifstream in(p.string());
        std::stringstream buffer;
        buffer << in.rdbuf();
        return buffer.str();
    }

    fs::path getPathFromDate(time_t date) {
        std::tm tm = *std::localtime(&date);
        int year = tm.tm_year + 1900;
        int month = tm.tm_mon + 1;
        int day = tm.tm_mday;
        fs::path p = std::to_string(year);
        return p.append(std::to_string(month)).append(std::to_string(day));
    }
}

#endif /* __UTILS_H__ */
#ifndef __TIME_RANGE_H__
#define __TIME_RANGE_H__

#include <string>
#include <ctime>

#include "../utils.h"

class TimeRange {
    time_t start;
    time_t end;
    int timeSlotNumber;

private:
    void parse(const std::string &start, const std::string &end) {
        this->start = utils::str2time(start);
        this->end = utils::str2time(end);
    }

public:
    TimeRange(const std::string &start, const std::string &end) {
        this->parse(start, end);
        this->timeSlotNumber = utils::daysbetween(this->start, this->end);
    }

    int getTimeSlotNumber() {
        return this->timeSlotNumber;
    }

    int getTimeSlotNumber(const std::string &date) {
        time_t t = utils::str2time(date);
        return utils::daysbetween(this->start, t);
    }

    int getTimeSlotNumber(time_t date) {
        return utils::daysbetween(this->start, date);
    }

    time_t getStart() {
        return this->start;
    }

    time_t getEnd() {
        return this->end;
    }
};

#endif /* __TIME_RANGE_H__ */

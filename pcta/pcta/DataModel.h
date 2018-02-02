#ifndef __DATA_MODEL_H__
#define __DATA_MODEL_H__

#include <vector>

#include "../model/Dictionary.h"
#include "../model/TimeRange.h"
#include "../model/CategoryRange.h"
#include "../model/WordCount.h"
#include "../model/WordCountList.h"

class DataModel {

protected:
    Dictionary *dictionary;
    TimeRange *timeRange;
    CategoryRange *categoryRange;
    int timeSlotNumber;
    int categoryNumber;

public:
    DataModel() {
        this->dictionary = NULL;
        this->timeRange = NULL;
        this->categoryRange = NULL;
        this->timeSlotNumber = 0;
        this->categoryNumber = 0;
    }

    DataModel(Dictionary *dictionary, TimeRange *timeRange, CategoryRange *categoryRange) {
        this->dictionary = dictionary;
        this->timeRange = timeRange;
        this->categoryRange = categoryRange;
        this->timeSlotNumber = timeRange->getTimeSlotNumber();
        this->categoryNumber = categoryRange->getCategoryNumber();
    }

    virtual ~DataModel() {}

    TimeRange *getTimeRange() {
        return this->timeRange;
    }

    CategoryRange *getCategoryRange() {
        return this->categoryRange;
    }

    int getTimeSlotNumber() {
        return this->timeSlotNumber;
    }

    int getCategoryNumber() {
        return this->categoryNumber;
    }

    virtual WordCountList *getWordCountList(int m, int n) = 0;

    virtual std::vector<WordCountList *>
    getListsByRange(int timeStart, int timeEnd, int categoryStart, int categoryEnd) = 0;
};

#endif /* __DATA_MODEL_H__ */
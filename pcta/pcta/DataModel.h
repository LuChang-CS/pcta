#ifndef __DATA_MODEL_H__
#define __DATA_MODEL_H__

#include <vector>
#include <fstream>
#include <sstream>
#include <string>

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
    fs::path basePath;
    fs::path modelPath;

    fs::path getListPath(int m, int n, const std::string &postfix = "") {
        return this->modelPath / std::to_string(m) / (std::to_string(n) + postfix);
    }

    WordCountList *readList(const fs::path &p) {
        if (!fs::exists(p)) return NULL;
        std::ifstream in(p.string());
        int size;
        in >> size;
        WordCountList *l = new WordCountList(size);

        int wordIndex;
        int count;
        while (in >> wordIndex >> count) {
            l->put(wordIndex, count);
        }
        in.close();
        return l;
    }

    void saveList(const fs::path &p, WordCountList *l) {
        std::ofstream out(p.string());
        std::ostringstream oss;
        oss << l->size() << std::endl;
        WordCountList::iterator it = l->begin();
        while (it != l->end()) {
            oss << (*it)->getWord() << ' ' << (*it)->getCount() << std::endl;
            ++it;
        }
        out << oss.str();
        out.close();
    }

    void saveWordCountList(int m, int n, WordCountList *l) {
        fs::path p = this->getListPath(m, n);
        this->saveList(p, l);
    }

    void mkdir() {
        if (!fs::is_directory(this->modelPath)) {
            fs::create_directories(this->modelPath);
        }
        for (int m = 0; m < this->timeSlotNumber; ++m) {
            fs::path timeDir = this->modelPath / std::to_string(m);
            if (!fs::is_directory(timeDir)) {
                fs::create_directory(timeDir);
            }
        }
    }

public:
    DataModel() {
        this->dictionary = NULL;
        this->timeRange = NULL;
        this->categoryRange = NULL;
        this->timeSlotNumber = 0;
        this->categoryNumber = 0;
    }

    DataModel(Dictionary *dictionary, TimeRange *timeRange, CategoryRange *categoryRange, const fs::path &basePath) {
        this->dictionary = dictionary;
        this->timeRange = timeRange;
        this->categoryRange = categoryRange;
        this->timeSlotNumber = timeRange->getTimeSlotNumber();
        this->categoryNumber = categoryRange->getCategoryNumber();
        this->basePath = basePath;
    }

    virtual ~DataModel() {}

    TimeRange *getTimeRange() {
        return this->timeRange;
    }

    CategoryRange *getCategoryRange() {
        return this->categoryRange;
    }

    Dictionary *getDictionary() {
        return this->dictionary;
    }

    int getTimeSlotNumber() {
        return this->timeSlotNumber;
    }

    int getCategoryNumber() {
        return this->categoryNumber;
    }

    virtual void build() = 0;

    // remember to delete
    WordCountList *getWordCountList(int m, int n) {
        if (m < 0 || n < 0 || m >= this->timeSlotNumber || n >= this->categoryNumber) {
            return NULL;
        }
        fs::path p = this->getListPath(m, n);
        return this->readList(p);
    }

};

#endif /* __DATA_MODEL_H__ */
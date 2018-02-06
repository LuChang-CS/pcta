#ifndef __BASIC_DATA_MODEL_H__
#define __BASIC_DATA_MODEL_H__

#include <iostream>
#include <fstream>
#include <experimental/filesystem>

#include "../libs/json/document.h"

#include "DataModel.h"
#include "../model/WordCountList.h"

namespace fs = std::experimental::filesystem;
namespace json = rapidjson;

class BasicDataModel: public DataModel {

private:
    WordCountList ***wordCountLists;

public:
    BasicDataModel(Dictionary *dictionary, TimeRange *timeRange, CategoryRange *categoryRange)
    : DataModel(dictionary, timeRange, categoryRange) {
        this->wordCountLists = new WordCountList **[this->timeSlotNumber];
        for (int m = 0; m < this->timeSlotNumber; ++m) {
            WordCountList **list = new WordCountList *[this->categoryNumber];
            for (int n = 0; n < this->categoryNumber; ++n) {
                WordCountList *l = new WordCountList();
                list[n] = l;
            }
            this->wordCountLists[m] = list;
        }
    }

    ~BasicDataModel() {
        for (int m = 0; m < this->timeSlotNumber; ++m) {
            WordCountList **list = this->wordCountLists[m];
            if (list != NULL) {
                for (int n = 0; n < this->categoryNumber; ++n) {
                    WordCountList *l = list[n];
                    if (l != NULL) delete l;
                }
                delete list;
            }
        }
        delete this->wordCountLists;
    }

    WordCountList *getWordCountList(int m, int n) {
        if (m < 0 || n < 0 || m >= this->timeSlotNumber || n >= this->categoryNumber) {
            return NULL;
        }
        return this->wordCountLists[m][n];
    }

    std::vector<WordCountList *>
    getListsByRange(int timeStart, int timeEnd, int categoryStart, int categoryEnd) {
        std::vector<WordCountList *> result;
        result.reserve((timeEnd - timeStart) * (categoryEnd - categoryStart));
        for (int m = timeStart; m < timeEnd; ++m) {
            WordCountList **list = this->wordCountLists[m];
            for (int n = categoryStart; n < categoryEnd; ++n) {
                result.push_back(list[n]);
            }
        }
        return result;
    }

    void build(const fs::path &basePath) {
        std::cout << "building basic model" << std::endl;
        time_t current = this->timeRange->getStart();
        time_t end = this->timeRange->getEnd();
        while (current < end) {
            int timeSlotIndex = this->timeRange->getTimeSlotNumber(current);
            std::cout << "building " << timeSlotIndex << " in " << this->timeSlotNumber << std::endl;

            fs::path p = basePath / utils::getPathFromDate(current) / "count";
            std::string content = utils::fileInputContent(p);
            if (content.length() == 0) continue;

            json::Document d;
            d.Parse(content.c_str());
            json::Value::ConstMemberIterator it = d.MemberBegin();
            while (it != d.MemberEnd()) {
                std::string category = it->name.GetString();
                int categoryIndex = this->categoryRange->getCategoryNumber(category);
                WordCountList *l = this->getWordCountList(timeSlotIndex, categoryIndex);

                const json::Value &value = it->value;
                json::Value::ConstMemberIterator wordIt = value.MemberBegin();
                l->reserve(value.MemberCount());
                while (wordIt != value.MemberEnd()) {
                    std::string word = wordIt->name.GetString();
                    int count = wordIt->value.GetInt();
                    int wordIndex = this->dictionary->get(word);
                    l->put(wordIndex, count);
                    ++wordIt;
                }
                ++it;
            }
            current = utils::nextDate(current);
        }
    }

    void saveModel(const fs::path &basePath) {
        std::cout << "saveing basic data model" << std::endl;
        fs::path modelPath = basePath / "model" / "basicModel";
        if (!fs::is_directory(modelPath)) fs::create_directories(modelPath);
        std::ofstream out(modelPath / "model");
        for (int m = 0; m < this->timeSlotNumber; ++m) {
            for (int n = 0; n < this->categoryNumber; ++n) {
                WordCountList *l = this->getWordCountList(m, n);
                WordCountList::iterator it = l->begin();
                out << l->size() << std::endl;
                while (it != l->end()) {
                    out << (*it)->getWord() << ' ' << (*it)->getCount() << ' ';
                    ++it;
                }
                out << std::endl;
            }
        }
        out.close();
    }

    void loadModel(const fs::path &basePath) {
        std::cout << "loading basic data model" << std::endl;
        fs::path modelPath = basePath / "model" / "basicModel";
        std::ifstream in(modelPath / "model");
        for (int m = 0; m < this->timeSlotNumber; ++m) {
            for (int n = 0; n < this->categoryNumber; ++n) {
                int size;
                in >> size;
                WordCountList *l = this->getWordCountList(m, n);
                l->reserve(size);
                for (int i = 0; i < size; ++i) {
                    int word, count;
                    in >> word >> count;
                    l->put(word, count);
                }
            }
        }
    }

};

#endif /* __BASIC_DATA_MODEL_H__ */

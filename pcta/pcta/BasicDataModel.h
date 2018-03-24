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

public:
    BasicDataModel(Dictionary *dictionary, TimeRange *timeRange, CategoryRange *categoryRange, const fs::path &basePath)
    : DataModel(dictionary, timeRange, categoryRange, basePath) {
        this->modelPath = this->basePath / "model" / "basicModel";
    }

    void build() {
        std::cout << "building basic model" << std::endl;
        this->mkdir();
        time_t current = this->timeRange->getStart();
        time_t end = this->timeRange->getEnd();
        while (current < end) {
            int timeSlotIndex = this->timeRange->getTimeSlotNumber(current);
            std::cout << "building " << timeSlotIndex << " in " << this->timeSlotNumber << std::endl;

            fs::path p = this->basePath / utils::getPathFromDate(current) / "count";
            std::string content = utils::fileInputContent(p);
            if (content.length() == 0) continue;

            json::Document d;
            d.Parse(content.c_str());
            json::Value::ConstMemberIterator it = d.MemberBegin();
            while (it != d.MemberEnd()) {
                std::string category = it->name.GetString();
                int categoryIndex = this->categoryRange->getCategoryNumber(category);
                WordCountList *l = new WordCountList();

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
                l->sort();
                this->saveWordCountList(timeSlotIndex, categoryIndex, l);
                delete l;
                ++it;
            }
            current = utils::nextDate(current);
        }
    }

};

#endif /* __BASIC_DATA_MODEL_H__ */

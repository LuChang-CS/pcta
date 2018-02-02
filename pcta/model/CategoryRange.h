#ifndef __CATEGORY_RANGE_H__
#define __CATEGORY_RANGE_H__

#include <unordered_map>
#include <string>
#include <vector>
#include <experimental/filesystem>

#include "../libs/json/document.h"

namespace fs = std::experimental::filesystem;
namespace json = rapidjson;

typedef std::unordered_map<std::string, std::vector<int> *> CategoryMap;

class CategoryRange {
    CategoryMap categoryMap;
    int categoryNumber;

public:
    CategoryRange(const fs::path &p) {
        this->setCategoryRange(d);
        this->categoryNumber = this->categoryMap.size();
    }

    ~CategoryRange() {
        for (auto &item : this->categoryMap) {
            std::vector<int> *index = item.second;
            if (index != NULL) delete index;
        }
    }

    void setCategoryRange(const fs::path &p) {
        const string content = utils::fileInputContent(p);
        json::Document d;
        d.Parse(content.c_str());
        this->categoryMap.clear();
        int size = d.MemberCount();
        this->categoryMap.reserve(size);
        json::Value::ConstMemberIterator it = d.MemberBegin();
        while (it != d.MemberEnd()) {
            std::string category = it->name.GetString();
            json::Type type = it->value.GetType();

            std::vector<int> *index = new std::vector<int>;
            index->reserve(2);
            if (type == json::kNumberType) {
                index->push_back(it->value.GetInt());
            } else {
                const json::Value &a = it->value;
                index->push_back(a[0].GetInt());
                index->push_back(a[1].GetInt());
            }
            this->categoryMap.insert(std::make_pair(category, index));
            ++it;
        }
    }

    int getCategoryNumber() {
        return this->categoryNumber;
    }

    int getCategoryNumber(const std::string &category) {
        CategoryMap::const_iterator it = this->categoryMap.find(category);
        return it == this->categoryMap.end() ? -1 : it->second->at(0);
    }

    std::vector<int> *getCategoryRange(const std::string &category) {
        CategoryMap::const_iterator it = this->categoryMap.find(category);
        return it == this->categoryMap.end() ? NULL : it->second;
    }
};

#endif /* __CATEGORY_RANGE_H__ */

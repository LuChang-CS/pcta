#ifndef __CATEGORY_RANGE_H__
#define __CATEGORY_RANGE_H__

#include <unordered_map>
#include <string>

typedef std::unordered_map<std::string, int> CategoryMap;

class CategoryRange {
    CategoryMap categoryMap;
    int categoryNumber;

public:
    CategoryRange() {
        
    }
};

#endif /* __CATEGORY_RANGE_H__ */

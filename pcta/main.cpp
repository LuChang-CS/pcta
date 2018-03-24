#include <iostream>
#include <string>
#include <sstream>
#include <experimental/filesystem>

#include "model/WordCountList.h"
#include "model/Dictionary.h"
#include "model/CategoryRange.h"
#include "model/TimeRange.h"
#include "pcta/BasicDataModel.h"
#include "pcta/PCTAModel.h"
#include "utils.h"

#include "libs/json/document.h"

namespace fs = std::experimental::filesystem;
namespace json = rapidjson;

using namespace std;

int main() {
    fs::path basePath = "/media/sdb1/luchang/workspace/fdu/dataset/reuters/";
    Dictionary dictionary(basePath / "dictionary");
    CategoryRange categoryRange(basePath / "categories_dict");
    TimeRange timeRange("2016-01-01", "2017-01-01");

    BasicDataModel basicDataModel(&dictionary, &timeRange, &categoryRange, basePath);
    basicDataModel.build();

    // cout << basicDataModel.getTimeSlotNumber() << " " << basicDataModel.getCategoryNumber() << endl;
    PCTAModel pctaModel(&basicDataModel, basePath);
    pctaModel.build();

    // WordCountList *l = basicDataModel.getWordCountList(2, 5);
    // WordCountList::iterator it = l->begin();
    // for (; it != l->end(); ++it) {
    //     cout << (*it)->toString() << endl;
    // }
    return 0;
}

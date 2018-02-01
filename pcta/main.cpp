#include <iostream>
#include <string>
#include <sstream>
#include <experimental/filesystem>

#include "utils.h"

#include "libs/json/document.h"

namespace fs = std::experimental::filesystem;
namespace json = rapidjson;

using namespace std;

int main() {
    fs::path p = "/media/sdb1/luchang/workspace/fdu/dataset/test/categories_dict";
    const string &content = utils::fileInputContent(p);
    json::Document d;
    d.Parse(content.c_str());
    cout << d["cat3"].GetInt() << endl;
    return 0;
}

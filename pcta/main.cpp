#include <iostream>
#include <string>
#include <experimental/filesystem>
#include "utils.h"

namespace fs = std::experimental::filesystem;

using namespace std;

int main() {
    fs::path p = "/media/sdb1/luchang/workspace/fdu/dataset/test/categories_dict";
    cout << p.string() << endl;
    string content = utils::fileInputContent(p);
    cout << content << endl;
    return 0;
}

#include <iostream>
#include <string>
#include <experimental/filesystem>
#include "utils.h"

namespace fs = std::experimental::filesystem;

using namespace std;

int main() {
    fs::path p = "C:/Users/chang/Documents/luchang/workspace/www/demo/test.json";
    cout << p.string();
    string content = utils::fileInputContent(p);
    cout << content << endl;
    return 0;
}

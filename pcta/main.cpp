#include <iostream>
#include <string>
#include <sstream>
#include <experimental/filesystem>

#include "model/WordCountList.h"
#include "utils.h"

#include "libs/json/document.h"

namespace fs = std::experimental::filesystem;
namespace json = rapidjson;

using namespace std;

int main() {
    WordCountList *wordCountList = new WordCountList(5);
    wordCountList->put(1, 5)->put(2, 3)->put(3, 1)->put(4, 2)->put(5, 4);
    WordCountList::iterator it = wordCountList->begin();
    WordCountList::const_iterator end = wordCountList->end();
    wordCountList->sort();
    while (it < end) {
        cout << (*it)->toString() << endl;
        ++it;
    }
    cout << endl;

    WordCountList *w2 = new WordCountList(wordCountList->size());
    w2->negativeReverse(wordCountList);
    w2->add(wordCountList);
    WordCountList::iterator it1 = w2->begin();
    WordCountList::const_iterator end1 = w2->end();
    while (it1 < end1) {
        cout << (*it1)->toString() << endl;
        ++it1;
    }
    return 0;
}

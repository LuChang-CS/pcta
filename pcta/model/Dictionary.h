#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <string>
#include <unordered_map>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

typedef std::unordered_map<std::string, int> WordMap;

class Dictionary {
    WordMap wordMap;

public:
    Dictionary(const fs::path &p) {
        std::ifstream in(p.string());
        std::string word;
        int i = 0;
        while (in >> word) {
            this->wordMap.insert(std::make_pair(word, i++));
        }
    }

    int get(std::string &word) {
        WordMap::const_iterator it = this->wordMap.find(word);
        return it == this->wordMap.end() ? -1 : it->second;
    }
};

#endif /* __DICTIONARY_H__ */

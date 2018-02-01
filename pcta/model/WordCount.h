#ifndef __WORD_COUNT_H__
#define __WORD_COUNT_H__

#include <string>

class WordCount {

    int word;

    int count;

public:

    WordCount(int word): word(word) {
        this->count = 0;
    }

    WordCount(int word, int count): word(word), count(count) {}

    int getWord() const {
        return this->word;
    }

    int getCount() const {
        return this->count;
    }

    void addCount(int count) {
        this->count += count;
    }

    std::string toString() {
        return std::to_string(this->word) + " " + std::to_string(this->count);
    }

    bool operator< (const WordCount &o) {
        return o.count > this->count;
    }
};

#endif /* __WORD_COUNT_H__ */

#ifndef __WORD_COUNT_LIST_H__
#define __WORD_COUNT_LIST_H__

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "WordCount.h"

typedef std::unordered_map<int, WordCount *> WordCountMap;
typedef std::vector<WordCount *> WordCountVector;

bool wordCountPointerCmp(const WordCount *wc1, const WordCount *wc2) {
    return wc2->getCount() < wc1->getCount();
}

class WordCountList {

    WordCountMap wordCountMap;

    WordCountVector wordCountVector;

public:

    typedef WordCountVector::iterator iterator;
    typedef WordCountVector::const_iterator const_iterator;

    WordCountList(int size) {
        this->reserve(size);
    }

    ~WordCountList() {
        WordCountList::iterator it = this->begin();
        WordCountList::const_iterator end = this->end();
        while (it != end) {
            delete *it;
            ++it;
        }
    }

    int get(int word) {
        WordCountMap::const_iterator it = this->wordCountMap.find(word);
        return it == this->wordCountMap.end() ? -1 : it->second->getCount();
    }

    WordCountList *put(int word, int count) {
        WordCount *wc = new WordCount(word, count);
        this->wordCountMap.insert(std::make_pair(word, wc));
        this->wordCountVector.push_back(wc);
        return this;
    }

    WordCountList *put(WordCountList *wordCountList) {
        if (wordCountList != NULL) {
            WordCountList::iterator it = wordCountList->begin();
            WordCountList::const_iterator end = wordCountList->end();
            while (it != end) {
                this->put((*it)->getWord(), (*it)->getCount());
                ++it;
            }
        }
        return this;
    }

    WordCountList *add(int word, int count) {
        WordCountMap::const_iterator it = this->wordCountMap.find(word);
        if (it == this->wordCountMap.end()) return this->put(word, count);
        it->second->addCount(count);
        return this;
    }

    WordCountList *add(WordCountList *wordCountList) {
        if (wordCountList != NULL) {
            WordCountList::iterator it = wordCountList->begin();
            WordCountList::const_iterator end = wordCountList->end();
            while (it != end) {
                this->add((*it)->getWord(), (*it)->getCount());
                ++it;
            }
        }
        return this;
    }

    WordCountList *minus(WordCountList *wordCountList) {
        if (wordCountList != NULL) {
            WordCountList::iterator it = wordCountList->begin();
            WordCountList::const_iterator end = wordCountList->end();
            while (it != end) {
                this->add((*it)->getWord(), -(*it)->getCount());
                ++it;
            }
        }
        return this;
    }

    void sort() {
        std::sort(this->begin(), this->end(), wordCountPointerCmp);
    }

    iterator begin() {
        return this->wordCountVector.begin();
    }

    iterator end() {
        return this->wordCountVector.end();
    }

    void negativeReverse(WordCountList *wordCountList) {
        if (wordCountList != NULL) {
            this->clear();
            WordCountList::iterator it = wordCountList->begin();
            WordCountList::const_iterator end = wordCountList->end();
            while (it != end) {
                this->put((*it)->getWord(), -(*it)->getCount());
                ++it;
            }
            std::reverse(this->begin(), this->end());
        }
    }

    int size() {
        return this->wordCountVector.size();
    }

    void reserve(int size) {
        this->wordCountMap.reserve(size);
        this->wordCountVector.reserve(size);
    }

    void clear() {
        this->wordCountMap.clear();
        this->wordCountVector.clear();
    }

};

#endif /* __WORD_COUNT_LIST_H__ */

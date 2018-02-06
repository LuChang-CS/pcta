#ifndef __PCTA_MODEL_H__
#define __PCTA_MODEL_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <experimental/filesystem>

#include "DataModel.h"
#include "BasicDataModel.h"
#include "../model/WordCountList.h"

namespace fs = std::experimental::filesystem;

class PCTAModel: public DataModel {

private:

    BasicDataModel *basicDataModel;
    fs::path basePath;
    fs::path modelPath;

    fs::path getListPath(int m, int n, const std::string &postfix = "") {
        return this->modelPath / std::to_string(m) / (std::to_string(n) + postfix);
    }

    WordCountList *readList(const fs::path &p) {
        std::ifstream in(p.string());
        int size;
        in >> size;
        WordCountList *l = new WordCountList(size);

        int wordIndex;
        int count;
        while (in >> wordIndex >> count) {
            l->put(wordIndex, count);
        }
        in.close();
        return l;
    }

    void saveList(const fs::path &p, WordCountList *l) {
        std::ofstream out(p.string());
        out << l->size() << std::endl;
        WordCountList::iterator it = l->begin();
        while (it != l->end()) {
            out << (*it)->getWord() << ' ' << (*it)->getCount() << std::endl;
            ++it;
        }
        out.close();
    }

    WordCountList *getWordCountList(int m, int n) {
        if (m < 0 || n < 0 || m >= this->timeSlotNumber || n >= this->categoryNumber) {
            return NULL;
        }
        fs::path p = this->getListPath(m, n);
        return this->readList(p);
    }

    WordCountList *getnrWordCountList(int m, int n) {
        if (m < 0 || n < 0 || m >= this->timeSlotNumber || n >= this->categoryNumber) {
            return NULL;
        }
        fs::path p = this->getListPath(m, n, "n");
        return this->readList(p);
    }

    void saveWordCountList(int m, int n, WordCountList *l) {
        fs::path p = this->getListPath(m, n);
        this->saveList(p, l);
    }

    void savenrWordCountList(int m, int n, WordCountList *l) {
        fs::path p = this->getListPath(m, n, "n");
        this->saveList(p, l);
    }

    void mkdir() {
        if (!fs::is_directory(this->modelPath)) {
            fs::create_directories(this->modelPath);
        }
        for (int m = 0; m < this->timeSlotNumber; ++m) {
            fs::path timeDir = this->modelPath / std::to_string(m);
            if (!fs::is_directory(timeDir)) {
                fs::create_directory(timeDir);
            }
        }
    }

public:

    PCTAModel(BasicDataModel *basicDataModel, const fs::path &basePath)
    : DataModel(basicDataModel->getDictionary(), basicDataModel->getTimeRange(), basicDataModel->getCategoryRange()) {
        this->basicDataModel = basicDataModel;
        this->basePath = basePath;
        this->modelPath = basePath / "model" / "pctaModel";

        this->mkdir();
    }

    void build() {
        std::cout << "building pcta model" << std::endl;
        WordCountList **tempLists = new WordCountList *[this->categoryNumber + 1];
        for (int n = 0; n < this->categoryNumber + 1; ++n) {
            tempLists[n] = NULL;
        }
        for (int m = 0; m < this->timeSlotNumber; ++m) {
            std::cout << "building " << m << " in " << this->timeSlotNumber << std::endl;
            WordCountList *l_mn = NULL,
                        *l_prime_mnminus1 = NULL,
                        *l_prime_mminus1n = NULL,
                        *l_prime_mminus1nminus1 = NULL,
                        *l_prime_mn = NULL;
            WordCountList *tempList = NULL;
            for (int n = 0; n < this->categoryNumber; ++n) {
                l_mn = this->basicDataModel->getWordCountList(m, n);
                // l_prime_mnminus1 = this->getWordCountList(m, n - 1);
                l_prime_mnminus1 = tempList;
                // l_prime_mminus1n = this->getWordCountList(m - 1, n);
                l_prime_mminus1n = tempLists[n + 1];
                // l_prime_mminus1nminus1 = this->getWordCountList(m - 1, n - 1);
                l_prime_mminus1nminus1 = tempLists[n];

                int size = 0;
                if (l_prime_mnminus1 != NULL) size += l_prime_mnminus1->size();
                if (l_prime_mminus1n != NULL) size += l_prime_mminus1n->size();
                if (l_prime_mminus1nminus1 != NULL) size -= l_prime_mminus1nminus1->size();
                if (l_mn != NULL) size += l_mn->size();
                l_prime_mn = new WordCountList(size);
                l_prime_mn->add(l_prime_mnminus1)
                        ->add(l_prime_mminus1n)
                        ->minus(l_prime_mminus1nminus1)
                        ->add(l_mn);
                l_prime_mn->sort();

                WordCountList *nrList = new WordCountList(l_prime_mn->size());
                nrList->negativeReverse(l_prime_mn);

                this->saveWordCountList(m, n, l_prime_mn);
                this->savenrWordCountList(m, n, nrList);

                tempLists[n] = tempList;
                tempList = l_prime_mn;

                if (l_prime_mminus1nminus1 != NULL) delete l_prime_mminus1nminus1;
                delete nrList;
            }
            if (l_prime_mminus1n != NULL) delete l_prime_mminus1n;
            tempLists[this->categoryNumber] = l_prime_mn;
        }
        for (int n = 0; n < this->categoryNumber + 1; ++n) {
            WordCountList *l = tempLists[n];
            if (l != NULL) delete l;
        }
    }

    std::vector<WordCountList *>
    getListsByRange(int timeStart, int timeEnd, int categoryStart, int categoryEnd) {
        std::vector<WordCountList *> t;
        return t;
    }

};

#endif /* __PCTA_MODEL_H__ */

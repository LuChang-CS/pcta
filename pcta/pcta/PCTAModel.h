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

public:

    PCTAModel(BasicDataModel *basicDataModel, const fs::path &basePath)
    : DataModel(basicDataModel->getDictionary(), basicDataModel->getTimeRange(), basicDataModel->getCategoryRange(), basePath) {
        this->basicDataModel = basicDataModel;
        this->modelPath = this->basePath / "model" / "pctaModel";
    }

    void build() {
        std::cout << "building pcta model" << std::endl;
        this->mkdir();
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
                delete l_mn;
            }
            if (l_prime_mminus1n != NULL) delete l_prime_mminus1n;
            tempLists[this->categoryNumber] = l_prime_mn;
        }
        for (int n = 0; n < this->categoryNumber + 1; ++n) {
            WordCountList *l = tempLists[n];
            if (l != NULL) delete l;
        }
    }

private:

    WordCountList *getnrWordCountList(int m, int n) {
        if (m < 0 || n < 0 || m >= this->timeSlotNumber || n >= this->categoryNumber) {
            return NULL;
        }
        fs::path p = this->getListPath(m, n, "n");
        return this->readList(p);
    }

    void savenrWordCountList(int m, int n, WordCountList *l) {
        fs::path p = this->getListPath(m, n, "n");
        this->saveList(p, l);
    }

};

#endif /* __PCTA_MODEL_H__ */

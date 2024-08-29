#ifndef WORKER_H
#define WORKER_H

#include "DynQueue.h"

using namespace std;


struct Top3 {
    vector<District> districts;
    vector<float> scores;

    Top3() = default;
    Top3(const Top3&) = delete;
    Top3& operator=(const Top3&) = delete;
    Top3(Top3&&) noexcept = default;
    Top3& operator=(Top3&&) noexcept = default;

    void addDistrict(const District& district, float score);
};

float calculateDissimilarity(const District& district, const District& userHouse);

void processDistricts(Queue& q, const District& userHouse, Top3& top3, mutex& queueMutex, mutex& coutMutex);


#endif // WORKER_H

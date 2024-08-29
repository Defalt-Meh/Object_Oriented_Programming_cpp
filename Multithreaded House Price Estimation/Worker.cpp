#include "Worker.h"


using namespace std;

float calculateDissimilarity(const District& district, const District& userHouse) {
    float ageDiff = (district.median_age - userHouse.median_age) / 26.0f;
    float roomCountDiff = (district.avg_room_count - userHouse.avg_room_count) / 10.0f;
    float bedroomCountDiff = (district.avg_bedroom_count - userHouse.avg_bedroom_count) / 4.0f;
    float latitudeDiff = (district.latitude - userHouse.latitude) / 1.0f;
    float longitudeDiff = (district.longitude - userHouse.longitude) / 1.0f;

    return sqrt(ageDiff * ageDiff +
                roomCountDiff * roomCountDiff +
                bedroomCountDiff * bedroomCountDiff +
                latitudeDiff * latitudeDiff +
                longitudeDiff * longitudeDiff);
}

void Top3::addDistrict(const District& district, float score) {
    if (districts.size() < 3) {
        districts.push_back(district);
        scores.push_back(score);
    } else {
        auto maxElem = max_element(scores.begin(), scores.end());
        if (score < *maxElem) {
            size_t index = distance(scores.begin(), maxElem);
            districts[index] = district;
            scores[index] = score;
        }
    }
}

void processDistricts(Queue& q, const District& userHouse, Top3& top3, mutex& queueMutex, mutex& coutMutex) {
    static thread_local size_t count = 0;

    while (true) {
        District district;

        {
            lock_guard<mutex> lock(queueMutex);
            if (q.isEmpty()) break;
            district = q.dequeue();
        }

        float dissimilarity = calculateDissimilarity(district, userHouse);

        // Update top3 with the current district if necessary
        top3.addDistrict(district, dissimilarity);

        // Print progress every 1000 items
        if (++count % 1000 == 0) {
            lock_guard<mutex> lock(coutMutex);
            // Convert thread ID to a string using stringstream
            hash<std::thread::id> hasher;
            size_t threadID = hasher(this_thread::get_id());  // Hash the thread ID
            std::stringstream ss;
            ss << threadID;
            cout << "Thread with id " << ss.str() << " is processing its " << count << "th item." << endl;
        }
    }

    lock_guard<mutex> lock(coutMutex);
    // Convert thread ID to a string using stringstream
    hash<std::thread::id> hasher;
    size_t threadID = hasher(this_thread::get_id());  // Hash the thread ID
    std::stringstream ss;
    ss << threadID;
    cout << "Thread with id " << ss.str() << " is exiting." << endl;
}

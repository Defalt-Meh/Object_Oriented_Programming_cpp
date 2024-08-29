#include "DynQueue.h"
#include "Worker.h"

using namespace std;

mutex queueMutex;
mutex coutMutex;  // Mutex for synchronizing console output

// Function to convert thread::id to a decimal string
string thread_id_to_string(thread::id id) {
    hash<thread::id> hasher;
    size_t threadID = hasher(id);
    stringstream ss;
    ss << threadID;  // Convert to decimal
    return ss.str();
}

void readFile(const string& filename, Queue& q) {
    ifstream file(filename);
    if (!file) {
        cerr << "Unable to open file\n";
        return;
    }

    string line;
    getline(file, line);  // Skip the header line

    while (getline(file, line)) {
        stringstream ss(line);
        District district;
        string temp;

        getline(ss, temp, '\t');
        district.id = stoi(temp);

        getline(ss, temp, '\t');
        district.median_age = stof(temp);

        getline(ss, temp, '\t');
        district.avg_room_count = stof(temp);

        getline(ss, temp, '\t');
        district.avg_bedroom_count = stof(temp);

        getline(ss, temp, '\t');
        district.latitude = stof(temp);

        getline(ss, temp, '\t');
        district.longitude = stof(temp);

        getline(ss, temp, '\t');
        district.median_price = stof(temp);  // Read the median price

        q.enqueue(district);
    }

    lock_guard<mutex> lock(coutMutex);
    cout << "20640 districts have been loaded." << endl;
    file.close();
}


int main() {
    Queue queue;
    size_t numThreads;

    string filename = "/Users/mehmetaltunoren/Desktop/data.tsv";  // Adjust to your file path

    // Load data into the queue
    thread fileThread(readFile, filename, ref(queue));
    fileThread.join();

    cout << "Enter the number of threads to use: ";
    cin >> numThreads;

    if (numThreads <= 0) {
        cerr << "Number of threads must be positive.\n";
        return 1;
    }

    // Gather user input for their house details
    District userHouse;
    cout << "What is the age of the house?: ";
    cin >> userHouse.median_age;
    cout << "How many rooms does the house have?: ";
    cin >> userHouse.avg_room_count;
    cout << "How many bedrooms does the house have?: ";
    cin >> userHouse.avg_bedroom_count;
    cout << "What is the latitude of the house?: ";
    cin >> userHouse.latitude;
    cout << "What is the longitude of the house?: ";
    cin >> userHouse.longitude;

    vector<thread> threads;
    vector<Top3> top3Threads(numThreads);

    // Start threads to process districts
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back(processDistricts, ref(queue), userHouse, ref(top3Threads[i]), ref(queueMutex), ref(coutMutex));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    // Combine the top 3 districts from all threads
    Top3 finalTop3;
    for (const auto& top3 : top3Threads) {
        for (size_t i = 0; i < top3.districts.size(); ++i) {
            finalTop3.addDistrict(top3.districts[i], top3.scores[i]);
        }
    }

    // Calculate the average median house price of the top 3 districts
    float averagePrice = 0.0f;
    lock_guard<mutex> lock(coutMutex);
    cout << "Closest three districts in terms of features have the following median prices:" << endl;

    for (size_t i = 0; i < finalTop3.districts.size(); ++i) {
        cout << "Closest District " << (i + 1) << endl;
        cout << "--------" << endl;
        cout << "Id: " << finalTop3.districts[i].id << endl;  // Display district ID
        cout << "Median Price: " << finalTop3.districts[i].median_price << endl;  // Display median price
        cout << "Calculated Dissimilarity Metric: " << finalTop3.scores[i] << endl;
        cout << endl;
        averagePrice += finalTop3.districts[i].median_price;  // Sum up the median prices
    }

    averagePrice /= finalTop3.districts.size();  // Calculate average

    cout << "Average median price of three closest districts: " << averagePrice << endl;

    return 0;
}

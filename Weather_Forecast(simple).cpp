#include <iostream>
#include <thread>
#include <map>
#include <string>
#include <chrono>
#include <mutex>

using namespace std::chrono_literals;

// Mutex for thread safety
std::mutex mapMutex;

void RefreshForecast(std::map<std::string, int>& forecastMap) {
    while (true) {
        {
            // Lock the mutex to safely access the shared map
            std::lock_guard<std::mutex> lock(mapMutex);
            for (auto& item : forecastMap) {
                item.second++;
                std::cout << item.first << " - " << item.second << std::endl;
            }
        }
        std::this_thread::sleep_for(2000ms);
    }
}

int main() {
    std::map<std::string, int> forecastMap = {
        {"New York", 15},
        {"Mumbai", 20},
        {"Berlin", 18}
    };

    // Pass map by reference to avoid copying
    std::thread bgWorker(RefreshForecast, std::ref(forecastMap));
    
    // Let the background thread run for some time
    std::this_thread::sleep_for(10000ms); // 10 seconds

    // Detach or join the thread to ensure proper cleanup
    bgWorker.join();
    
    return 0;
}

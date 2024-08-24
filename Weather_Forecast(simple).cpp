#include <iostream>
#include <thread>
#include <map>
#include <string>
#include <chrono>
#include <mutex>
#include <iomanip> // For std::setw

using namespace std::chrono_literals;

class WeatherForecast {
public:
    WeatherForecast(const std::map<std::string, int>& initialData)
        : forecastMap(initialData), running(true) {
        refreshThread = std::thread(&WeatherForecast::RefreshForecast, this);
    }

    ~WeatherForecast() {
        StopRefreshing();
    }

    void StopRefreshing() {
        if (running) {
            running = false;
            if (refreshThread.joinable()) {
                refreshThread.join();
            }
        }
    }

private:
    std::map<std::string, int> forecastMap;
    std::thread refreshThread;
    std::mutex mapMutex;
    bool running;

    void RefreshForecast() {
        while (running) {
            {
                std::lock_guard<std::mutex> lock(mapMutex);
                std::cout << std::left << std::setw(15) << "Location" << " - " << "Temperature" << std::endl;
                for (const auto& item : forecastMap) {
                    std::cout << std::left << std::setw(15) << item.first << " - " << item.second << "°C" << std::endl;
                }
                std::cout << std::string(30, '-') << std::endl;
            }
            std::this_thread::sleep_for(2s);
            
            // Update temperatures
            std::lock_guard<std::mutex> lock(mapMutex);
            for (auto& item : forecastMap) {
                item.second++;
            }
        }
    }
};

int main() {
    // Initialize forecast data
    std::map<std::string, int> initialData = {
        {"New York", 15},
        {"Mumbai", 20},
        {"Berlin", 18}
    };

    // Create and start the WeatherForecast object
    WeatherForecast forecast(initialData);

    // Let the background thread run for 10 seconds
    std::this_thread::sleep_for(10s);

    // Stop the background thread and cleanup
    forecast.StopRefreshing();

    return 0;
}

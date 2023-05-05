#include "message_queue.h"
#include "queue_analyzer.h"

#include <thread>
#include <chrono>
#include <random>
#include <atomic>
#include <string>

using namespace std;

int main() {
        const int queue_size = 5;
        // Create a message queue with a maximum size of 5
        MessageQueue<string> queue(queue_size);

        // Create a QueueAnalyzer to periodically analyze the message queue
        QueueAnalyzer<string> analyzer(queue, "queue_analysis.txt");

        // Create a random number generator
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 1);

        // Define atomic variable to keep track of analyzer function calls
        std::atomic<int> analyzer_calls{ 0 };

        int workingTimeInSec = 300;

        // Start 5 threads
        std::thread thread1([&queue, &generator, &distribution, &workingTimeInSec]() {
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < workingTimeInSec) {
                std::this_thread::sleep_for(std::chrono::seconds(distribution(generator)));
                if (distribution(generator) == 0) {
                    queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(5), 1, "High priority message"));
                }
                else {
                    queue.pop();
                }
            }
            });

        std::thread thread2([&queue, &generator, &distribution, &workingTimeInSec]() {
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < workingTimeInSec) {
                std::this_thread::sleep_for(std::chrono::seconds(distribution(generator)));
                if (distribution(generator) == 0) {
                    queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(10), 2, "Medium priority message"));
                }
                else {
                    queue.pop();
                }
            }
            });

        std::thread thread3([&queue, &generator, &distribution, &workingTimeInSec]() {
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < workingTimeInSec) {
                std::this_thread::sleep_for(std::chrono::seconds(distribution(generator)));
                if (distribution(generator) == 0) {
                    queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(15), 3, "Low priority message"));
                }
                else {
                    queue.pop();
                }
            }
            });

        std::thread thread4([&queue, &workingTimeInSec]() {
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < workingTimeInSec) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                queue.pop();
            }
            });

        std::thread thread5([&queue, &analyzer, &analyzer_calls, &workingTimeInSec, &queue_size]() {
            std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
            const int MIN_ANALYZE_INTERVAL = 60;
            int last_analysis_time = 0;
            while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < workingTimeInSec) {

                if (queue.size() == queue_size || last_analysis_time == MIN_ANALYZE_INTERVAL) {
                    analyzer.analyze();
                    analyzer_calls++;

                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    last_analysis_time++;

                    if (last_analysis_time == MIN_ANALYZE_INTERVAL)
                        last_analysis_time = 0;
                }
            }
            });

        // Wait for all threads to finish
        thread1.join();
        thread2.join();
        thread3.join();
        thread4.join();
        thread5.join();

        // Print the number of analyzer function calls
        std::cout << "Analyzer function called " << analyzer_calls << " times." << std::endl;

        return 0;
}


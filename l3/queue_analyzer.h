#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <deque>
#include <mutex>

namespace fs = std::filesystem;

template <typename T>
class MessageQueue;

template <typename T>
class QueueAnalyzer {
public:
    explicit QueueAnalyzer(MessageQueue<T>& message_queue, const std::string& filename)
        : message_queue_(message_queue), filename_(filename) {}

    void analyze() const {
        std::lock_guard<std::mutex> lock(mutex_);

        std::ofstream outfile("C:/Users/user/Desktop/C++/CHI-labs/Project/queue_analysis.txt", std::ios::app);
        if (!outfile.is_open()) {
            std::cerr << "Error: Could not open file " << filename_ << std::endl;
            return;
        }

        // Save current time
        const auto current_time = std::chrono::system_clock::now();

        // Calculate size of queue
        const size_t queue_size = message_queue_.size();

        // Calculate percentage of messages with different urgency levels
        std::vector<size_t> urgency_counts(3);
        for (const auto& message : message_queue_.messages_) {
            if (message.urgency_level == 1/*Message<T>::HIGH*/) {
                ++urgency_counts[0];
            }
            else if (message.urgency_level == 2/*Message<T>::MEDIUM*/) {
                ++urgency_counts[1];
            }
            else if (message.urgency_level == 3/*Message<T>::LOW*/) {
                ++urgency_counts[2];
            }
        }
        std::vector<double> urgency_percentages(3);
        std::transform(urgency_counts.begin(), urgency_counts.end(), urgency_percentages.begin(),
            [queue_size](const size_t count) { return static_cast<double>(count) / queue_size * 100; });

        // Calculate total size of queue in kilobytes
        const double queue_size_kb = static_cast<double>(sizeof(T) * queue_size) / 1024;

        // Calculate maximum difference in expiration times
        std::chrono::duration<double> max_difference{};
        for (auto i = message_queue_.messages_.begin(); i != message_queue_.messages_.end(); ++i) {
            for (auto j = std::next(i); j != message_queue_.messages_.end(); ++j) {
                auto difference = j->expire_time - i->expire_time;
                if (difference > max_difference) {
                    max_difference = difference;
                }
            }
        }

        // Get current time
        auto time_t = std::chrono::system_clock::to_time_t(current_time);
        std::tm timeinfo;
        errno_t err = localtime_s(&timeinfo, &time_t);
        if (err != 0) {
            // handle error
        }
        
        // Write analysis results to file
        outfile << "Analysis results at " << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << ":\n";
        outfile << "Queue size: " << queue_size << " messages\n";
        
        // Write urgency analysis results to file
        outfile << "Urgency analysis:\n";
        outfile << "  Low urgency messages: " << std::fixed << std::setprecision(2) << urgency_percentages[0] << "%\n";
        outfile << "  Medium urgency messages: " << std::fixed << std::setprecision(2) << urgency_percentages[1] << "%\n";
        outfile << "  High urgency messages: " << std::fixed << std::setprecision(2) << urgency_percentages[2] << "%\n";

        outfile << "Total queue size: " << std::fixed << std::setprecision(2) << queue_size_kb << " KB\n";
        outfile << "Maximum difference in expiration times: " << std::fixed << std::setprecision(2) << max_difference.count() << " seconds\n\n";
    }

private:
    MessageQueue<T>& message_queue_;
    const std::string& filename_;
    mutable std::mutex mutex_;
};

#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <deque>
#include <mutex>
#include <thread>

template <typename T>
struct Message {
    std::chrono::time_point<std::chrono::system_clock> expire_time;
    int urgency_level;
    T message;

    /*enum class urgencyLevel
    {
        LOW,
        MEDIUM,
        HIGH
    };*/

    Message(const std::chrono::time_point<std::chrono::system_clock>& expire_time, const int& urgency_level, const T& message)
        : expire_time(expire_time), message(message) {
        if (urgency_level < 1 || urgency_level > 3) {
            throw std::invalid_argument("Invalid urgency level");
        }
        this->urgency_level = urgency_level;
    }

    bool is_expired() const {
        return std::chrono::system_clock::now() > expire_time;
    }

    // The operator< function compares two messages by their urgency level and expiration time.
    bool operator<(const Message& other) const {
        if (urgency_level == other.urgency_level) {
            return expire_time < other.expire_time;
        }
        return urgency_level < other.urgency_level;
    }
};

template <typename T>
class MessageQueue {
public:
    explicit MessageQueue(const size_t max_size) : max_size_(max_size) {}

    void push(const Message<T>& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (messages_.size() >= max_size_) {
            remove_expired_messages();
            if (messages_.size() >= max_size_) {
                throw std::runtime_error("Message queue is full");
            }
        }
        insert_message(message);
    }

    Message<T> pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!messages_.empty() && messages_.front().is_expired()) {
            messages_.pop_front();
        }
        if (messages_.empty()) {
            throw std::runtime_error("Message queue is empty");
        }
        Message<T> message = messages_.front();
        messages_.pop_front();
        return message;
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(mutex_);
        //remove_expired_messages();
        return messages_.size();
    }

private:
    size_t max_size_;
    std::deque<Message<T>> messages_;
    mutable std::mutex mutex_;

    // The insert_message function inserts a message into the queue in the correct priority order
    void insert_message(const Message<T>& message) {
        auto it = std::upper_bound(messages_.begin(), messages_.end(), message);
        messages_.insert(it, message);
    }

    void remove_expired_messages() {
        messages_.erase(std::remove_if(messages_.begin(), messages_.end(),
            [](const Message<T>& message) { return message.is_expired(); }),
            messages_.end());
    }

    template <typename T>
    friend class QueueAnalyzer;
};

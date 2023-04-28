#include "message_queue.h"
#include "queue_analyzer.h"

using namespace std;

int main() {
    try {
        // Create a message queue with a maximum size of 5
        MessageQueue<string> queue(5);

        // Create a QueueAnalyzer to periodically analyze the message queue
        QueueAnalyzer<string> analyzer(queue, "C:/Users/user/Desktop/C++/CHI-labs/Project/queue_analysis.txt");

        // Add messages to the queue
        queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(5), 1, "High priority message 1"));
        queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(10), 2, "Medium priority message 2"));
        queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(15), 3, "Low priority message 3"));
        queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(20), 1, "High priority message 4"));
        queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(25), 2, "Medium priority message 5"));

        // Analyze the queue
        analyzer.analyze();

        // Wait for some time
        this_thread::sleep_for(chrono::seconds(10));

        queue.pop();

        // This message will cause a invalid_argument, because we specified an incorrect urgency level
        //queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(30), 4, "Low priority message 2"));

        // Add more messages to the queue
        queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(30), 3, "Low priority message 6"));
        queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(35), 2, "Medium priority message 7"));
        queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(25), 1, "High priority message 8"));

        // This message will cause a runtime_error, as the queue is already full
        //queue.push(Message<string>(chrono::system_clock::now() + chrono::seconds(35), 1, "Medium priority message 3"));

        // Analyze the queue again
        analyzer.analyze();
    }
    catch (const exception& ex) {
        cerr << "An exception has occurred: " << ex.what() << endl;
        return -1;
    }

    return 0;
}


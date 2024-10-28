#include "actions.h"
#include <iostream>
#include <chrono>
#include "thread_pool.h"

void show_loading_bar(int duration_ms, const std::string& action_name) {
    int total_steps = 10;
    int step_duration = duration_ms / total_steps;

    for (int i = 0; i < total_steps; ++i) {
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < step_duration) {
            // Busy-wait loop
        }
    }
}

void wait(int duration_ms, const std::string& action_name) {
    std::cout << "\n[" << action_name << "] Processing...\n";

    auto total_start = std::chrono::steady_clock::now();

    int total_steps = 10;
    int step_duration = duration_ms / total_steps;

    for (int i = 0; i < total_steps; ++i) {
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < step_duration) {
            // Busy-wait loop
        }
    }

    auto total_end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count();

    std::cout << "[" << action_name << "] Completed in " << elapsed << " ms.\n" << std::endl;
}


bool has_received_message(const std::vector<std::string>& topics, const std::vector<std::string>& messages, const std::string& target_topic, const std::string& target_message) {
    for (size_t i = 0; i < topics.size(); ++i) {
        if (topics[i] == target_topic && messages[i] == target_message) {
            return true;
        }
    }
    return false;
}

Action1::Action1() {}

void Action1::action_function(const std::string& topicName, const std::string& message) {
    std::cout << "\n[Action1] Received message on topic '" << topicName << "': " << message << std::endl;
    wait(1000, "Action 1");
    this->publish("Start", "msg1");
    std::cout << "[Action1] Published 'msg1' on topic 'Start'\n" << std::endl;
}

Action2::Action2() {}

void Action2::action_function(const std::string& topicName, const std::string& message) {
    if (has_received_message(message_topics, messages, "Start", "msg1")) {
        std::cout << "\n[Action2] Received message on topic '" << topicName << "': " << message << std::endl;
        wait(1500, "Action 2");

        std::cout << "[Action2] Publishing 'msg2' asynchronously on topic 'Async'\n";
        this->publish("Async", "msg2");
    } else {
        std::cout << "[Action2] Waiting for 'msg1' on topic 'Start' before executing.\n" << std::endl;
    }
}

Action3::Action3() {}

void Action3::action_function(const std::string& topicName, const std::string& message) {
    if (has_received_message(message_topics, messages, "Start", "msg1")) {
        std::cout << "\n[Action3] Received message on topic '" << topicName << "': " << message << std::endl;
        wait(5000, "Action 3");

        std::cout << "[Action3] Publishing 'msg3' asynchronously on topic 'Async'\n";
        this->publish("Async", "msg3");
    } else {
        std::cout << "[Action3] Waiting for 'msg1' on topic 'Start' before executing.\n" << std::endl;
    }
}

Action4::Action4() {}

void Action4::action_function(const std::string& topicName, const std::string& message) {
    std::cout << "\n[Action4] Received message on topic '" << topicName << "': " << message << std::endl;
    if (has_received_message(message_topics, messages, "Async", "msg2") &&
        has_received_message(message_topics, messages, "Async", "msg3")) {
        wait(2500, "Action 4");
        std::cout << "[Action4] Action complete. All required messages received on 'Async'.\n" << std::endl;
    } else {
        std::cout << "[Action4] Waiting for both 'msg2' and 'msg3' on topic 'Async' before executing.\n" << std::endl;
    }
}

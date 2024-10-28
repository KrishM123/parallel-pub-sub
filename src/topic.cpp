#include "topic.h"
#include <iostream>
#include <algorithm>

Topic::Topic(const std::string& topicName)
    : name(topicName), pub_size(0), sub_size(0) {}

void Topic::add_publisher(PubSub* action) {
    if (!action) {
        std::cerr << "Error: Cannot add a null publisher." << std::endl;
        return;
    }

    if (!contains_publisher(action)) {
        publishers.push_back(action);
        ++pub_size;
    } else {
        std::cerr << "Error: Publisher already exists for topic '" << name << "'." << std::endl;
    }
}

void Topic::remove_publisher(PubSub* action) {
    if (!action) {
        std::cerr << "Error: Cannot remove a null publisher." << std::endl;
        return;
    }

    auto it = std::find(publishers.begin(), publishers.end(), action);
    if (it != publishers.end()) {
        publishers.erase(it);
        --pub_size;
    } else {
        std::cerr << "Error: Publisher not found for topic '" << name << "'." << std::endl;
    }
}

void Topic::add_subscriber(PubSub* action) {
    if (!action) {
        std::cerr << "Error: Cannot add a null subscriber." << std::endl;
        return;
    }

    if (!contains_subscriber(action)) {
        subscribers.push_back(action);
        ++sub_size;
    } else {
        std::cerr << "Error: Subscriber already exists for topic '" << name << "'." << std::endl;
    }
}

void Topic::remove_subscriber(PubSub* action) {
    if (!action) {
        std::cerr << "Error: Cannot remove a null subscriber." << std::endl;
        return;
    }

    auto it = std::find(subscribers.begin(), subscribers.end(), action);
    if (it != subscribers.end()) {
        subscribers.erase(it);
        --sub_size;
    } else {
        std::cerr << "Error: Subscriber not found for topic '" << name << "'." << std::endl;
    }
}

bool Topic::contains_publisher(PubSub* action) const {
    return action && std::find(publishers.begin(), publishers.end(), action) != publishers.end();
}

bool Topic::contains_subscriber(PubSub* action) const {
    return action && std::find(subscribers.begin(), subscribers.end(), action) != subscribers.end();
}

void Topic::publish(const std::string& message) {
    if (subscribers.empty()) {
        std::cerr << "Warning: Publishing to topic '" << name << "' with no subscribers." << std::endl;
    }
    
    messages.push_back(message);
    for (size_t i = 0; i < subscribers.size(); ++i) {
        if (subscribers[i]) {
            subscribers[i]->update(name, message);
        } else {
            std::cerr << "Warning: Null subscriber in topic '" << name << "'." << std::endl;
        }
    }
}

std::vector<std::string> Topic::get_messages() const {
    return messages;
}

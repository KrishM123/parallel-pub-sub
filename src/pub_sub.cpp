#include "pub_sub.h"
#include "message_broker.h"
#include "thread_pool.h"
#include <algorithm>
#include <iostream>

PubSub::PubSub() {}

void PubSub::add_publisher(const std::string& topicName) {
    MessageBroker& broker = MessageBroker::get_instance();
    
    if (!broker.valid_topic(topicName)) {
        std::cerr << "Error: Topic '" << topicName << "' does not exist." << std::endl;
        return;
    }

    if (std::find(publishing.begin(), publishing.end(), topicName) != publishing.end()) {
        std::cerr << "Error: Already a publisher for topic '" << topicName << "'." << std::endl;
        return;
    }

    publishing.push_back(topicName);
    broker.add_publisher(this, topicName);
}

void PubSub::remove_publisher(const std::string& topicName) {
    MessageBroker& broker = MessageBroker::get_instance();
    
    auto it = std::find(publishing.begin(), publishing.end(), topicName);
    if (it == publishing.end()) {
        std::cerr << "Error: Not a publisher for topic '" << topicName << "'." << std::endl;
        return;
    }

    publishing.erase(it);
    broker.remove_publisher(this, topicName);
}

void PubSub::add_subscriber(const std::string& topicName) {
    MessageBroker& broker = MessageBroker::get_instance();
    
    if (!broker.valid_topic(topicName)) {
        std::cerr << "Error: Topic '" << topicName << "' does not exist." << std::endl;
        return;
    }

    if (std::find(subscribed.begin(), subscribed.end(), topicName) != subscribed.end()) {
        std::cerr << "Error: Already subscribed to topic '" << topicName << "'." << std::endl;
        return;
    }

    subscribed.push_back(topicName);
    broker.add_subscriber(this, topicName);
}

void PubSub::remove_subscriber(const std::string& topicName) {
    MessageBroker& broker = MessageBroker::get_instance();
    
    auto it = std::find(subscribed.begin(), subscribed.end(), topicName);
    if (it == subscribed.end()) {
        std::cerr << "Error: Not subscribed to topic '" << topicName << "'." << std::endl;
        return;
    }

    subscribed.erase(it);
    broker.remove_subscriber(this, topicName);
}

void PubSub::publish(const std::string& topicName, const std::string& message) {
    MessageBroker& broker = MessageBroker::get_instance();
    
    if (std::find(publishing.begin(), publishing.end(), topicName) == publishing.end()) {
        std::cerr << "Error: Not a publisher for topic '" << topicName << "'." << std::endl;
        return;
    }

    broker.publish(topicName, this, message);
}

void PubSub::update(const std::string& topicName, const std::string& message) {
    messages.push_back(message);
    message_topics.push_back(topicName);

    ThreadPool::getInstance().enqueue([=]() {
        action_function(topicName, message);
    });
}

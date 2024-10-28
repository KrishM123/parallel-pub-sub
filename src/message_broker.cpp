#include <iostream>
#include "message_broker.h"
#include "pub_sub.h"

MessageBroker& MessageBroker::get_instance() {
    static MessageBroker instance;
    return instance;
}

void MessageBroker::new_topic(const std::string& name) {
    if (name.empty()) {
        std::cerr << "Error: Topic name cannot be empty." << std::endl;
        return;
    }

    if (!valid_topic(name)) {
        topics[name] = new Topic(name);
    } else {
        std::cerr << "Error: Topic '" << name << "' already exists." << std::endl;
    }
}

void MessageBroker::add_publisher(PubSub* action, const std::string& topicName) {
    if (!action || !valid_topic(topicName)) {
        std::cerr << (action ? "Error: Topic '" + topicName + "' does not exist." : "Error: Cannot add a null publisher.") << std::endl;
        return;
    }

    Topic* topic = topics[topicName];
    if (!topic->contains_publisher(action)) {
        topic->add_publisher(action);
    } else {
        std::cerr << "Error: Publisher already exists for topic '" << topicName << "'." << std::endl;
    }
}

void MessageBroker::add_subscriber(PubSub* action, const std::string& topicName) {
    if (!action || !valid_topic(topicName)) {
        std::cerr << (action ? "Error: Topic '" + topicName + "' does not exist." : "Error: Cannot add a null subscriber.") << std::endl;
        return;
    }

    Topic* topic = topics[topicName];
    if (!topic->contains_subscriber(action)) {
        topic->add_subscriber(action);
    } else {
        std::cerr << "Error: Subscriber already exists for topic '" << topicName << "'." << std::endl;
    }
}

void MessageBroker::remove_publisher(PubSub* action, const std::string& topicName) {
    if (!action || !valid_topic(topicName)) {
        std::cerr << (action ? "Error: Topic '" + topicName + "' does not exist." : "Error: Cannot remove a null publisher.") << std::endl;
        return;
    }

    Topic* topic = topics[topicName];
    if (topic->contains_publisher(action)) {
        topic->remove_publisher(action);
    } else {
        std::cerr << "Error: Publisher not found for topic '" << topicName << "'." << std::endl;
    }
}

void MessageBroker::remove_subscriber(PubSub* action, const std::string& topicName) {
    if (!action || !valid_topic(topicName)) {
        std::cerr << (action ? "Error: Topic '" + topicName + "' does not exist." : "Error: Cannot remove a null subscriber.") << std::endl;
        return;
    }

    Topic* topic = topics[topicName];
    if (topic->contains_subscriber(action)) {
        topic->remove_subscriber(action);
    } else {
        std::cerr << "Error: Subscriber not found for topic '" << topicName << "'." << std::endl;
    }
}

void MessageBroker::publish(const std::string& topicName, PubSub* publisher, const std::string& message) {
    if (!publisher || !valid_topic(topicName)) {
        std::cerr << (publisher ? "Error: Topic '" + topicName + "' does not exist." : "Error: Cannot publish with a null publisher.") << std::endl;
        return;
    }
    
    Topic* topic = topics[topicName];
    if (topic->contains_publisher(publisher)) {
        topic->publish(message);
    } else {
        std::cerr << "Error: Publisher not registered for topic '" << topicName << "'." << std::endl;
    }
}

bool MessageBroker::valid_topic(const std::string& topicName) const {
    return topics.find(topicName) != topics.end();
}

std::vector<std::string> MessageBroker::get_messages(const std::string& topicName) const {
    if (valid_topic(topicName)) {
        return topics.at(topicName)->get_messages();
    } else {
        std::cerr << "Error: Topic '" << topicName << "' does not exist." << std::endl;
    }
    return {};
}

#ifndef MESSAGE_BROKER_H
#define MESSAGE_BROKER_H

#include <string>
#include <unordered_map>
#include "topic.h"

class PubSub;
class Topic;

class MessageBroker {
public:
    static MessageBroker& get_instance();

    void new_topic(const std::string& name);

    void add_publisher(PubSub* action, const std::string& topicName);
    void add_subscriber(PubSub* action, const std::string& topicName);

    void remove_publisher(PubSub* action, const std::string& topicName);
    void remove_subscriber(PubSub* action, const std::string& topicName);

    void publish(const std::string& topicName, PubSub* publisher, const std::string& message);

    bool valid_topic(const std::string& topicName) const;

    std::vector<std::string> get_messages(const std::string& topicName) const;

private:
    std::unordered_map<std::string, Topic*> topics;
};

#endif

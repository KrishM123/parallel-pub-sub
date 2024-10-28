#ifndef PUB_SUB_H
#define PUB_SUB_H

#include <string>
#include <vector>
#include <map>
#include "message_broker.h"

class MessageBroker;

class PubSub {
public:
    virtual ~PubSub() = default;

    void add_publisher(const std::string& topicName);
    void remove_publisher(const std::string& topicName);
    void add_subscriber(const std::string& topicName);
    void remove_subscriber(const std::string& topicName);

    void publish(const std::string& topicName, const std::string& message);

    void update(const std::string& topicName, const std::string& message);

    virtual void action_function(const std::string& topicName, const std::string& message) = 0;

protected:
    PubSub();

    std::vector<std::string> publishing;
    std::vector<std::string> subscribed;
    std::vector<std::string> message_topics;
    std::vector<std::string> messages;
};

#endif

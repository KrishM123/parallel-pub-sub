#ifndef TOPIC_H
#define TOPIC_H

#include <string>
#include <vector>
#include "pub_sub.h"

class PubSub;

class Topic {
public:
    explicit Topic(const std::string& topicName);

    void add_publisher(PubSub* action);
    void remove_publisher(PubSub* action);
    void add_subscriber(PubSub* action);
    void remove_subscriber(PubSub* action);

    bool contains_publisher(PubSub* action) const;
    bool contains_subscriber(PubSub* action) const;

    void publish(const std::string& message);

    std::vector<std::string> get_messages() const;

private:
    std::string name;
    std::vector<PubSub*> publishers;
    std::vector<PubSub*> subscribers;
    std::vector<std::string> messages;
    int pub_size;
    int sub_size;
};

#endif

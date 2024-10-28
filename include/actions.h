#ifndef ACTION_H
#define ACTION_H

#include "pub_sub.h"
#include <string>

class Action1 : public PubSub {
public:
    Action1();
protected:
    void action_function(const std::string& topicName, const std::string& message) override;
};

class Action2 : public PubSub {
public:
    Action2();
protected:
    void action_function(const std::string& topicName, const std::string& message) override;
};

class Action3 : public PubSub {
public:
    Action3();
protected:
    void action_function(const std::string& topicName, const std::string& message) override;
};

class Action4 : public PubSub {
public:
    Action4();
protected:
    void action_function(const std::string& topicName, const std::string& message) override;
};

#endif

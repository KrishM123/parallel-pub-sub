#include <iostream>
#include <string>
#include "include/message_broker.h"
#include "include/actions.h"
#include "include/thread_pool.h"

int main() {
    ThreadPool& pool = ThreadPool::getInstance(5);
    MessageBroker& broker = MessageBroker::get_instance();

    broker.new_topic("Start");
    broker.new_topic("Async");

    PubSub* action1 = new Action1();
    PubSub* action2 = new Action2();
    PubSub* action3 = new Action3();
    PubSub* action4 = new Action4();

    action1->add_publisher("Start");
    action2->add_subscriber("Start");
    action3->add_subscriber("Start");
    
    action2->add_publisher("Async");
    action3->add_publisher("Async");
    action4->add_subscriber("Async");

    std::string command;
    bool running = true;

    std::cout << "Type 'start' to publish the first message, or 'end' to quit." << std::endl;
    std::cout << "> ";

    while (running) {
        std::getline(std::cin, command);

        if (command == "start") {
            action1->action_function("Start", "Start message not actually in the topic");
        } else if (command == "end") {
            running = false;
        } else {
            std::cout << "Unknown command. Please type 'start' or 'end'." << std::endl;
        }
    }

    pool.waitForAll();

    delete action1;
    delete action2;
    delete action3;
    delete action4;

    return 0;
}

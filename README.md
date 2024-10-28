# Pub/Sub Design Pattern

![Group 1.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/18972c3f-cc50-470d-b8d1-5cb369efa733/fe108290-c163-49ba-bc9b-029481bad31b/Group_1.png)

## Structure

- `message_broker.cpp` will handle all of the topics
- `pub_sub.cpp` will have a PubSub parent class for action functions
- `actions.cpp` defines every action as a PubSub type
- `topic.cpp` defines topics

## PubSub

### Attributes

- List of topics publishing to: `vector<string> publishing`
- List of topics subscribed to: `vector<string> subscribed`
- List of messages (protected): `vector<string> messages`
- Chronological list of topics (protected): `vector<string> message_topics`

### Methods

- Constructor: `PubSub()`
- Add/remove publisher/subscriber: `void [add/remove]_[publisher/subscriber](string topicName)`
    - These are 4 different functions
    - Checks if topic exists using `MessageBroker.valid_topic(topicName)`
    - If topic exists, calls `MessageBroker.add_[publisher/subscriber](this, topicName)`
- Publish message: `void publish(string message)`
    - For every `topicName` in `publishing` list, call `MessageBroker.publish(topicName, message)`
- Recieved message from subscription: `void update(string topicName, string message)`
    - Adds seen message to `message` and calls `this->action_function`
- Abstract action_function: actions that are of type PubSub rewrite this function

## MessageBroker

### Attributes

- Map of name and topic: `unordered_map<string, Topic*> topics`

### Methods

- Constructor: it is global and static
- New topic: `void new_topic(string name)`
    - Initializes new topic on heap
    - Adds name and topic pointer to `topics`
- Add publisher: `void add_[publisher/subscriber](PubSub* action, string topicName)`
    - These are 2 different functions
    - Using `topics` map, finds topic with `topicName`
    - With the pointer of topics associated with `topicName`, check if the topic contains the action using `topic->contains[publisher/subscriber](action)`
        - If it does not contain, then calls `topic->add_[publisher/subscriber](action)`
- Remove publisher: `void remove_[publisher/subscriber](PubSub* action, string topicName)`
    - These are 2 different functions
    - Using `topics` map, finds topic with `topicName`
    - With the pointer of topics associated with `topicName`, check if the topic contains the action using `topic->contains[publisher/subscriber](action)`
        - If it contains, then calls `topic->add_[publisher/subscriber](action)`
- Publish message: `void publish(string topicName, string message)`
    - Using the `topics` map, gets the topic pointer associated with `topicName`
    - Calls `topic->publish(message)`
- Valid topic: `bool valid_topic(string topicName)`
    - Returns true if `topics` map contains the string `topicName`
- Get latest message: `vector<string> get_messages(string topicName)`
    - Using the `topics` map, gets the pointer to the subscribed topic
    - Calls `topic->get_messages`

## Topic

### Attributes

- List of publishers: `vector<PubSub*> publishers`
- List of subscribers: `vector<PubSub*> subscribers`
- List of messages:  `vector<string> messages`
- Num of publishers: `int pub_size`
- Num of subscribers: `int sub_size`
- Name of topic: `string name`

### Methods

- Constructor: `Topic(string name)`
- Add or remove publisher or subscriber: `void [add/remove]_[publisher/subscriber](PubSub* action)`
    - These are 4 different functions
    - Adds/removes pointer from the `publishers/subscribers` list
    - Increments or decrements pub or sub size
- Contains publisher or subscriber: `bool contains_[publisher/subscriber](PubSub* action)`
    - These are two different functions
    - Returns true if the `publishers/subscribers`list contains the action given, else false
- Publish message: `void publish(string message)`
    - Adds new message to `messages`
    - For all `sub` in `subscribers`, call `sub->update(name, message)`
- Get messages: `vector<string> get_messages()`

# Threadpool

## Structure

- A singleton threadpool will be created for easy concurrency management
- A list of pending tasks (action functions)`queue<function<void()>> tasks` will be used to allocate tasks to threads
- A list of workers `vector<thread> workers` that will monitor `tasks` and execute a function when found
- A mutex `mutex queueMutex` for allowing access to `tasks` to only one thread at a time
- A conditional variable `conditional_variable condition` that wakes threads when a new task is added in `task`

## Functions

- Constructor (singleton) will initialize the threads specified and instruct them to monitor `tasks`
- `enque(function<void()> task)` takes in any lambda function, and adds it to tasks queue
- `waitForAll()` this function can be used to synchronozie. All tasks in queue will be finished
- Destructor will safely join all joinable threads

## Optimizations

- Use of emplace_back when adding worker threads
- Use of emplace(move()) when adding functions to queue
- Use of condition.notify_one instead of notify_all when adding functions to queue

### Safety

- Use of scoped block and unique_lock
- Joining threads when destructing
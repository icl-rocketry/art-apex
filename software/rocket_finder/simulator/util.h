#pragma once

using namespace std;

// Wrap the message with a time, so we can work out a good delivery schedule
// We have a recepient for each message, because a message can be delivered to different people at the same time
template <typename MSG>
struct TimedMessage {
    uint64_t time;
    MSG msg;
    uint8_t recepient;
};

// This is just needed to compare 2 different timed messages
template <typename MSG>
struct compare {
    bool operator()(const TimedMessage<MSG>& a, const TimedMessage<MSG>& b) {
        return a.time > b.time;
    }
};

template <typename MSG>
string to_string(MSG msg) {
    // TODO - fill this in for pretty printing
    return "Rick Roll";
}

template <typename MSG>
void log(TimedMessage<MSG> msg) {
    std::cout << "{\"time\": " << msg.time << ", \"recepient\": " << static_cast<int>(msg.recepient) << ", \"msg\": " << to_string(msg.msg) << "}" << std::endl ;
}
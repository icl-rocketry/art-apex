#pragma once
#include <cstdlib>

using namespace std;


// Gets you a number between 0 and 1 inclusive (technically not a pdf but shouldn't be a problem)
float rand_uniform() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

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
void log(TimedMessage<MSG> msg) {
    std::cout << "{\"time\": " << msg.time << ", \"recepient\": " << static_cast<int>(msg.recepient) << ", \"msg\": " << to_string(msg.msg) << "}" << std::endl ;
}

template <typename MSG>
void log(uint64_t time, uint8_t sender, MSG msg) {
    std::cout << "{\"time\": " << time << ", \"sender\": " << static_cast<int>(sender) << ", \"msg\": " << to_string(msg) << "}" << std::endl ;
}

template <typename MSG>
int corrupt(MSG* msg, float p_corruption) {
    int n = 0;
    char* ptr = (char*)msg;
    for (int i = 0; i < sizeof(MSG); i++) {
        for (int j = 0; j < 8; j++) { // 8 bits to a byte
            if (rand_uniform() < p_corruption) {
                ptr[i] ^= 1 << j; //Flip the jth bit in the ith byte
                n++;
            }
        }
    }
    return n;
}

template <typename MSG>
void collide(MSG* msg) {
    char* ptr = (char*)msg;
    for (int i = 0; i < sizeof(MSG); i++) {
        uint8_t byte = static_cast<unsigned char>(rand() % 0xff);
        ptr[i] = byte;
    }
}
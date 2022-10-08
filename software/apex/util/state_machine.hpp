#pragma once

#include "Adafruit.h"
#include "neopixel.hpp"

typedef char* Error;

class AbstractState {
public:
    virtual AbstractState* run() = 0;
    virtual ~AbstractState() {};


    Error get_error() {
        return error;
    }

    Colour& get_colour() {
        return colour;
    }

private:
    Colour colour;
    Error error;
};


class StateMachine {
public:
    StateMachine(AbstractState* initial_state): state(initial_state) {}
    
    void run() {
        Colour error_colour(255, 0, 0);

        while (state != nullptr) {
            show_colour(state->get_colour());
            AbstractState* new_state = state->run();

            Error err = state->get_error();
            if (err != nullptr) {
                show_colour(error_colour);
                while (true) {
                    Serial.println(err);
                    delay(100);
                }
            }

            delete state;
            state = new_state;
        }
    }
    
private:
    AbstractState* state;
};
#pragma once

#include <string>

class Button
{
    private:
    std::string connector;
    int pin;
    bool was_pressed;
    bool previous_state;
public:
    explicit Button(std::string connector, int pin, bool set_interrupt);
    bool GetIsPressed();
    bool GetWasPressed();
    void Pressed();
    void Tick();
};
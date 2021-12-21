#pragma once

#include <string>

class IndicatorLED
{
private:
    std::string connector;
    int pin;
    bool Set(std::string state);

public:
    explicit IndicatorLED(std::string connector, int pin);
    bool On();
    bool Off();
};
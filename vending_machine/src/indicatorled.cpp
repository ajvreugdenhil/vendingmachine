#include <fstream>
#include <iostream>
#include "indicatorled.h"

IndicatorLED::IndicatorLED(std::string connector, int pin) : connector(connector), pin(pin)
{
    const int msg_max_len = 120;
    char msg[msg_max_len] = "";
    snprintf(msg, msg_max_len, "%s %d 0 0", this->connector.c_str(), this->pin);

    std::fstream my_file;
    my_file.open("/sys/kernel/ES6/gpio", std::ios::out);
    if (!my_file)
    {
        std::cout << "ERROR: could not set gpio direction" << std::endl;
        my_file.close();
        return;
    }
    else
    {
        my_file << msg;
    }
    my_file.close();
}

bool IndicatorLED::Set(std::string state)
{
    std::fstream my_file;
    const int max_file_len = 120;
    char filename[max_file_len] = "";
    snprintf(filename, max_file_len, "/dev/%s_%d", this->connector.c_str(), this->pin);
    my_file.open(filename, std::ios::out);
    if (!my_file)
    {
        std::cout << "ERROR: could not set indicator led state" << std::endl;
        my_file.close();
        return false;
    }
    else
    {
        my_file << state;
    }
    my_file.close();
    return true;
}

bool IndicatorLED::On()
{
    return Set("1");
}

bool IndicatorLED::Off()
{
    return Set("0");
}
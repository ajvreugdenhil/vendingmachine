#include <string>
#include <fstream>
#include <iostream>
#include "motor.h"

Motor::Motor(int pwm_nr) : pwm_nr(pwm_nr)
{
    if (pwm_nr == 1)
    {
        SetSetting("/dev/pwm1_duty", "50");
        SetSetting("/dev/pwm1_freq", "100");
    }
    else if (pwm_nr == 2)
    {
        SetSetting("/dev/pwm2_duty", "50");
        SetSetting("/dev/pwm2_freq", "100");
    }
}

bool Motor::SetSetting(std::string filename, std::string value)
{
    std::fstream my_file;
    my_file.open(filename, std::ios::out);
    if (!my_file)
    {
        std::cout << "ERROR: could not set motor setting" << std::endl;
        my_file.close();
        return false;
    }
    else
    {
        my_file << value;
    }
    my_file.close();
    return true;
}

bool Motor::On()
{
    if (this->pwm_nr == 1)
    {
        return SetSetting("/dev/pwm1_en", "1");
    }
    return false;
}

bool Motor::Off()
{
    if (this->pwm_nr == 1)
    {
        return SetSetting("/dev/pwm1_en", "0");
    }
    return false;
}
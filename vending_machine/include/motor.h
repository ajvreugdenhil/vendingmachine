#pragma once

class Motor
{
private:
    int pwm_nr;
    bool SetSetting(std::string filename, std::string value);

public:
    explicit Motor(int pwm_nr);
    bool On();
    bool Off();
};
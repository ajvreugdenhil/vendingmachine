#pragma once

#include <string>

class Display
{
private:
    std::string filename;

public:
    explicit Display(std::string filename);
    bool SetMessage(std::string message);
};
#include <string>
#include <fstream>
#include <iostream>
#include "display.h"


Display::Display(std::string filename) : filename(filename)
{
}

bool Display::SetMessage(std::string message)
{
    std::cout << "display: '" << message << "'" << std::endl;
    std::fstream my_file;
    my_file.open(this->filename, std::ios::out);
    my_file.clear();
    my_file.seekg(0);
    if (!my_file)
    {
        return false;
    }
    else
    {
        my_file << message;
    }
    my_file.close();
    return true;
}
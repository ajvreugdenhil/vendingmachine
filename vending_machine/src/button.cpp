#include <string>
#include <iostream>
#include <fstream>
#include "button.h"

Button::Button(std::string connector, int pin, bool set_interrupt) : connector(connector), pin(pin)
{

    const int msg_max_len = 120;
    char msg[msg_max_len] = "";
    snprintf(msg, msg_max_len, "%s %d 1 %d", this->connector.c_str(), this->pin, set_interrupt);

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

    this->previous_state = this->GetIsPressed();
    this->was_pressed = this->previous_state;
}

bool Button::GetIsPressed()
{
    std::fstream my_file;
    std::string state;
    const int max_file_len = 120;
    char filename[max_file_len] = "";
    snprintf(filename, max_file_len, "/dev/%s_%d", this->connector.c_str(), this->pin);
    
    my_file.open(filename, std::ios::in);
    my_file.clear();
    my_file.seekg(0);
    if (!my_file)
    {
        std::cout << "ERROR: could not read button " << this->connector << " " << this->pin << " state" << std::endl;
        return false;
    }
    else
    {
        my_file >> state;
    }
    my_file.close();
    
    char c;
    if (state.length() != 1)
    {
        std::cout << "Invalid button state stringlen: " << state.length() << ": '" << state << "'" << std::endl;
        return false;
    }
    else
    {
        c = state[0];
    }

    // If state is '0' then the button is pressed
    if (c == '0')
    {
        return true;
    }
    else if (c == '1')
    {
        return false;
    }
    else
    {
        std::cout << "ERROR: invalid state read from button " << this->connector << " " << this->pin << ": " << (int)c << std::endl;
        return false;
    }
}

bool Button::GetWasPressed()
{
    bool result = this->was_pressed;
    this->was_pressed = false;
    return result;
}

void Button::Pressed()
{
    this->was_pressed = true;
}

void Button::Tick()
{
    if ((previous_state != GetIsPressed()) && GetIsPressed())
    {
        Pressed();
    }
    previous_state = GetIsPressed();
}
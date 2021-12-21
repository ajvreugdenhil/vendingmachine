#pragma once

#include "motor.h"
#include "button.h"
#include "display.h"
#include "indicatorled.h"
#include "paymentprovider.h"
#include "eventgenerator.h"
#include "states.h"
#include "timer.h"

class VendingMachine
{
private:
    Motor *motor;
    Display *display;
    PaymentProvider *payment_provider;
    Button **product_buttons;
    int product_button_count;
    EventGenerator *event_generator;
    State state;
    Timer *timer;
    IndicatorLED *indicatorled;
    int user_choice;

public:
    VendingMachine(Motor *motor,
                   Display *display,
                   PaymentProvider *pprovider,
                   Button **product_buttons,
                   int product_button_count,
                   EventGenerator *event_generator,
                   Timer *timer,
                   IndicatorLED *indicatorled);
    void Tick();

    void state_idle_entry();
    void state_showing_error_entry();
    void state_awaiting_payment_entry();
    void state_awaiting_payment_exit();
    void state_dispensing_entry();
    void state_dispensing_exit();
};
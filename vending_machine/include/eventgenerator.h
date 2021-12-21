#pragma once

#include <deque>
#include "events.h"
#include "button.h"
#include "paymentprovider.h"
#include "timer.h"

class EventGenerator
{
private:
    Button **product_choice_buttons;
    int product_choice_button_count;
    PaymentProvider *payment_provider;
    Timer *timer;
    std::deque<Event> *event_queue;
    void HandleSignal(int signum);

public:
    EventGenerator(Button **b, int button_count, PaymentProvider *p, Timer *t);
    Event GetLastEvent();
    Event PeekLastEvent();
    void AddEvent(Event event);
    void Tick();
    ~EventGenerator();
};
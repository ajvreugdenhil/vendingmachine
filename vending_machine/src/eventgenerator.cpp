#include "eventgenerator.h"
#include "events.h"
#include "buttonevents.h"
#include <deque>

//debug
//#include <iostream>

EventGenerator::EventGenerator(Button **b, int button_count, PaymentProvider *p, Timer *timer) : product_choice_buttons(b),
                                                                                                 product_choice_button_count(button_count),
                                                                                                 payment_provider(p),
                                                                                                 timer(timer)
{
    this->event_queue = new std::deque<Event>;
}

Event EventGenerator::PeekLastEvent()
{
    Event result;
    if (!this->event_queue->empty())
    {
        //std::cout << "len: " << this->event_queue->size() << std::endl;
        //std::cout << "val: " << (int)this->event_queue->front() << std::endl;
        result = this->event_queue->front();
    }
    else
    {
        result = NONE;
    }
    return result;
}

Event EventGenerator::GetLastEvent()
{
    Event e;
    e = this->PeekLastEvent();
    if (e != NONE)
    {
        this->event_queue->pop_front();
    }
    return e;
}

void EventGenerator::Tick()
{
    for (int i = 0; i < this->product_choice_button_count; i++)
    {
        this->product_choice_buttons[i]->Tick();
        if (this->product_choice_buttons[i]->GetWasPressed())
        {
            this->AddEvent(button_events[i]);
        }
    }

    this->payment_provider->Tick();
    if (this->payment_provider->GetPaymentSuccess())
    {
        this->AddEvent(PAYMENT_SUCCESS);
    }
    if (this->payment_provider->GetPaymentFailure())
    {
        this->AddEvent(PAYMENT_FAILED);
    }

    if (this->timer->GetTimedOut())
    {
        this->AddEvent(TIMER_TIMEOUT);
    }
}

EventGenerator::~EventGenerator()
{
    delete this->event_queue;
}

void EventGenerator::AddEvent(Event event)
{
    this->event_queue->push_back(event);
}
#include <iostream>
#include "vendingmachine.h"
#include "events.h"
#include "eventgenerator.h"

VendingMachine::VendingMachine(Motor *motor,
                               Display *display,
                               PaymentProvider *pprovider,
                               Button **product_buttons,
                               int product_button_count,
                               EventGenerator *event_generator,
                               Timer *timer,
                               IndicatorLED *indicatorled) : motor(motor),
                                                             display(display),
                                                             payment_provider(pprovider),
                                                             product_buttons(product_buttons),
                                                             product_button_count(product_button_count),
                                                             event_generator(event_generator),
                                                             timer(timer),
                                                             indicatorled(indicatorled)
{
    this->state = IDLE;
    user_choice = -1;
}

void VendingMachine::Tick()
{
    event_generator->Tick();
    Event last_event;
    do
    {
        last_event = this->event_generator->GetLastEvent();
        switch (this->state)
        {
        case IDLE:
            if (last_event == USER_CHOICE_BTN1_PRESSED)
            {
                user_choice = 1;
                state_awaiting_payment_entry();
                this->state = AWAITING_PAYMENT;
            }
            else if (last_event == USER_CHOICE_BTN2_PRESSED)
            {
                user_choice = 2;
                state_awaiting_payment_entry();
                this->state = AWAITING_PAYMENT;
            }
            else if (last_event == USER_CHOICE_BTN3_PRESSED)
            {
                user_choice = 3;
                state_awaiting_payment_entry();
                this->state = AWAITING_PAYMENT;
            }
            else if (last_event == USER_CHOICE_BTN4_PRESSED)
            {
                user_choice = 4;
                state_awaiting_payment_entry();
                this->state = AWAITING_PAYMENT;
            }
            break;
        case AWAITING_PAYMENT:
            if (last_event == PAYMENT_SUCCESS)
            {
                state_awaiting_payment_exit();
                state_dispensing_entry();
                this->state = DISPENSING;
            }
            else if (last_event == PAYMENT_FAILED)
            {
                state_awaiting_payment_exit();
                state_showing_error_entry();
                this->state = SHOWING_ERROR;
            }
            else if (last_event == TIMER_TIMEOUT)
            {
                state_awaiting_payment_exit();
                state_showing_error_entry();
                this->state = SHOWING_ERROR;
            }
            break;
        case DISPENSING:
            if (last_event == TIMER_TIMEOUT)
            {
                state_dispensing_exit();
                state_idle_entry();
                this->state = IDLE;
            }
            break;
        case SHOWING_ERROR:
            if (last_event == TIMER_TIMEOUT)
            {
                state_idle_entry();
                this->state = IDLE;
            }
            break;
        default:
            std::cout << "ERROR: Vending machine in invalid state" << std::endl;
            break;
        }
    } while (last_event != NONE);
}

void VendingMachine::state_idle_entry()
{
    this->display->SetMessage("select product   ");
}

void VendingMachine::state_showing_error_entry()
{
    this->display->SetMessage("payment failed");
    this->timer->Set(2);
}

void VendingMachine::state_awaiting_payment_entry()
{
    this->indicatorled->On();
    this->timer->Set(20);
    this->display->SetMessage("2 eur");
}

void VendingMachine::state_awaiting_payment_exit()
{
    this->indicatorled->Off();
}

void VendingMachine::state_dispensing_entry()
{
    this->display->SetMessage("Dispensing");

    // Sets motor long enough for the right beverage
    this->motor->On();
    this->timer->Set(user_choice * 3);
}

void VendingMachine::state_dispensing_exit()
{
    this->motor->Off();
}
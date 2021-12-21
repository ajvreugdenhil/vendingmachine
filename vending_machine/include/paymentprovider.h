#pragma once

#include "button.h"

class PaymentProvider
{
private:
    Button *btn_success;
    Button *btn_failure;

public:
    PaymentProvider(Button *btn_success, Button *btn_failure);
    void Tick();
    bool GetPaymentSuccess();
    bool GetPaymentFailure();
};
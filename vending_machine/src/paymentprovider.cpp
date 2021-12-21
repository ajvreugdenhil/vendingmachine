#include <string>
#include "paymentprovider.h"
#include "button.h"

PaymentProvider::PaymentProvider(Button *btn_success, Button *btn_failure) : btn_success(btn_success), btn_failure(btn_failure)
{
}

void PaymentProvider::Tick()
{
    btn_success->Tick();
    btn_failure->Tick();
}

bool PaymentProvider::GetPaymentSuccess()
{
    return (btn_success->GetWasPressed());
}

bool PaymentProvider::GetPaymentFailure()
{
    return (btn_failure->GetWasPressed());
}

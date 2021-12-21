#include <iostream>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "vendingmachine.h"
#include "button.h"
#include "display.h"
#include "indicatorled.h"
#include "motor.h"
#include "eventgenerator.h"
#include "events.h"
#include "timer.h"

#define REG_CURRENT_TASK _IOW('a', 'a', int32_t *)
#define SIG_GPIOCHANGE 44

EventGenerator *event_generator;

void HandleSignal(int signum)
{
  if (signum == SIGINT)
  {
    event_generator->AddEvent(SHUTDOWN);
  }
}

void signalFunction(int n, siginfo_t *info, void *unused)
{
  n = n;
  unused = unused;
  info = info;
  std::cout << "GPIO change signal received" << std::endl;
}

void subscribeToGPIOSignal()
{
  struct sigaction sig;
  sig.sa_sigaction = signalFunction;
  sig.sa_flags = SA_SIGINFO;
  sigaction(SIG_GPIOCHANGE, &sig, NULL);

  int32_t unused;
  int fd = open("/dev/gpio", O_RDWR);
  if (fd < 0)
  {
    std::cout << "could not open file" << std::endl;
    exit(1);
  }
  if (ioctl(fd, REG_CURRENT_TASK, (int32_t *)&unused) < 0)
  {
    std::cout << "ioctl failed: " << errno << std::endl;
    close(fd);
    exit(1);
  }
  close(fd);
}

int main()
{
  const int product_option_count = 4;
  Button *product_options[product_option_count] = {new Button("J3", 54, false),
                                                   new Button("J3", 46, true),
                                                   new Button("J3", 36, true),
                                                   new Button("J1", 24, true)};
  Button *payment_success_button = new Button("J3", 47, false);
  Button *payment_failure_button = new Button("J3", 48, false);
  IndicatorLED *indicatorled = new IndicatorLED("J3", 50);
  Motor *m = new Motor(1);
  Display *d = new Display("/dev/display");

  PaymentProvider *p = new PaymentProvider(payment_success_button, payment_failure_button);
  Timer *t = new Timer();
  event_generator = new EventGenerator(product_options, product_option_count, p, t);
  VendingMachine *v = new VendingMachine(m, d, p, product_options, product_option_count, event_generator, t, indicatorled);

  subscribeToGPIOSignal();
  signal(SIGINT, HandleSignal);
  while (event_generator->PeekLastEvent() != SHUTDOWN)
  {
    v->Tick();
    usleep(5000);
    //std::cout << "Main thread slept\n";
  }
  std::cout << "done\n";
  return 0;
}

#include "helloWorld.h"
#include <iostream>

HelloWorld::HelloWorld() {}

void HelloWorld::Run() { std::cout << "Hello World\n"; }

int HelloWorld::Square(int input) { return input * input; }
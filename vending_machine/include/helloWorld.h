#pragma once

#include "iHelloWorld.h"

class HelloWorld : public IHelloWorld{
public:
  HelloWorld();
  void Run();
  int Square(int input);
};
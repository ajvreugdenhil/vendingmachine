#pragma once

class IHelloWorld
{
    public:
    virtual ~IHelloWorld() = default;
    virtual int Square(int input) = 0;
};
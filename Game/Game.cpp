#include "../Engine/Engine.h"
#include <iostream>


#define TEXT "hello!\n"
#define MAX(a, b) (a > b) ? a : b

int main()
{
    fnEngine();
    std::cout << MAX(5, 10);
}


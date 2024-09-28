#include "src/HelloWorld.h"
#include <nuttx/config.h>
#include <cstdio>
#include <debug.h>

extern "C" int main(int argc, FAR char *argv[])
{
    CHelloWorld *pHelloWorld = new CHelloWorld();
    pHelloWorld->HelloWorld();

    CHelloWorld helloWorld;
    helloWorld.HelloWorld();

    delete pHelloWorld;
    return 0;
}
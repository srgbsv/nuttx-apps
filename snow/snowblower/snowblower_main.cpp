//
// Created by sergey on 20.07.24.
//
#include "csignal"
#include "include/MainController.h"
#include <nuttx/config.h>
#include <cstdio>

extern "C" int main(int argc, char *argv[])
{
    printf ("Hellow main\n");
    MainController::startMain(argc, argv);
    return 0;
}

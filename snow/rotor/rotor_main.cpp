/****************************************************************************
 * apps/snow/rotor/rotor_main.cpp
 ****************************************************************************/

#include <nuttx/config.h>
#include <iostream>
#include <unistd.h>
#include "MainController.hpp"

extern "C" int main(int argc, char *argv[])
{

  extern "C" int main(int argc, char *argv[])
  {
    MainController::startMain(argc, argv);
    return 0;
  }

  return 0;
}
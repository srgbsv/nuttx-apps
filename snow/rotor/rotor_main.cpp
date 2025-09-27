/****************************************************************************
 * apps/snow/rotor/rotor_main.cpp
 ****************************************************************************/

#include <nuttx/config.h>
#include <iostream>
#include <unistd.h>

extern "C" int main(int argc, char *argv[])
{

  std::cout << "Rotor daemon started and finished" << std::endl;

  return 0;
}
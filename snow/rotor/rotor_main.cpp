/****************************************************************************
 * apps/snow/rotor/rotor_main.cpp
 ****************************************************************************/

#include <nuttx/config.h>
#include <unistd.h>

#include "controllers/MainController.hpp"

extern "C" int main(int argc, char *argv[])
{
  printf("Start rotor programm\n");
  int ret = MainController::startMain(argc, argv);
  if (ret < 0) {
      printf("Rotor start failed\n");
  }
  return 0;
}
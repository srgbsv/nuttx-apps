/****************************************************************************
 * apps/snow/rotor/rotor_main.cpp
 ****************************************************************************/

#include <nuttx/config.h>
#include <unistd.h>

#include "Logging.hpp"
#include "controllers/MainController.hpp"

extern "C" int main(int argc, char *argv[])
{
  snowinfo("Start rotor programm\n");
  int ret = MainController::startMain(argc, argv);
  if (ret < 0) {
      snowinfo("Rotor start failed\n");
  }
  return 0;
}
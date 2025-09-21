/****************************************************************************
 * apps/snow/rotor/rotor_main.cpp
 ****************************************************************************/

#include <nuttx/config.h>
#include <iostream>
#include <unistd.h>

#include "rotor_can.hpp"
#include "rotor_control.hpp"

// Global instances
static RotorCan *g_can = nullptr;
static RotorControl *g_control = nullptr;
static bool g_daemon_started = false;

extern "C" int main(int argc, char *argv[])
{
  if (g_daemon_started)
    {
      std::cout << "Rotor daemon already running" << std::endl;
      return 1;
    }

  g_daemon_started = true;
  std::cout << "Rotor daemon starting" << std::endl;

  // Create instances
  g_can = new RotorCan();
  g_control = new RotorControl();

  // Initialize modules
  if (!g_can->init() || !g_control->init())
    {
      std::cerr << "Failed to initialize modules" << std::endl;
      delete g_can;
      delete g_control;
      g_can = nullptr;
      g_control = nullptr;
      g_daemon_started = false;
      return 1;
    }

  while (true)
    {
      std::cout << "Rotor daemon is running..." << std::endl;
      sleep(5);
    }

  // Cleanup
  delete g_can;
  delete g_control;
  g_can = nullptr;
  g_control = nullptr;

  g_daemon_started = false;
  std::cout << "Rotor daemon finishing" << std::endl;

  return 0;
}
/****************************************************************************
 * apps/snow/rotor/include/controllers/CanController.hpp
 ****************************************************************************/

#pragma once

#include <memory>
#include <stdatomic.h>
#include "state/State.hpp"

class CanController
{
public:
  CanController::run(const char * interface_name);
  ~CanController();

  static int stop();
  int run();

  static bool _is_running;
  static bool _need_stop;
private:
  // Private members
  CanController() {};
  std::shared_ptr<State> _state;
};

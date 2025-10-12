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
  int run(const char * interface_name);

  CanController(std::shared_ptr<State> state);
  ~CanController();

  static int stop();

  static bool _is_running;
  static bool _need_stop;
private:
  // Private members
  CanController() {};
  CanController(const CanController &controller) {};
  std::shared_ptr<State> _state;
};

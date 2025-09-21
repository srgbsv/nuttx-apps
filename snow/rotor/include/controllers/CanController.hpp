/****************************************************************************
 * apps/snow/rotor/include/controllers/CanController.hpp
 ****************************************************************************/

#ifndef __APPS_SNOW_ROTOR_CAN_CONTROLLER_HPP
#define __APPS_SNOW_ROTOR_CAN_CONTROLLER_HPP
#include <memory>
#include "State.hpp"
#include <stdatomic.h>

class CanController
{
public:
  CanController(std::shared_ptr<State> state);
  ~CanController();

  static int stop();
  int run();

private:
  // Private members
  CanController() {};
  std::shared_ptr<State> _state;
};

#endif /* __APPS_SNOW_ROTOR_CAN_CONTROLLER_HPP */

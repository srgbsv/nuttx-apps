/****************************************************************************
 * apps/snow/rotor/include/controllers/CanController.hpp
 ****************************************************************************/

#pragma once

#include "state/State.hpp"
#include <stdatomic.h>

namespace Snow {

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

}

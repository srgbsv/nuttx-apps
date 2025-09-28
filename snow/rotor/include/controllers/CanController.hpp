/****************************************************************************
 * apps/snow/rotor/include/controllers/CanController.hpp
 ****************************************************************************/

#pragma once

#include <memory>
#include <stdatomic.h>
#include "state/State.hpp"

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

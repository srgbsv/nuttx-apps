#include "controllers/CanController.hpp"
#include "providers/CanRotorNode.hpp"
#include "Logging.hpp"
#include "state/State.hpp"

namespace Snow {

    CanController::CanController(std::shared_ptr<State> state) {
    if (state != nullptr) {
        _state = state;
    }
    }

    CanController::~CanController () {
    }

    int CanController::run() {
        int ret = CanProvider::run();
        if (ret != 0) {
            snowerror("Can't start CAN provider. Error: %d", ret);
        }
        return 0;
    }

    int CanController::stop() {
        if (!_is_running) {
            snowinfo("Task not running");
            return 0;
        }

        if (_is_running) {
            _need_stop = true;
        }
    }

}

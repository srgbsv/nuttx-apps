//
// Created by sergey on 21.07.24.
//

#include <sys/ioctl.h>
#include <fcntl.h>
#include <syslog.h>

#include "Config.hpp"
#include "Logging.hpp"
#include "controllers/MainController.hpp"

pthread_mutex_t MainController::_thrower_mutex = PTHREAD_MUTEX_INITIALIZER;
std::unique_ptr<MainController> MainController::_instance; 
pid_t MainController::_task_id = -1;
bool MainController::_task_should_exit = false;


MainController::~MainController () {
    pthread_mutex_destroy (&_thrower_mutex);
}

int MainController::stopCommand () {
    int ret = 0;
    lockModule ();

    if (isRunning ()) {

        if (_instance) {
            _instance->requestStop ();

            unsigned int i = 0;

            do {
                unlockModule ();
                usleep(10000); // 10 ms
                lockModule ();

                if (++i > 500 && _task_id != -1) { // wait at most 5 sec
                    snowinfo ("timeout, forcing stop");

                    task_delete (_task_id);

                    _task_id = -1;

                    _instance.reset();

                    ret = -1;
                    break;
                }
            } while (_task_id != -1);

        } else {
            // In the very unlikely event that can only happen on work queues,
            // if the starting thread does not wait for the work queue to initialize,
            // and inside the work queue, the allocation of _object fails
            // and exitAndCleanup() is not called, set the _task_id as invalid.
            _task_id = -1;
        }
    } else {
        snowinfo ("Already stopped\n");
    }

    unlockModule ();
    return ret;
}

int MainController::startMain (int argc, char** argv) {
    snowdebug ("Start main\n");
    /*if (argc <= 1 || strcmp (argv[1], "-h") == 0 || strcmp (argv[1], "help") == 0
        || strcmp (argv[1], "info") == 0 || strcmp (argv[1], "usage") == 0) {
        MainController::printUsage ();
        return 0;
    }*/
    if (argc <= 1) {
        MainController::startCommand (argc - 1, argv + 1);
        return 0;
    }

    if (strcmp (argv[1], "start") == 0) {
        snowdebug("Command: Start\n");
        return startCommand (argc - 1, argv + 1);
    }

    if (strcmp (argv[1], "stop") == 0) {
        snowdebug("Command: Stop\n");

        return MainController::stopCommand();
    }

    if (strcmp (argv[1], "status") == 0) {
        snowdebug("Command: Status\n");

        return MainController::statusCommand();
    }

    MainController::printUsage();
    return 0;
}


int MainController::startCommand (int argc, char* argv[]) {
    int ret = 0;
    lockModule ();

    if (isRunning ()) {
        ret = -1;
        snowinfo ("Task already running\n");

    } else {
        snowdebug ("Try to spawn task. Priority: %d\n", ROTOR_TASK_PRIORITY);
                
        ret = task_create("ROTOR", ROTOR_TASK_PRIORITY, ROTOR_TASK_STACK_SIZE, MainController::taskSpawn, argv);

        if (ret < 0) {
            snowerror ("Task start failed (%i)\n", ret);
        } else {
            snowdebug ("Task spawned.  (%i)\n", ret);
        }

    }

    unlockModule ();
    return ret;
}

int MainController::statusCommand () {
    int ret = -1;
    lockModule ();

    if (isRunning () && _instance) {
        ret = _instance->printStatus ();

    } else {
        snowinfo ("not running");
    }

    unlockModule ();
    return ret;
}

bool MainController::taskShouldExit () {
    return _task_should_exit;
}

void MainController::requestStop() {
    _task_should_exit = true;
}


bool MainController::isRunning () {
    return _task_id != -1;
}

int MainController::printStatus () {
    snowinfo ("running. PID: %d\n", _task_id);
    return 0;
}

int MainController::taskSpawn (int argc, char** argv) {
    //TODO Make constructor
    std::unique_ptr<MainController> _instance = std::make_unique<MainController>(argc, argv);

    if (_instance) {
        _task_id = getpid();
        snowdebug("Task id: %d\n", _task_id);

        bool res = _instance->init();
        _instance->run();

        _instance.reset();
        _task_id = -1;
        return 0;
    } else {
        snowinfo("alloc failed");
    }

    _instance.reset();
    _task_id = -1;

    return -1;
    
}

bool MainController::updateState() {    
    _state->lock();
    auto motor_value    = _state->getMotorState().getValue();
    auto rotation_value = _state->getRotateState().getTargetAngle();
    auto ejection_value = _state->getEjectionState().getAngle();
    auto is_enabled     = _state->getIsEnabled();
    _state->unlock();
    
    bool changed = false;
   
    if (
        motor_value == _current_motor_value &&
        rotation_value == _current_rotation_value &&
        ejection_value == _current_angle_value
    ) {
        return false;
    }

    snowdebug("MainController::updateState:\nOld state: motor_value=[%d], rotation_value=[%d], ejection_value=[%d]\nNew state: motor_value=[%d], rotation_value=[%d], ejection_value=[%d]\n",
        _current_motor_value, _current_rotation_value, _current_angle_value,
        motor_value, rotation_value, ejection_value
    );

    _current_motor_value = motor_value;
    _current_rotation_value = rotation_value;
    _current_angle_value = ejection_value;

    _ejection_controller->setMotor (motor_value);
    _ejection_controller->setAngle (ejection_value);
    _ejection_controller->setRotation (rotation_value);
    _ejection_controller->setEnable (is_enabled);
      
    return true;
}

void MainController::run() {
    while (!taskShouldExit()) {
        // loop as the wait may be interrupted by a signal
        this->updateState();
        _ejection_controller->loop();

        usleep (10);
    }

    initStop();

}
 
bool MainController::init() {
    snowinfo("Initing...\n");
    _ejection_controller->init(
        _state,
        TESTING_SWITCH_IN,
        ROTATION_FIRST_SENSOR_IN,
        ROTATION_SECOND_SENSOR_IN,
        SENSOR_FIRST_ENABLE_GPIO,
        ROTATION_ENCODER_IN,
        ROTATION_ENABLE_GPIO,
        ROTATION_DIRECTION_GPIO,
        MOTOR_PWM,
        ANGLE_PWM
    );
    int ret = CanRotorNode::startNode(CAN_IFACE_NAME, _state);
    if (ret < 0) {
        snowerror("Can`t init CAN. Stopping...\n");
        return false;
    }
    return true;
}

bool MainController::initStop() {
    snowinfo("Stop initing\n");
    _instance->stop();
    _task_should_exit = true;
    return true;
}

void MainController::stop() {
    CanRotorNode::stopNode();
    _ejection_controller->stop();
}

void MainController::printUsage() {
    snowinfo("Usage: rotor [COMMAND]\n");
    snowinfo("Commands:\n");
    snowinfo("  start   - Start the rotor control task\n");
    snowinfo("  stop    - Stop the rotor control task\n");
    snowinfo("  status  - Show the status of the rotor control task\n");
}

void MainController::printUsage(const char *reason) {
    if (reason) {
        snowerror ("%s\n", reason);
    }

    printUsage();
}

void MainController::customCommand(int argc, char *argv[]) {
    return printUsage("unknown command");
}

MainController::MainController (int argc, char* argv[])
    : _state(new State()), 
    _ejection_controller(new EjectionController()) {

}
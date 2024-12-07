//
// Created by sergey on 21.07.24.
//

#include "../include/MainController.h"
#include "../include/EjectionController.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <syslog.h>
#include "../include/config.h"


pid_t MainController::_task_id = -1;
pthread_mutex_t MainController::_thrower_mutex = PTHREAD_MUTEX_INITIALIZER;
MainController* MainController::_object = nullptr;
bool MainController::_should_exit = false;


MainController::~MainController () {
    pthread_mutex_destroy (&_thrower_mutex);
    _ejection_controller.stop();
}

int MainController::stopCommand () {
    int ret = 0;
    lockModule ();

    if (isRunning ()) {
        MainController* object = _object;

        if (object) {
            object->requestStop ();

            unsigned int i = 0;

            do {
                unlockModule ();
                usleep(10000); // 10 ms
                lockModule ();

                if (++i > 500 && _task_id != -1) { // wait at most 5 sec
                    printf ("timeout, forcing stop");

                    task_delete (_task_id);

                    _task_id = -1;

                    delete _object;
                    _object = nullptr;

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
        printf ("Already stopped\n");
    }

    unlockModule ();
    return ret;
}

int MainController::startMain (int argc, char** argv) {
    printf ("startMain\n");
    if (argc <= 1 || strcmp (argv[1], "-h") == 0 || strcmp (argv[1], "help") == 0
        || strcmp (argv[1], "info") == 0 || strcmp (argv[1], "usage") == 0) {
#ifdef SNOW_DEBUG
        printf("Command: PrintUsage\n");
#endif
        return MainController::printUsage ();
    }

    if (strcmp (argv[1], "start") == 0) {
#ifdef SNOW_DEBUG
        printf("Command: Start\n");
#endif
        return startCommand (argc - 1, argv + 1);
    }

    if (strcmp (argv[1], "stop") == 0) {
#ifdef SNOW_DEBUG
        printf("Command: Stop\n");
#endif
        return MainController::stopCommand();
    }

    if (strcmp (argv[1], "status") == 0) {
#ifdef SNOW_DEBUG
        printf("Command: Status\n");
#endif
        return MainController::statusCommand();
    }

    return 1;
}


int MainController::startCommand (int argc, char* argv[]) {
    int ret = 0;
    lockModule ();

    if (isRunning ()) {
        ret = -1;
        printf ("Task already running");

    } else {
#ifdef SNOW_DEBUG
        printf ("Try to spawn task. Priority: %d", SNOW_TASK_PRIORITY);
#endif
        ret = task_create("Snowblower", SNOW_TASK_PRIORITY, SNOW_TASK_STACK_SIZE, MainController::taskSpawn, argv);

        if (ret < 0) {
            printf ("Task start failed (%i)", ret);
        } else {
#ifdef SNOW_DEBUG
            printf ("Task spawned.  (%i)", ret);
#endif
        }
    }

    unlockModule ();
    return ret;
}

int MainController::statusCommand () {
    int ret = -1;
    lockModule ();

    if (isRunning () && _object != nullptr) {
        MainController* object = _object;
        ret                    = object->printStatus ();

    } else {
        printf ("not running\n");
    }

    unlockModule ();
    return ret;
}

int MainController::printStatus () {
    printf ("running. PID: %d", _task_id);
    return 0;
}

int MainController::taskSpawn (int argc, char** argv) {
    {
        //TODO Make constructor
        MainController *instance = new MainController(argc, argv);

        if (instance) {
            _object = instance;
            _task_id = getpid();
            instance->init();
            instance->run();

            delete instance;
            _object = nullptr;
            _task_id = -1;
            return true;
        } else {
            printf("alloc failed");
        }

        delete instance;
        _object = nullptr;
        _task_id = -1;

        return false;
    }
}

bool MainController::checkState() {
#ifdef SNOW_DEBUG
    syslog (LOG_DEBUG, "CheckState()");
#endif
    auto motor_value = _input_controller.getMotorValueRaw();
    auto rotation_value = _input_controller.getRotationValueRaw();
    auto ejection_value = _input_controller.getEjectionValueRaw();
    auto is_test_btn_pressed = _input_controller.isTestBtnPressed();
#ifdef SNOW_DEBUG
    syslog(LOG_DEBUG, "MainController::checkState: New state: motor_value=[%d], rotation_value=[%d], ejection_value=[%d], is_test_btn_pressed: [%d]\n",
        motor_value, rotation_value, ejection_value, is_test_btn_pressed    
    );
#endif
    return true;
}

bool MainController::updateState() {
#ifdef SNOW_DEBUG
    syslog (LOG_DEBUG, "updateState()");
#endif    int getEjectionValue();

    auto motor_value = _input_controller.getMotorValue();
    auto rotation_value = _input_controller.getRotationValue();
    auto ejection_value = _input_controller.getEjectionValue();
    auto is_test_btn_pressed = _input_controller.isTestBtnPressed();
#ifdef SNOW_DEBUG
    syslog(LOG_DEBUG, "MainController::updateState: New state: motor_value=[%d], rotation_value=[%d], ejection_value=[%d], is_test_btn_pressed: [%d]\n",
    motor_value, rotation_value, ejection_value, is_test_btn_pressed
    );
#endif
    if (is_test_btn_pressed && !_is_test_going) {
        MainController::lockModule();
        _is_test_going = true;
        testScenario();
        _is_test_going = false;
        MainController::unlockModule();
        usleep (1000);
        return true;
    }
    _ejection_controller.forceMotorSet (motor_value);
    _ejection_controller.forceRotationSet (rotation_value == 1? 0: 1, rotation_value == -1 ? 0: 1);
    _ejection_controller.forceAngleSet (ejection_value);
    return true;
}

void MainController::run() {
    while (!shouldExit()) {
        // loop as the wait may be interrupted by a signal
        updateState();
        //checkState();
        usleep (500);
    }

    exitAndClean();
}

MainController::MainController (int argc, char** argv) {
}
bool MainController::testScenario () {
    syslog (LOG_INFO, "Run test scenario");
    /*_ejection_controller.forceRotationSet (1, 1);
    usleep (5000000);
    _ejection_controller.forceRotationSet (1, 0);
    usleep (10000000);
    _ejection_controller.forceRotationSet (1, 1);
    usleep (5000000);*/
    _ejection_controller.forceMotorSet (20);
    _ejection_controller.forceAngleSet (50);
    usleep (20000000);
    _ejection_controller.stop();
    syslog (LOG_INFO, "Test scenario complete");
    return true;
}

bool MainController::init() {
    _input_controller.init(_motor_enable_in, _rotation_enable_in, _angle_ejection_in, _test_btn_in);
    _ejection_controller.init(_direction_gpio, _rotation_enable_gpio, _motor_enable_gpio, _angle_pwm_gpio);
    return true;
}

void MainController::exitAndClean() {
#ifdef SNOW_DEBUG
    printf("Exit end clean");
#endif
    _ejection_controller.stop();
}
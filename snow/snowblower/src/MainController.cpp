//
// Created by sergey on 21.07.24.
//

#include "../include/MainController.h"
#include "../include/EjectionController.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <syslog.h>
#include "config.h"


pid_t MainController::_task_id = -1;
pthread_mutex_t MainController::_thrower_mutex = PTHREAD_MUTEX_INITIALIZER;
MainController* MainController::_object = nullptr;
bool MainController::_should_exit = false;


MainController::~MainController () {
    pthread_mutex_destroy (&_thrower_mutex);
    _ejection_controller.set_rotation_enable (false, false);

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
        printf ("Try to spawn task");
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
    auto motor_value = _input_controller.getMotorValue();
    auto rotation_value = _input_controller.getRotationValue();
    auto ejection_value = _input_controller.getEjectionValue();
    auto is_test_btn_pressed = _input_controller.isTestBtnPressed();
#ifdef SNOW_DEBUG
    syslog(LOG_DEBUG, "MainController::checkState: New state: motor_value=[%d], rotation_value=[%d], ejection_value=[%d], is_test_btn_pressed: [%d]\n",
        motor_value, rotation_value, ejection_value, is_test_btn_pressed    
    );
#endif
    return true;
}


void MainController::run() {
    while (!shouldExit()) {
        // loop as the wait may be interrupted by a signal
        checkState();
        usleep (500);
    }

    exitAndClean();
}

MainController::MainController (int argc, char** argv) {
}

bool MainController::init() {
    _input_controller.init(_motor_enable_in, _rotation_enable_in, _angle_ejection_in, _test_btn_in);
    //_ejection_controller.init();
    return true;
}

void MainController::exitAndClean() {
#ifdef SNOW_DEBUG
    printf("Exit end clean");
#endif
    //_ejection_controller->stop();


}
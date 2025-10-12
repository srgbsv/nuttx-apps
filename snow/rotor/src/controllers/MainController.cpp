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
    if (argc <= 1 || strcmp (argv[1], "-h") == 0 || strcmp (argv[1], "help") == 0
        || strcmp (argv[1], "info") == 0 || strcmp (argv[1], "usage") == 0) {
        MainController::printUsage ();
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

    printUsage();
    return 0;
}


int MainController::startCommand (int argc, char* argv[]) {
    int ret = 0;
    lockModule ();

    if (isRunning ()) {
        ret = -1;
        printf ("Task already running");

    } else {
#ifdef ROTOR_DEBUG
        printf ("Try to spawn task. Priority: %d", ROTOR_TASK_PRIORITY);
#endif
                
        ret = task_create("ROTOR", ROTOR_TASK_PRIORITY, ROTOR_TASK_STACK_SIZE, MainController::taskSpawn, argv);

        if (ret < 0) {
            printf ("Task start failed (%i)", ret);
        } else {
#ifdef ROTOR_DEBUG
            printf ("Task spawned.  (%i)", ret);
#endif
        }

        /*ret = CanRotorNode::startNode("can0");
        if (ret < 0) {
            snowerror("Can`t init CAN. Stopping...");
            return -1;
        }*/
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
        snowinfo ("not running\n");
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
    printf ("running. PID: %d", _task_id);
    return 0;
}

int MainController::taskSpawn (int argc, char** argv) {
    //TODO Make constructor
    std::unique_ptr<MainController> _instance = std::make_unique<MainController>(argc, argv);

    if (_instance) {
        _task_id = getpid();
        snowdebug("Task id: %d", _task_id);

        bool res = _instance->init();
        _instance->run();

        _instance.reset();
        _task_id = -1;
        return 0;
    } else {
        printf("alloc failed");
    }

    _instance.reset();
    _task_id = -1;

    return -1;
    
}

void MainController::run() {
    while (!taskShouldExit()) {
        // loop as the wait may be interrupted by a signal
        //printf("Tick...\n");
        usleep (10000);
    }

    initStop();
}

bool MainController::init() {
    printf("Initing...\n");
    int ret = CanRotorNode::startNode(iface_name);
    if (ret < 0) {
        snowerror("Can`t init CAN. Stopping...");
        return false;
    }
    return true;
}

bool MainController::initStop() {
    snowinfo("Stop initing\n");
}

void MainController::printUsage() {
}

void MainController::printUsage(const char *reason) {
    if (reason) {
        printf ("%s\n", reason);
    }

    printUsage();
}

void MainController::customCommand(int argc, char *argv[]) {
    return printUsage("unknown command");
}

MainController::MainController (int argc, char* argv[])
    : _state(new State()) {

}
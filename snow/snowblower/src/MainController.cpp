//
// Created by sergey on 21.07.24.
//

#include "../include/MainController.h"
#include "../include/MotorController.h"
#include "../include/EjectionController.h"
#include "MainController.h"
#include <sys/ioctl.h>
#include <fcntl.h>


MainController::~MainController () {
    _motor_controller->set_enable (false);
    _ejection_controller->set_rotation_enable (false, false);
}

MainController::MainController (int argc, char** argv)
	_pwm_input_motor(_motor_enable_in)
	_pwm_input_rotation(_rotation_enable_in)
	_motor_controller (_motor_enable_gpio)
	_ejection_controller ()
{

}

int MainController::main (int argc, char** argv) {
    if (argc <= 1 || strcmp (argv[1], "-h") == 0 || strcmp (argv[1], "help") == 0 ||
    strcmp (argv[1], "info") == 0 || strcmp (argv[1], "usage") == 0) {
        return MainController::print_usage ();
    }

    if (strcmp (argv[1], "start") == 0) {
        // Pass the 'start' argument too, because later on px4_getopt() will ignore the first argument.
        return start_command_base (argc - 1, argv + 1);
    }

    return 1;
}

bool MainController::init () {
    _rotation_e_gpio_fd = open (_rotation_enable_gpio, O_RDWR);
    if (_rotation_e_gpio_fd == 0) {
        printf("Can't open device %s", _rotation_enable_gpio);
        clear_and_exit();
    }
    _motor_e_fd = open (_motor_enable_gpio, O_RDWR);
    _direction_fd = open (_direction_gpio, O_RDWR);
    _angle_pwm_fd = 0;
}

int MainController::start_command_base (int argc, char* argv[]) {
    int ret = 0;
    lock_module ();

    if (is_running ()) {
        ret = -1;
        printf ("Task already running");

    } else {
        ret = MainController::task_spawn (argc, argv);

        if (ret < 0) {
            printf ("Task start failed (%i)", ret);
        }
    }

    unlock_module ();
    return ret;
}

int MainController::status_command () {
    int ret = -1;
    lock_module ();

    if (is_running () && _object) {
        MainController* object = _object;
        ret                    = object->print_status ();

    } else {
        printf ("not running");
    }

    unlock_module ();
    return ret;
}

bool MainController::check_State() {
    float _current_rotation;
    float _target_rotation;
    float _current_angle;
    float _target_angle;
    bool _enable_rotation;
    bool _rotation_direction;
    bool _enable_angle_control;
}

bool MainController::should_exit () { return _should_exit; }

int MainController::print_status () {
    printf ("running. PID: %d", _task_id);
    return 0;
}

int MainController::task_spawn (int argc, char** argv) {
    {
        MainController *instance = new MainController(argc, argv);

        if (instance) {
            _object = instance;
            _task_id = getpid();

            if (instance->is_running()) {
                return true;
            }

            instance->run();
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

void MainController::run() {
    while (!should_exit()) {
        // loop as the wait may be interrupted by a signal
        check_state();
    }

    exit_and_cleanup();
    return;
}

void MainController::exit_and_clean() {
    _ejection_controller->stop();
    _motor_controller->stop();

    if (_rotation_e_gpio_fd) { close (_rotation_e_gpio_fd); }
    if (_direction_fd) { close (_direction_fd); }
    if (_motor_e_fd) { close(_motor_e_fd); }
    if (_)
}
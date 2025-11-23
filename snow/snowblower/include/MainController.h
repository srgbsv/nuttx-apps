//
// Created by sergey on 21.07.24.
//

#ifndef APPS_MAINCONTROLLER_H
#define APPS_MAINCONTROLLER_H

#include "config.h"
#include "EjectionController.h"
#include "InputController.h"
//#include "Logger.h"

class MainController {
    private:
    EjectionController _ejection_controller;
    InputController _input_controller;

    //Input signals
    // USART B6 B7
    const char* _motor_enable_in      = "/dev/pwmin1"; // Ejection motor pwm signal. PA8 PA9
    const char* _rotation_enable_in   = "/dev/pwmin2"; // Ejection rotation pwm signal PA0 PA1
    const char* _angle_ejection_in    = "/dev/pwmin3"; // Ejection angle pwm signal PA6 PA7
    const char* _general_switch_in    = "/dev/pwmin9"; // Ejection global_in pwm signal PA2 PA3

    //Output signals
    const char* _rotation_enable_gpio = "/dev/gpio2";    // Rotation enable gpio. PA4
    const char* _motor_enable_gpio    = "/dev/pwmout10"; // Motor PWM. PB8
    const char* _direction_gpio       = "/dev/gpio3";    // Rotation enable gpio. PA5
    const char* _angle_pwm_gpio       = "/dev/pwmout11"; // Angle servo. PB9
    const char* _general_switch_gpio  = "/dev/gpio1";    // General switch PB4 //TODO redefine

    const char * _log_path            = "/mnt/log.log";  // Log path 

    enum class Instance : uint8_t {
        Main = 0,
        Secondary
    };

    static bool _should_exit;
    static pid_t _task_id;
    static pthread_mutex_t _thrower_mutex;

    static privateMainController* _object; // Not concurrency

    /**
	 * @brief Stars the command, ('command start'), checks if if is already
	 *        running and calls MainController::task_spawn() if it's not.
	 * @param argc The task argument count.
	 * @param argc Pointer to the task argument variable array.
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    static int startCommand (int argc, char* argv[]);

    /**
	 * @brief Stops the command, ('command stop'), checks if it is running and if it is, request the module to stop and waits for the task to complete.
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    static int stopCommand ();

    /**
	 * @brief Handle 'command status': check if running and call print_status() if it is
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    static int statusCommand ();

    [[nodiscard]] static bool shouldExit() { return _should_exit; }

    /**
	 * @brief Main loop method for modules running in their own thread. Called from run_trampoline().
	 *        This method must return when should_exit() returns true.
     */
    void run ();

    bool checkState();

    bool updateState();

    bool testScenario();

    /**
	 * @brief lock_module Mutex to lock the module thread.
     */
    static void lockModule () {
#ifdef SNOW_DEBUG
        printf ("Module LOCKED\n");
#endif
        pthread_mutex_lock (&MainController::_thrower_mutex);
    }

    /**
	 * @brief unlock_module Mutex to unlock the module thread.
     */
    static void unlockModule () {
#ifdef SNOW_DEBUG
        printf ("Module UNLOCKED\n");
#endif
        pthread_mutex_unlock (&MainController::_thrower_mutex);
    }

    /**
	 * @brief Tells the module to stop (used from outside or inside the module thread).
     */
    static void requestStop()
    {
#ifdef SNOW_DEBUG
        printf("Exit requested");
#endif
        _should_exit = true;
    }

    static int printUsage(const char *reason) {
        if (reason) {
            printf ("%s\n", reason);
        }

        printUsage();

        return 1;
    }

    static int printUsage()
    {
        printf ("help: Show this help\n");
        printf ("start: Run snowblower controller\n");
        printf ("stop: Stop snowblower controller\n");
        return 0;
    }

    void exitAndClean ();


public:
    MainController (int argc, char* argv[]);
    virtual ~MainController ();

    bool init();


    static int taskSpawn (int argc, char* argv[]);

    /**
	 * @brief Returns the status of the module.
	 * @return Returns true if the module is running, false otherwise.
     */
    static bool isRunning () { return _task_id != -1; }

    /**
	 * @brief Print the status if the module is running. This can be overridden by the module to provide more specific information.
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    static int printStatus ();

    /**
	 * @brief main Main entry point to the module that should be
	 *        called directly from the module's main method.
	 * @param argc The task argument count.
	 * @param argc Pointer to the task argument variable array.
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    static int startMain (int argc, char* argv[]);

};

#endif // APPS_MAINCONTROLLER_H
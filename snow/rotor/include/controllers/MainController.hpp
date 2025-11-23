//
// Created by sergey on 21.07.24.
//

#pragma once
#include <stdatomic.h>
#include <pthread.h>
#include <sys/types.h> 
#include <cstdio>
#include <memory>

#include "state/State.hpp"
#include "providers/CanRotorNode.hpp"
#include "controllers/EjectionController.hpp"
 
class MainController {

private:
    std::shared_ptr<State> _state;
    std::shared_ptr<CanRotorNode> _can_rotor_node;
    std::shared_ptr<EjectionController> _ejection_controller;

    /**
	 * @brief Stars the command, ('command start'), checks if if is already
	 *        running and calls MainController::task_spawn() if it's not.
	 * @param argc The task argument count.
	 * @param argc Pointer to the task argument variable array.
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    static int startCommand (int argc, char* argv[]);

    /**
	 * @brief Handle 'command status': check if running and call print_status() if it is
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    static int statusCommand ();

    bool taskShouldExit(); 

    /**
	 * @brief Main loop method for modules running in their own thread. Called from run_trampoline().
	 *        This method must return when should_exit() returns true.
     */
    void run ();

    bool init();

    bool checkState();

    static bool initStop();

    /**
	 * @brief lock_module Mutex to lock the module thread.
     */
    static void lockModule () {
        pthread_mutex_lock (&_thrower_mutex);
    }

    /**
	 * @brief unlock_module Mutex to unlock the module thread.
     */
    static void unlockModule () {
        pthread_mutex_unlock (&_thrower_mutex);
    }

    /**
	 * @brief Tells the module to stop (used from outside or inside the module thread).
     */
    void requestStop();

    
    void customCommand(int argc, char *argv[]);
    
    static void printUsage();

    static void printUsage(const char *reason);
    void stop();
        
public:

    //////   CONFIG    //////
    static constexpr const char* CAN_IFACE_NAME             = "can0";

    ////// Actuator Indexes. Can config //////
    static const int ROTATE_ACTUATOR_INDEX                  = 1;
    static const int ESC_ACTUATOR_INDEX                     = 2;
    static const int ANGLE_ACTUATOR_INDEX                   = 3;

    static constexpr const char* TESTING_SWITCH_IN          = "/dev/gpio1";    // Testing switch gpio.

    ////// ROTATION CONFIG //////
    // Input signals (device paths)
    static constexpr const char* ROTATION_FIRST_SENSOR_IN   = "/dev/gpio2";    // Rotation begin sensor gpio.
    static constexpr const char* ROTATION_SECOND_SENSOR_IN  = "/dev/gpio3";    // Rotation end sensor gpio.
    static constexpr const char* SENSOR_FIRST_ENABLE_GPIO   = "/dev/gpio4";    // First sensor enable gpio.
    static constexpr const char* ROTATION_ENCODER_IN        = "/dev/qe0";      // Rotation encoder.

    // Output signals (device paths)
    static constexpr const char* ROTATION_ENABLE_GPIO       = "/dev/gpio5";    // Rotation enable gpio.
    static constexpr const char* ROTATION_DIRECTION_GPIO    = "/dev/gpio6";    // Direction gpio.
    ////////////////////////////////

    /// @brief MOTOR CONFIG
    static constexpr const char* MOTOR_PWM                  = "/dev/pwmout10"; // Motor PWM.

    /// @brief ANGLE SERVO CONFIG
    static constexpr const char* ANGLE_PWM                  = "/dev/pwmout11"; // Angle servo.

    ////// END CONFIG //////

    static std::unique_ptr<MainController> _instance; 
    static pid_t                           _task_id;
    static pthread_mutex_t                 _thrower_mutex;
    static bool                            _task_should_exit;

    MainController (int argc, char* argv[]);
    ~MainController ();

    static int taskSpawn (int argc, char** argv);

    /**
	 * @brief Returns the status of the module.
	 * @return Returns true if the module is running, false otherwise.
     */
    static bool isRunning ();
     

    static int stopCommand();

    /**
	 * @brief Print the status if the module is running. This can be overridden by the module to provide more specific information.
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    int printStatus ();

    /**
     * @brief
     * @return Returns 0 iff successful, -1 otherwise.
     */
    static int startMain (int argc, char** argv);

    bool updateState();

};
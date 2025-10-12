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
 
class MainController {

private:
    std::shared_ptr<State> _state;
    std::shared_ptr<CanRotorNode> _can_rotor_node;

    //////   CONFIG    //////
    const char * iface_name = "can0";




    ////// END CONFIG //////

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
        
public:
    static std::unique_ptr<MainController> _instance; 
    static pid_t _task_id;
    static pthread_mutex_t _thrower_mutex;
    static bool _task_should_exit;

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

};
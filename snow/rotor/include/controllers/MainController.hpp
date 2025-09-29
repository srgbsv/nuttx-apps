//
// Created by sergey on 21.07.24.
//

#pragma once
#include <stdatomic.h>
 
extern pthread_mutex_t thrower_mutex;

class MainController {
    private:

    static std::unique_ptr<MainController> _instance; // Not concurrency

    /**
	 * @brief main Main entry point to the module that should be
	 *        called directly from the module's main method.
	 * @param argc The task argument count.
	 * @param argc Pointer to the task argument variable array.
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    static int startMain (int argc, char* argv[]);

    /**
	 * @brief Stars the command, ('command start'), checks if if is already
	 *        running and calls MainController::task_spawn() if it's not.
	 * @param argc The task argument count.
	 * @param argc Pointer to the task argument variable array.
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    static int startCommandBase (int argc, char* argv[]);

    /**
	 * @brief Stops the command, ('command stop'), checks if it is running and if it is, request the module to stop and waits for the task to complete.
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    int stop_command () {
        int ret = 0;
        lock_module ();

        if (is_running ()) {

            if (_instance) {
                _instance->request_stop ();

                unsigned int i = 0;

                do {
                    unlock_module ();
                    usleep(10000); // 10 ms
                    lock_module ();

                    if (++i > 500 && _task_id != -1) { // wait at most 5 sec
                        printf ("timeout, forcing stop");

                        task_delete (_task_id);

                        _task_id = -1;

                        delete _instance;

                        ret = -1;
                        break;
                    }
                } while (_task_id != -1);

            } else {
                // In the very unlikely event that can only happen on work queues,
                // if the starting thread does not wait for the work queue to initialize,
                // and inside the work queue, the allocation of _object fails
                // and exit_and_cleanup() is not called, set the _task_id as invalid.
                _task_id = -1;
            }
        }

        unlock_module ();
        return ret;
    }

    /**
	 * @brief Handle 'command status': check if running and call print_status() if it is
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    static int status_command ();

    bool should_exit();

    /**
	 * @brief Main loop method for modules running in their own thread. Called from run_trampoline().
	 *        This method must return when should_exit() returns true.
     */
    void run ();

    bool init();

    void check_state();

    public:
    MainController (int argc, char* argv[]);
    ~MainController ();


    static int task_spawn (int argc, char* argv[]);

    /**
	 * @brief Returns the status of the module.
	 * @return Returns true if the module is running, false otherwise.
     */
    static bool is_running () {
        return _task_id != -1;
    }

    /**
	 * @brief Print the status if the module is running. This can be overridden by the module to provide more specific information.
	 * @return Returns 0 iff successful, -1 otherwise.
     */
    int print_status ();

    private:
    /**
	 * @brief lock_module Mutex to lock the module thread.
     */
    static void lock_module () {
        pthread_mutex_lock (&thrower_mutex);
    }

    /**
	 * @brief unlock_module Mutex to unlock the module thread.
     */
    static void unlock_module () {
        pthread_mutex_unlock (&thrower_mutex);
    }

    /**
	 * @brief Tells the module to stop (used from outside or inside the module thread).
     */
    virtual void request_stop()
    {
        _task_should_exit = true;
    }

    int custom_command(int argc, char *argv[])
    {
        return print_usage("unknown command");
    }

    static int print_usage(const char *reason) {
        if (reason) {
            printf ("%s\n", reason);
        }

        print_usage();

        return 1;
    }

    static int print_usage()
    {
        printf ("Start module");
    }

    bool check_State ();
};

pid_t MainController::_task_id = -1;


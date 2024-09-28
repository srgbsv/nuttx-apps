//
// Created by sergey on 20.07.24.
//
#include "csignal"
#include "include/MainController.h"
#include <nuttx/config.h>
#include <cstdio>

/****************************************************************************
 * Name: sigterm_action
 ****************************************************************************/

static void sigterm_action(int signo, siginfo_t *siginfo, void *arg)
{
    if (signo == SIGTERM)
    {
        std::printf("SIGTERM received\n");

        g_led_daemon_started = false;
        printf("led_daemon: Terminated.\n");
    }
    else
    {
        printf("\nsigterm_action: Received signo=%d siginfo=%p arg=%p\n",
        signo, siginfo, arg);
    }
}

/****************************************************************************
 * Name: snowblower_daemon
 ****************************************************************************/

static int led_daemon(int argc, char *argv[])
{
    userled_set_t supported;
    userled_set_t ledset;
    bool incrementing;
    int ret;
    int fd;
    pid_t mypid;
    struct sigaction act;

    /* SIGTERM handler */

    memset(&act, 0, sizeof(struct sigaction));
    act.sa_sigaction = sigterm_action;
    act.sa_flags     = SA_SIGINFO;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGTERM);

    ret = sigaction(SIGTERM, &act, NULL);
    if (ret != 0)
    {
        fprintf(stderr, "Failed to install SIGTERM handler, errno=%d\n",
        errno);
        return (EXIT_FAILURE + 1);
    }

    /* Indicate that we are running */

    mypid = getpid();

}

extern "C" int main(int argc, char *argv[])
{
    MainController::task_spawn (argc, argv);
    return 0;
}

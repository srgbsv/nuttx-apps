//
// Created by sergey on 01.09.24.
//

#ifndef SNOWBLOWER_PWMOUTPUT_H
#define SNOWBLOWER_PWMOUTPUT_H

#include "PwmState.h"

struct pwm_info_s
{
    /* Frequency of the pulse train */
    uint32_t           frequency;
#ifdef CONFIG_PWM_MULTICHAN
    /* Per-channel output state */
    struct pwm_chan_s  channels[CONFIG_PWM_NCHANNELS];
#else
    /* Duty of the pulse train, "1"-to-"0" duration.
     * Maximum: 65535/65536 (0x0000ffff)
     * Minimum:     1/65536 (0x00000001)
     */
    ub16_t             duty;
#ifdef CONFIG_PWM_DEADTIME
    /* Dead time value for main output */
    ub16_t             dead_time_a;
    /* Dead time value for complementary output */
    ub16_t             dead_time_b;
#endif
#ifdef CONFIG_PWM_PULSECOUNT
    /* The number of pulse to generate.  0 means to
     * generate an indefinite number of pulses
     */
    uint32_t           count;
#endif
    /* Channel polarity */
    uint8_t            cpol;
    /* Disabled channel polarity */
    uint8_t            dcpol;
#endif /* CONFIG_PWM_MULTICHAN */
    /* User provided argument to be used in the lower half */
    FAR void           *arg;
};

class PwmOutput {
    char * devpath;
    file _fd;
    pwm_info_s pwminfo;

    public:
        PwmInput(char * devpath);
        ~PwmInput();

        int set_duty_cycle();
    private:
        PwmInput();
};


#endif // SNOWBLOWER_PWMOUTPUT_H

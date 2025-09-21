//
// Created by sergey on 01.09.24.
//

#ifndef SNOWBLOWER_PWMINPUT_H
#define SNOWBLOWER_PWMINPUT_H


class PwmInput {
    char * devpath;
    file fd

    public:
        PwmInput(char * devpath);
        ~PwmInput();

        int get_duty_cycle();
    private:
        PwmInput();
};


#endif // SNOWBLOWER_PWMINPUT_H

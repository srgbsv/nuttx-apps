#pragma once

#include <stdlib.h>
#include <time.h>
#include <memory>
  
class SystemTools
{
private:
    /* data */

public:

    /*
    get a 64 bit monotonic timestamp in microseconds since start. This
    is platform specific
    */
    static uint64_t micros64(void)
    {
        static uint64_t first_us;
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        uint64_t tus = (uint64_t)(ts.tv_sec * 1000000ULL + ts.tv_nsec / 1000ULL);
        if (first_us == 0) {
            first_us = tus;
        }
        return tus - first_us;
    }

    /*
    get monotonic time in milliseconds since startup
    */
    static uint32_t millis32(void)
    {
        return micros64() / 1000ULL;
    }

    /*
    get a 16 byte unique ID for this node, this should be based on the CPU unique ID or other unique ID
    */
    static void getUniqueID(uint8_t id[16])
    {
        memset(id, 0, 16);
        FILE *f = fopen("/etc/machine-id", "r");
        if (f) {
            fread(id, 1, 16, f);
            fclose(f);
        }
    }

};
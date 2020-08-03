/* apps/helloworld/components/Client/src/client.c */

#include <stdio.h>

#include <camkes.h>

#define NS_IN_SECOND 1000000000

static inline double get_time_seconds() {
    double ns = timer_get_time();
    return ns / NS_IN_SECOND;
}

void report_time() {
    printf("Time now: %lfs\n", get_time_seconds());
}

void delay() {
    /*
    // delay via spinning
    for (int i = 0; i < 20*1000*1000; ++i) {
        asm volatile("nop");
    }
    */
    // delay via timer
    printf("Sleeping for 2 seconds...\n");
    timer_sleep(2 * NS_IN_SECOND);
}

int run(void) {
    printf("----- Dhrystone on Rocket Chip -----\n\n");

    printf("Testing timer...\n");
    report_time();
    for (int i = 0; i < 10; ++i) {
        delay();
        report_time();
    }

    printf("----- Client done -----\n");

    return 0;
}

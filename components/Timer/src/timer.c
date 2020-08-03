#include <stdio.h>
#include <stdint.h>

#include <camkes.h>

#include "pwm.h"

#define CMP_BITS 16
#define CMP ((1 << CMP_BITS) - 1)
#define SHIFT 0xf

// simple timer implementing a wall clock and timeout
// IRQ is used to keep track of time in case of wrap around

uint64_t wraps = 0;

static inline uint32_t readl(void *reg, uint64_t offset) {
    uint64_t ptr = (uint64_t)reg + offset;
    ZF_LOGV("reading from %#lx", ptr);
    return *(uint32_t*)ptr;
}

static inline void writel(void* reg, uint64_t offset, uint32_t value) {
    uint64_t ptr = (uint64_t)reg + offset;
    ZF_LOGV("writing to %#lx", ptr);
    *(uint32_t*)ptr = value;
}

void timer__init() {
    ZF_LOGV("%s: Timer init", get_instance_name());
    
    // program PWM0 for monotonic
    writel(clock_reg, PWM_CMP0, CMP);
    writel(clock_reg, PWM_COUNT, 0);
    writel(clock_reg, PWM_CFG, SHIFT | PWMZEROCMP | PWMSTICKY | PWMENALWAYS);
}

uint64_t timer_get_time() {
    ZF_LOGV("%s: wraps=%lu", get_instance_name(), wraps);
    uint64_t ticks = wraps * (CMP << SHIFT);
    ticks += readl(clock_reg, PWM_COUNT);
    return ticks * ((double)NS_IN_SECOND / input_freq);
}

void timer_sleep(uint64_t ns) {
    uint64_t ticks = ns / ((double)NS_IN_SECOND / input_freq);

    // calculate log2(ticks)
    uint64_t tmp = ticks;
    uint64_t bits = 0;
    while (tmp >>= 1) {
        ++bits;
    }
    ++bits;

    uint64_t shift = 0;
    if (bits >= CMP_BITS) {
        shift = bits - CMP_BITS;
        if (shift == 0) {
            shift = 1;
        }
    }

    // sleep at least ns
    uint64_t shifted = (ticks >> shift) + 1;

    ZF_LOGV("ns=%lu ticks=%lu bits=%lu shift=%lu shifted=%lu", ns, ticks, bits, shift, shifted);
    assert(shift <= SHIFT);
    assert(shifted <= CMP);

    // program PWM1 for timeout
    writel(timeout_reg, PWM_CMP0, shifted);
    writel(timeout_reg, PWM_COUNT, 0);
    writel(timeout_reg, PWM_CFG, shift | PWMZEROCMP | PWMENONESHOT);

    int error = sem_wait();
    ZF_LOGF_IF(error != 0, "failed to wait on semaphore");
}

void clock_irq_handle() {
    ZF_LOGV("%s: Clock IRQ", get_instance_name());
    ++wraps;

    // clear interrupt & reset counter
    uint32_t cfg = readl(clock_reg, PWM_CFG);
    writel(clock_reg, PWM_CFG, cfg & (~PWMCMP0IP));
    writel(clock_reg, PWM_COUNT, 0);

    int error = clock_irq_acknowledge();
    ZF_LOGF_IF(error != 0, "failed to acknowledge IRQ");
}

void timeout_irq_handle() {
    ZF_LOGV("%s: Timeout IRQ", get_instance_name());

    // clear interrupt & stop timer
    uint32_t cfg = readl(timeout_reg, PWM_CFG);
    writel(timeout_reg, PWM_CFG, cfg & (~(PWMCMP0IP|PWMENONESHOT)));

    // post on semaphore
    int error = sem_post();
    ZF_LOGF_IF(error != 0, "failed to post to semaphore");

    error = timeout_irq_acknowledge();
    ZF_LOGF_IF(error != 0, "failed to acknowledge IRQ");
}

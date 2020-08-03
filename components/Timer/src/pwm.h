#pragma once

// register offsets
#define PWM_CFG   0x0
#define PWM_COUNT 0x8
#define PWM_PWMS  0x10
#define PWM_CMP0  0x20
#define PWM_CMP1  0x24
#define PWM_CMP2  0x28
#define PWM_CMP3  0x2c

// pwmcfg fields
#define PWMSCALE      0b1111 // [3:0]
#define PWMSTICKY     (1 << 8)
#define PWMZEROCMP    (1 << 9)
#define PWMDEGLITCH   (1 << 10)
#define PWMENALWAYS   (1 << 12)
#define PWMENONESHOT  (1 << 13)
#define PWMCMP0CENTER (1 << 16)
#define PWMCMP1CENTER (1 << 17)
#define PWMCMP2CENTER (1 << 18)
#define PWMCMP3CENTER (1 << 19)
#define PWMCMP0GANG   (1 << 24)
#define PWMCMP1GANG   (1 << 25)
#define PWMCMP2GANG   (1 << 26)
#define PWMCMP3GANG   (1 << 27)
#define PWMCMP0IP     (1 << 28)
#define PWMCMP1IP     (1 << 29)
#define PWMCMP2IP     (1 << 30)
#define PWMCMP3IP     (1 << 31)

#define NS_IN_SECOND 1000000000

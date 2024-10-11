
#ifndef HAL_CONSTANTS_RTC_H
#define HAL_CONSTANTS_RTC_H

typedef enum{
    RTC_ST_PREINIT,
    RTC_ST_IDLE,
    RTC_ST_WAIT_READ,
    RTC_ST_WAIT_WRITE,
    RTC_ST_SYNC
} RTC_ST_t;

#endif
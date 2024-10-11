

#ifndef HAL_TYPES_RTC_H
#define HAL_TYPES_RTC_H

#include <stdint.h>
#include <hal/constants/rtc.h>
#include <time/time.h>

typedef struct RTC_STATE_t {
    RTC_ST_t state;
    tm_real_t * iobuffer;
    void (*cb)(void);
}rtc_state_t;

#endif

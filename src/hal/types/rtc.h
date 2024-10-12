

#ifndef HAL_TYPES_RTC_H
#define HAL_TYPES_RTC_H

#include <stdint.h>
#include <hal/constants/rtc.h>

#ifndef TM_REAL_t_DEFINED
#define TM_REAL_t_DEFINED

typedef struct TM_REAL_t{
    uint8_t century;
    uint8_t year;
    uint8_t month;
    uint8_t date;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t millis; 
} tm_real_t;

#endif

typedef struct RTC_STATE_t {
    RTC_ST_t state;
    tm_real_t * iobuffer;
    void (*cb)(void);
}rtc_state_t;

#endif

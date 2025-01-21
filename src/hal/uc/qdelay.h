    /* 
 * Copyright (c)
 *   (c) 2025 Chintalagiri Shashank
 *  
 * This file is part of
 * Embedded bootstraps : hal-uC
 * 
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

/**
 * @file qdelay.h
 * @brief HAL for Quick Delay
 * 
 */

#ifndef HAL_UC_QDELAY_H
#define HAL_UC_QDELAY_H

#include "map.h"

#if uC_QDELAY_ENABLED

#if uC_QDELAY_NONBLOCKING

typedef struct QDELAY_SPEC_t{
    uC_QDELAY_TICKCOUNT_t max;
    uint32_t tick_freq;
} qdelay_spec_t;

typedef struct QDELAY_t{
    uC_QDELAY_TICKCOUNT_t start;
    uC_QDELAY_TICKCOUNT_t end;
} qdelay_t;


void qdelay_init(void);
void qdelay_start(qdelay_t * delay, uint16_t us);
bool qdelay_check_done(qdelay_t * delay);

#endif

static inline void qdelay_us(uint16_t us);

// Set up the implementation
#include <hal_platform/qdelay_impl.h>

#endif
#endif

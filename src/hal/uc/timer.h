/*  
   Copyright 2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
    
   This file is part of
   Embedded bootstraps : hal-uC
   
   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

/**
 * @file timer.h
 * @brief HAL for uC Timers
 *
 * 
 */

#ifndef HAL_UC_TIMER_H
#define HAL_UC_TIMER_H

#include "map.h"

#if uC_TIMER_ENABLED

/*
 * Least common denominator timer configuration types :
 * 
 *     - UP         : Count to TOP, Reset
 *     - UPDOWN     : Count to TOP, Count to BOT
 *     - CONTINUOUS : Count to MAX
 * 
 * On MSP430, this implementation reserves channel 0 for setting the period (TOP), 
 * so you would normally use CH1+ for specialized uses. On platforms like the 
 * MSP430, this implementation has the unfortunate side effect of making CH0 
 * unusable as a PWM in cases where TOP need not be changed. Being able to switch 
 * the top interrupt to the overflow interrupt would be useful, if messy.
 * 
 * Supported configuration types :
 * 
 *     - LF Periodic Timer : UP, other channels can be used to relative phase shift.
 *     - Edge Counting : CONTINUOUS. all channels can be used to hook specific counts. 
 *     - PWM : UP/CONTINUOUS, other channels have duty cycle set by CHx CCR.
 */

// Initialize timer clock source, default prescaler.
static inline void timer_init(HAL_BASE_t intfnum);

// Change timer prescaler.
static inline void timer_set_prescaler(HAL_BASE_t intfnum, uint16_t prescaler);

// Start the timer in the provided mode. 
static inline void timer_set_mode(HAL_BASE_t intfnum, TIMER_MODE_t mode);

// Enable the timer overflow interrupt.
static inline void timer_enable_int_overflow(HAL_BASE_t intfnum);

// Disable the timer overflow interrupt.
static inline void timer_disable_int_overflow(HAL_BASE_t intfnum);

// Enable the timer top interrupt.
static inline void timer_enable_int_top(HAL_BASE_t intfnum);

// Disable the timer top interrupt.
static inline void timer_disable_int_top(HAL_BASE_t intfnum);

// Set TOP for the timer.
static inline void timer_set_top(HAL_BASE_t intfnum, HAL_BASE_t top);

// Get TOP for the timer.
static inline HAL_BASE_t timer_get_top(HAL_BASE_t intfnum);

// Set output mode for the timer channel.
static inline void timer_set_outmode_ch(HAL_BASE_t intfnum, HAL_BASE_t channel, TIMER_OUTMODE_t outmode);

// Enable the compare match interrupt for the timer channel.
static inline void timer_enable_int_ch (HAL_BASE_t intfnum, HAL_BASE_t channel);

// Disable the compare match interrupt for the timer channel.
static inline void timer_disable_int_ch(HAL_BASE_t intfnum, HAL_BASE_t channel);

// Set the channel compare match value.
static inline void timer_set_cmr_ch(HAL_BASE_t intfnum, HAL_BASE_t channel, uC_TIMER_CCR_t cm);

// Get the channel compare match value.
static inline uC_TIMER_CCR_t timer_get_cmr_ch(HAL_BASE_t intfnum, HAL_BASE_t channel);

#include "hal_platform/timer_impl.h"
#include "hal_platform/timer_handlers.h"
#endif
#endif

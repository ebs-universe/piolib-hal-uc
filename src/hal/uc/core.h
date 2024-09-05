/* 
   Copyright 2015 Quazar Technologies Pvt. Ltd.
   Copyright 2015 Chintalagiri Shashank
    
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
 * @file core.h
 * @brief HAL for uC Core peripherals
 *
 * This file is the hardware abstraction layer for all core uC peripherals
 * and functions. 
 */

#ifndef HAL_UC_CORE_H
#define HAL_UC_CORE_H

#include "map.h"

#if uC_CORE_ENABLED
/**
 * @name Core Management Functions 
 * Implementation is left to the implementation layer at `core_impl.h`.
 */
/**@{*/ 
/** 
 * Initialize the core to sane defaults. 
 * 
 * This should do the following: 
 *  - Initialize CPU features on plaforms that need it
 *  - Initialize the interrupt system on platforms that need it
 *  - Initialize the core clocks needed to get the system functional
 *  
 * It is expected that applications needing something different will not 
 * use this portion of the HAL and instead use the platform specific library 
 * or code to set up each individual dimension of the core.
 */
void core_init(void);
/**@}*/ 

/**
 * @name Power Management API Functions
 * Various functions to control the power system. Implementation is 
 * left to the implementation layer at `core_impl.h`.
 */
/**@{*/ 
/** Set core voltage to the highest possible. The use of low power 
 modes will require exploring the common motifs of power system 
 selection and improving this API. For now, we don't care. Since this
 is a one-off thing, we'll also make it a regular function and not 
 bother about the overhead. */
void power_set_full(void);
/**@}*/ 

/**
 * @name Clock Management API Functions
 * Various functions to control the clock system. Implementation is 
 * left to the implementation layer at `core_impl.h`.
 * 
 * Note that with the inclusion of complex clock systems such as the 
 * RCC of STM32 CM4 processors, the prescaler and default functions 
 * are largely meaningless. Finding a common interface for all possible
 * clock architectures is going to be next to impossible. 
 * 
 * For now, the assumption is that clock_set_default() is the main 
 * function, while clock_set_prescaler() is deprecated and may be 
 * implemented as a noop, if at all
 */
/**@{*/ 

/** Set the clock prescaler. See the specific HAL implementation for 
 * what is allowed here and what the implications of changing the 
 * prescaler are going to be. 
 */
void clock_set_prescaler(uint16_t ps);

/** Set all clocks to some sane defaults. For now, we don't care about 
 * the details. Since this is a one-off thing, we'll also make it a 
 * regular function and not bother about the overhead. 
 */
void clock_set_default(void);

/**@}*/ 
#endif 

#if uC_WDT_ENABLED
/**
 * @name Watchdog API Functions
 * Various functions to control the primary watchdog. Implementation is 
 * left to the implementation layer at `core_impl.h`.
 */
/**@{*/ 
/** 
 * Initialize the watchdog timer to some default interval. If the platform 
 * does not allow setting of watchdog parameters separate from start, then 
 * this function does nothing. 
 */
void watchdog_init(void);

/** Hold the watchdog and prevent reset due to watchdog timeout.*/
void watchdog_hold(void);

/** Start the watchdog timer.*/
void watchdog_start(void);

/** Clear the watchdog timer.*/
static inline void watchdog_clear(void);

/**@}*/ 

#endif

#if uC_SYSTICK_TIMER_ENABLED

/** Enable the SysTick timer, if one exists. ARM Cortex provides these 
 * timers, it is unclear how common these are. Given the approach taken 
 * with EBS pioloib-time, which expects a 1ms tick, we simply configure 
 * it to that frequency. Use cases wanting something different can be 
 * considered for future enhancements.   
 * 
 * Platforms without a special systick timer should simply make this a no-op.
 * 
 * Actually calling this function will be left the system initialization 
 * sequence, specifically, tm_systick_init of piolib-time. 
 * 
 * If the system requires the tick to boot up to the point where tm_init 
 * can be run, then that use case may need to be separately addressed. For 
 * the moment, implementations should assume this function might be called 
 * multiple times during init.   
 */
void core_systick_start(void); 

#endif

// Set up the implementation
#include <hal_platform/core_impl.h>
#include <hal_platform/core_handlers.h>

#endif


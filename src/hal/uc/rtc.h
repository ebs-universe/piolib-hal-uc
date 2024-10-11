/* 
 * Copyright (c)
 *   (c) 2024 Chintalagiri Shashank
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
 * @file rtc.h
 * @brief HAL for uC RTCs
 *
 * This file is the hardware abstraction layer for uC provided RTCs. 
 * 
 * A number of platforms do not include RTCs, so applications may need to 
 * pull in additional drivers for each hardware design. How this is going 
 * to work is unclear. For the moment, it is recommended that RTC drivers
 * try to mirror the interface defined here.
 * 
 * In general, it should not be assumed that any particular platform or 
 * board even has an RTC, even when the platform embeds one. Applications
 * should take responsibility for the hardware being present and enable 
 * the RTC interface. 
 * 
 * The RTC abstraction layer should only be enabled if the ebs time library 
 * is also included in the project. Specifically, we make use of the 
 * various time types and manipulation functions provided by the time 
 * library in this interface. 
 * 
 * The RTC interface functions are generally expected to block for some time, 
 * to ensure the read / write is aligned reasonably closely to the second 
 * tickoover or a similar least count. In general, applications are expected 
 * to maintain their own time for regular operations using the ebs time 
 * library, while interfacing with the RTC peripheral at these points: 
 * 
 *  - At initialization, rtc_read can be used to initialize the system 
 *    time to the RTC value. 
 *  - At external time provision, rtc_sync (if available) or rtc_write
 *    can be used to correct the RTC stored time.
 *  - For always-on or otherwise long-running devices, the application 
 *    can periodically sync system time from the RTC using rtc_sync 
 *    (if available) at safe points. 
 * 
 * Applications should avoid any other interactions with the RTC.
 * 
 * Two types of implementations are possible for the RTC interface: 
 * 
 *  - Basic interface, where read and writes may block to upto 1 second,
 *    but are relatively simpler and probably use less memory.
 *  - Complex interface, where read and write would be interrupt driven,
 *    so don't block, but need to store a reasonable amount of state in 
 *    the heap. 
 * 
 */

#ifndef HAL_UC_RTC_H
#define HAL_UC_RTC_H

#include "map.h"

#if uC_RTC_ENABLED

#include <hal/types/rtc.h>

/**
 * @name RTC API Functions
 * 
 */
/**@{*/ 

/**
 * Perform any startup initialization needed for the RTC. 
 * 
 */
void rtc_init(void);

/**
 * @brief Read the current time from the uC's RTC.
 * @param target Pointer to the tm_system_t in which this time should be written. 
 * @param cb Pointer to the callback function. 
 * 
 * Basic implementations may block for upto 1 second. 
 * 
 * Complex implementations should setup the read and return. The read itself 
 * should be handled by interrupts instead of blocking. Some implementations 
 * may choose to manage the transaction by polling, in which case they may use 
 * `rtc_poll`, which will be added to the main loop. 
 * 
 * When the read is complete, the provided callback (if any) should be called 
 * to inform the application that the read is complete. Applications should 
 * ensure the target is read only after the read is complete.
 * 
 * Applications must ensure that atmost one RTC operation is running at any 
 * given time. Implementations must ensure that rtc_init will cancel any 
 * in-progress transactions as safely as possible.
 * 
 * Returns : 
 * 
 *  0 - Read is complete and data is valid
 *  1 - Read is triggerd, not complete
 *  2+ - Read failed
 */
uint8_t rtc_read(tm_real_t * target, void(*cb)(void));

/**
 * @brief Write the current time to the uC's RTC.
 * @param source Pointer to the tm_system_t containing the timestamp to be written.
 * @param cb Pointer to the callback function. 
 * 
 * Basic implementations may block for upto 1 second. 
 * 
 * Complex implementations should setup the write and return. The write itself 
 * should be handled by interrupts instead of blocking. Some implementations may 
 * choose to manage the transaction by polling, in which case they may use 
 * `rtc_poll`, which will be added to the main loop. 
 * 
 * When the read is complete, the provided callback (if any) should be called 
 * to inform the application that the write is complete. Applications should
 * ensure that the source remains valid until then. 
 * 
 * Applications must ensure that atmost one RTC operation is running at any 
 * given time. Implementations must ensure that rtc_init will cancel any 
 * in-progress transactions as safely as possible.
 * 
 * * Returns : 
 * 
 *  0 - Write is complete
 *  1 - Write is triggerd, not complete
 *  2+ - Write failed 
 */
uint8_t rtc_write(tm_real_t * source, void(*cb)(void));


/**
 * @brief Sync the current time to the uC's RTC.
 * @param cb Pointer to the callback function. 
 * 
 * This function should sync the current system time as maintained by the 
 * ebs time library to the RTC. This function may block. Implementations
 * should attempt to keep this as quick as possible, and be aware that 
 * other interrupts (including the system tick) may occur in the interim.  
 * 
 * When the sync is complete, the provided callback (if any) should be called 
 * to inform the application that the sync is complete.
 * 
 * Applications must ensure that atmost one RTC operation is running at any 
 * given time. Implementations must ensure that rtc_init will cancel any 
 * in-progress transactions as safely as possible.
 * 
 * Not all implementations may allow sync. In such case, implementations 
 * may simply write the current timestamp to the RTC. Applications 
 * requiring high quality timestamps must ensure the platform and 
 * implementation being used has a suitable sync implementation. 
 * 
 */
void rtc_sync(void(*cb)(void));


static inline void rtc_poll(void);

/**@}*/ 

// Set up the implementation
#include <hal_platform/rtc_impl.h>

#endif
#endif

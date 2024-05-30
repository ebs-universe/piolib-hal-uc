/* 
   Copyright 2024 Chintalagiri Shashank
    
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
 * @file hal_uc_eeprom.h
 * @brief HAL for uC EEPROM peripherals
 *
 * This file is the hardware abstraction layer for uC built-in EEPROMs and 
 * might extend to other NV storage options. 
 */

#ifndef HAL_UC_EEPROM_H
#define HAL_UC_EEPROM_H

#include "map.h"

/**
 * @name EEPROM API Functions
 * Various functions to interact with the eeprom. Implementation is 
 * left to the implementation layer at `eeprom_impl.h`.
 * 
 * We expect to be using an upstream library for these functions in 
 * almost all platforms. As such, the functions are effectively thin
 * wrappers and are left to be inlined. This may be revisted in the 
 * future.
 * 
 */
/**@{*/ 

/** 
 * Initialize the EEPROM, if it needs any initialization. Most 
 * implementations will probably leave this blank. 
 */
static inline void eeprom_init(void);

/** Check if EEPROM is ready for the next operation. 
 * 
 * This is provided separately for allowing shorter spinlocks in applications with 
 * highly constrained timings, 
 * 
 * For most applications, the HAL implementation's internal checks should hopefully 
 * be enough, even if slow. 
 * */
static inline uint8_t eeprom_ready(void);

/** Write a byte to the EEPROM at the specified address.*/
static inline void eeprom_write(uint16_t address, uint8_t data);

/** Read a byte from the EEPROM at the specified address*/
static inline uint8_t eeprom_read(uint16_t address);

/** Write a regular buffer to the EEPROM at the specified address. */
static inline void eeprom_write_buffer(uint16_t address, const uint8_t * data, uint8_t length);

/** Read into a regular buffer from the EEPROM at the specified address. */
static inline void eeprom_read_buffer(uint16_t address, uint8_t * data, uint8_t length);

/**@}*/ 

// Set up the implementation
#include <hal_platform/eeprom_impl.h>

#endif


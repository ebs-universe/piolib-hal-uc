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
 * @file eeprom.h
 * @brief HAL for uC EEPROM peripherals
 *
 * This file is the hardware abstraction layer for uC built-in EEPROMs 
 * and might extend to other on-chip NV storage options. 
 */

#ifndef HAL_UC_EEPROM_H
#define HAL_UC_EEPROM_H

#include "map.h"

#if uC_EEPROM_ENABLED == 1

/**
 * @name EEPROM API Functions
 * Various functions to interact with the eeprom. Implementation is 
 * left to the implementation layer at `eeprom_impl.h`.
 * 
 * In platforms which don't have a built in EEPROM, implementations 
 * may implement this interface for any other non-volatile storage 
 * backend. 
 * 
 * Users should not expect this to be implemented for every platform, 
 * since not every platform includes on-chip non-volatile storage. 
 * Even when implemented, users should be aware of the nature of 
 * non-volatile storage provided on each of the target platforms. 
 * Mere presense of this interface in an implementation should not 
 * be treated as a guarantee of reliable non-volatile storage.
 * 
 * Implementations can provide some control to the application to 
 * choose the non-volatile storage system to use, through the defines
 *   - APP_EEPROM_PROVIDER
 *   - APP_EEPROM_SIZE
 *   - APP_EEPROM_SIZETYPE
 *   - APP_EEPROM_PAGESIZE
 * 
 * While this should be verified for each underlying implementation,
 * the following guidelines are recommended to implementations : 
 * 
 * Provider 1 : Regular EEPROM or Data Flash with byte r/w access
 * Provider 2 : Battery Backed SRAM with byte r/w access
 * Provider 3 : EEPROM emulation with Program Flash with page erase
 * 
 * EEPROM size to be made available via this interface must also be 
 * specified in application.h. It is the application's responsibility
 * to ensure the specified size fits within the available storage in 
 * the underlying hardware. 
 * 
 * For more complex non-volatile storage configurations including 
 * support for multiple storage volumes and off-chip storage, see 
 * the EBS nvstorage library instead. 
 *  
 */
/**@{*/ 

/** 
 * Initialize the EEPROM, if it needs any initialization. Most 
 * implementations will probably leave this blank. 
 */
static inline uint8_t eeprom_init(void);

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
static inline uint8_t eeprom_write_byte(uC_EEPROM_SIZE_t address, uint8_t data);

/** Read a byte from the EEPROM at the specified address*/
static inline uint8_t eeprom_read_byte(uC_EEPROM_SIZE_t address);

/** Write a regular buffer to the EEPROM at the specified address. */
static inline uC_EEPROM_SIZE_t eeprom_write(uC_EEPROM_SIZE_t address, const void * data, uC_EEPROM_SIZE_t length);

/** Read into a regular buffer from the EEPROM at the specified address. */
static inline uC_EEPROM_SIZE_t eeprom_read(uC_EEPROM_SIZE_t address, void * data, uC_EEPROM_SIZE_t length);

/**@}*/ 

// Set up the implementation
#include <hal_platform/eeprom_impl.h>

#endif
#endif


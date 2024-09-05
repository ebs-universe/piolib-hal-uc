/* 
 * Copyright (c)
 *   (c) 2018 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
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
 * @file id.h
 * @brief HAL for uC ID
 *
 * This file is the hardware abstraction layer for acquring unique 
 * identification information from a uC, using the best available
 * mechanism for the platform.
 * 
 * This HAL framework expects there to be two possible lengths for the ID.
 * 
 *  - uC_ID_MAXLEN : Length of the underlying ID source, defined by the implementation.  
 *  - uC_ID_LENGTH : Length of the standardized ID source, ie, 8 bytes.
 * 
 * If the underlying ID source is not 8 bytes, the implementation should 
 * repack it into 8-bytes while preserving as much uniqueness as possible.
 * It might be done just once at startup and stored in RAM or recalculated 
 * at each call to the HAL.  
 * 
 * The repacked value should be returned only when an 8 byte ID is requested.
 * Note that any other length requested will return bytes from the original
 * underlying ID.
 * 
 * Applications critically dependent on uniqueness of the ID or on the 
 * security provided by an in-silicon serial number should use uC_ID_MAXLEN 
 * instead. 
 * 
 */

#ifndef HAL_UC_ID_H
#define HAL_UC_ID_H

#include "map.h"

#if uC_ID_ENABLED

#define uC_ID_LENGTH    8

typedef union DeviceID_t {
    HAL_BASE_t native[uC_ID_LENGTH / sizeof(HAL_BASE_t)];
    uint8_t bytes[uC_ID_LENGTH];
} DeviceID_t;

/**
 * @name ID API Functions
 * 
 */
/**@{*/ 

/**
 * Perform any startup initialization needed to support the ID API. 
 * Most implementations might not actually do anything here.
 * 
 * This hook can also be used by the implementation to generate the 
 * standardized length ID (8 bytes) if the generation is computationally 
 * expensive.  
 * 
 */
void id_init(void);

/**
 * @brief Read the uC's unique identification.
 * @param maxlen Length (in bytes) of the identifier to write to the buffer. 
 * @param buffer Pointer to a buffer into which the ID is to be written.
 * @return Number of bytes of the ID which were written to the buffer.
 * 
 * This obtains the best available identification information for the uC and 
 * writes it into the provided buffer. 
 * 
 * The nature of the identifier used is dependent on the uC family. In-silicon 
 * serial number is the ideal identifier when available. See the specific uC 
 * implementation for information about what is used for each platform.
 *  
 * If the maxlen requested is 8, then the underlying implementation should 
 * return a repacked version of the ID in standardized length while preserving 
 * as much uniqueness as possible.
 *
 */
uint8_t id_read(uint8_t len, void * buffer);

/**
 * @brief Write the uC's unique identification.
 * @param len Length of the identifier to write to the uC. 
 * @param content Pointer to a buffer containing the ID to be written.
 * @return Number of bytes of the ID which was written to the uC.
 * 
 * Writes a unique ID to the uC. This is applicable only for platforms which 
 * allow writing an ID to the uC. Such a mechanism should only be provided 
 * when a uC does not itself provide a sufficient and readable identification 
 * mechanism. 
 * 
 * For platforms which do not require this functionality, this function should
 * return 0 for all provided inputs and not have any other effect.
 * 
 * Generally, the length to be written here should always be 8 bytes.
 *
 * Note that though len is taken in bytes, application code must ensure that 
 * the provided length is a multiple of HAL_INT_t.
 */
uint8_t id_write(uint8_t len, void * content);
/**@}*/ 

// Set up the implementation
#include <hal_platform/id_impl.h>

#endif
#endif

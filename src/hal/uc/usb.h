/* 
 * Copyright (c)
 *   (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
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
 * @file usb.h
 * @brief HAL for USB interface
 *
 * This file is the hardware abstraction layer for uC USB interfaces. Due to 
 * the complexity of the USB implentation, this layer is handled differently for
 * USB compared to the other peripherals. The HAL only has minimal control over
 * the interface, and most of the details are left to the underlying 
 * implementation. Most configuration details are hardcoded into the implementation. 
 * 
 * When porting to a different board/uc/product, care must be taken. The needed
 * configuration data should go into the underlying implementation. As such, it
 * is recommended that as far as possible, predefined USB configurations be used 
 * as far as possible. A separate protocol can be built on top of the USB layer. 
 * 
 * The only role of the implementation in `usb_impl.h` and `usb_impl.c` is to 
 * translate these function names to the names in the underlying manufacturer 
 * implementation.
 * 
 * The intent is to try and provide a unified, even if feature-incomplete, HAL 
 * interface to handle multiple USB device libraries across uC families. 
 * 
 * The application can use the underlying functions directly as well, for the cost 
 * of reduction in portablility.
 * 
 * @see [USB API Integration and Usage Notes](@ref usbhal)
 * 
 * @see usb_impl.h 
 * @see usb_impl.c
 */

#ifndef HAL_UC_USB_H
#define HAL_UC_USB_H

#include "map.h"

#if uC_USB_ENABLED

/**
 * @name USB Connection Statuses 
 */
/**@{*/ 
/** \brief USB Host is not connected */
#define USB_ST_DISCONNECTED            0

/** \brief USB Host is connected but not enumerated */
#define USB_ST_NOTENUMERATED           1

/** \brief USB Enumeration in progress */
#define USB_ST_ENUMERATING             2

/** \brief USB is ready for use */
#define USB_ST_ACTIVE                  3

/** \brief Host has placed the device in suspend */
#define USB_ST_SUSPENDED               4

/** \brief Device suspended before enumeration */
#define USB_ST_NOENUM_SUSPENDED        5

/** \brief USB Error */
#define USB_ST_ERROR                   6
/**@}*/ 

/**
 * @name USB Core Management API Functions
 * 
 */
/**@{*/ 
/**
 * \brief Initialize USB and connect if possible.
 */
static inline void usb_init(void);

/**
 * \brief Enable the USB module. 
 * 
 * This function may be deprecated. Use usb_connect() and usb_diconnect() instead, 
 * while usb_disable() should be treated as usb_deinit().
 * 
 * Needed only if USB is to be disabled intermittently. Otherwise, `usb_setup()` 
 * should also execute this function or equivalent.
 */
static inline void usb_enable(void);

/**
 * \brief Disable the USB module. 
 * 
 * Needed only if USB is to be disabled intermittently.
 */
static inline void usb_disable(void);

/**
 * \brief Connect to host. 
 * 
 * Needed only if USB is to be disabled intermittently. Otherwise, `usb_setup()` 
 * should also execute this function or equivalent.
 */
static inline void usb_connect(void);

/**
 * \brief Disconnect from host. 
 * 
 * Needed only if USB is to be disabled intermittently.
 */
static inline void usb_disconnect(void);

/**
 * \brief Get the current status of the USB subsystem.
 * 
 * \returns Status as specified as one of the allowed status defines.
 *          
 */
static inline uint8_t usb_get_status(void);

/**@}*/ 

#include <hal_platform/usb/usb_impl.h>
#include <hal_platform/usb/usb_handlers.h>
#endif
#endif

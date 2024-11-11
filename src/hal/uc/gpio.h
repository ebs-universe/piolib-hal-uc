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
 * @file gpio.h
 * @brief HAL for uC GPIOs
 *
 * This file is the hardware abstraction layer for uC GPIOs
 */

#ifndef HAL_UC_GPIO_H
#define HAL_UC_GPIO_H

#include <platform/types.h>
#include "map.h"

#if uC_GPIO_ENABLED

/**
 * @name GPIO Configuration API Functions
 * Various functions to configure the uC GPIOs.
 * 
 * The following common notes apply to all the GPIO API sections.
 * 
 * Pin number(s) is to be provided as: 
 *   - a mask for platforms with simple GPIO interfaces.
 *   - an integer for platforms with complex GPIO interfaces with multi-bit registers.
 * 
 * Note that when provided as an integer, functions can only operate on a single pin 
 * at a time. In such cases, the platform probably provides a way to execute atomic 
 * multiple pin changes. The platform's library or registers should be used directly 
 * in such cases.
 * 
 * The following implementations expect a mask: 
 *   - AVR
 *   - MSP430
 * 
 * The following implementations expect an integer: 
 *   - STM32F4
 * 
 */
/**@{*/ 

/** 
 * @brief Initialize the GPIO peripherals.
 * 
 * This is for common initialization for GPIO peripherals, such as enabling the GPIO clock
 * on STM32 Cortex M processors. This will likely be empty for most other implementations. 
 */
void gpio_init(void);

/** 
 * @brief Configure pin / pins as output. 
 * 
 * @param port Port number
 * @param pin Pin number(s)
 */
void gpio_conf_output(PORTSELECTOR_t port, PINSELECTOR_t  pin);

/** 
 * @brief Configure pin / pins as input.
 * @param port Port number
 * @param pin Pin number(s)
 */
void gpio_conf_input(PORTSELECTOR_t port, PINSELECTOR_t pin);

/** 
 * @brief Configure pin / pins for peripheral's use.
 * @param port Port number
 * @param pin Pin number(s)
 * @param periph Peripheral. In platforms where it matters, the MSB determines whether 
 *               the pin is to be used as an input or output. The remaining 7 bits are 
 *               reserved for use in devices which allow configurable peripheral use. 
 */
void gpio_conf_periph(PORTSELECTOR_t port, PINSELECTOR_t pin, uint8_t periph);

/** 
 * @brief Configure pin / pins for analog use.
 * 
 * If this is controlled directly by the MCU and not needed, the implementation should
 * simply leave this as a noop. 
 * 
 * @param port Port number
 * @param pin Pin number(s)
 */
void gpio_conf_analog(PORTSELECTOR_t port, PINSELECTOR_t pin);

/** 
 * @brief Configure pullup on pin / pins.
 * @param port Port number
 * @param pin Pin number(s)
 */
void gpio_conf_pullup(PORTSELECTOR_t port, PINSELECTOR_t pin);

/** 
 * @brief Configure pin / pins for input with pulldown.
 * @param port Port number
 * @param pin Pin number(s)
 */
void gpio_conf_pulldown(PORTSELECTOR_t port, PINSELECTOR_t pin);

/** 
 * @brief Configure pin / pins for high impedance input.
 * @param port Port number
 * @param pin Pin number(s)
 */
void gpio_conf_hiz(PORTSELECTOR_t port, PINSELECTOR_t pin);

/** 
 * @brief Configure pin / pins for Push-Pull output mode.
 * 
 * If the platform does not support push-pull type outputs, this should error out.
 * 
 * @param port Port number
 * @param pin Pin number(s)
 */
void gpio_conf_outmode_pp(PORTSELECTOR_t port, PINSELECTOR_t pin); 


/** 
 * @brief Configure pin / pins as Open Drain output.
 * 
 * If the platform does not support open drain output types, this should error out.
 * 
 * @param port Port number
 * @param pin Pin number(s)
 */
void gpio_conf_outmode_od(PORTSELECTOR_t port, PINSELECTOR_t pin); 


void gpio_conf_speed(PORTSELECTOR_t port, PINSELECTOR_t pin, uint8_t speed);
/**@}*/

/**
 * @name GPIO Interaction API Functions
 * Various functions to interact with uC GPIOs
 */
/**@{*/ 

/** 
 * @brief Set pin / pins output to high.
 * @param port Port number
 * @param pin Pin number(s)
 */
static inline void gpio_set_output_high(PORTSELECTOR_t port,
                                        PINSELECTOR_t pin);

/** 
 * @brief Set pin / pins output to low.
 * @param port Port number
 * @param pin Pin number
 */
static inline void gpio_set_output_low(PORTSELECTOR_t port,
                                       PINSELECTOR_t pin);

/** 
 * @brief Toggle output state of pin / pins.
 * @param port Port number
 * @param pin Pin number(s)
 */
static inline void gpio_set_output_toggle(PORTSELECTOR_t port,
                                          PINSELECTOR_t pin);

/** 
 * @brief Retrieve state of single input pin.
 * @param port Port number
 * @param pin Pin number
 */
static inline HAL_BASE_t gpio_get_input(PORTSELECTOR_t port,
                                        PINSELECTOR_t pin);

/**@}*/ 

/**
 * @name GPIO Interrupt Related API Functions
 * Various functions to configure and manage interrupts for uC GPIOs.
 * 
 * Note that GPIO interrupts take many different forms on different platforms. 
 * The underlying implementation should take a reasonable least common 
 * denominator approach to provide a basic interface to the most common 
 * configurations.
 * 
 * Applications must also take care to ensure that interrupts requested 
 * here are actually present and viable, and that there are no hardware 
 * conflicts it creates. 
 * 
 */
/**@{*/ 

#define GPIO_INT_EDGE_NONE      0x00
#define GPIO_INT_EDGE_RISING    0x01
#define GPIO_INT_EDGE_FALLING   0x10
#define GPIO_INT_EDGE_BOTH      0x11

#define GPIO_INT_ON_RISING      0xF0
#define GPIO_INT_ON_FALLING     0x00

/** 
 * @brief Configure interrupt on pin change. 
 * 
 * This function configures the GPIO pin change interrupt on the chosen edges
 * but does not arm it. 
 * 
 * The application must : 
 *  - use gpio_conf_interrupt_handler to install a handler.
 *  - use gpio_interrupt_arm to arm the interrupt. 
 * 
 * Application code must also ensure the GPIO interrupt specified is viable 
 * based on the MCU resources available. If there is an underlying conflict,
 * the EBS HAL does not do anything to detect it. Unexpected behavior may occur. 
 * 
 * Note that the EBS HAL does not hanlde any debouncing. Implementations for 
 * platforms with integrated debouncing mechanisms, if any, may need the interface
 * to be tweaked slightly. In general, applications are responsible for 
 * implementing any debouncing needed, and should do so in hardware if possible.  
 * 
 * @param port Port number
 * @param pin Pin number
 * @param edge The edge on which the interrupt should fire.
 */
void gpio_conf_interrupt(PORTSELECTOR_t port, PINSELECTOR_t pin, HAL_BASE_t edge);

/** 
 * @brief Install the provided function as an interrupt handler for the pin. 
 * 
 * Note that this function does not check whether the correct interrupts are 
 * configured. This is left to the application code to correctly setup using 
 * other EBS HAL functions.
 * 
 * Note that the provided function will be called from an interrupt context. 
 * Also, in the case of GPIO interrupts, we might already be one or two 
 * function calls deep inside the interrupt. If this function is used, try 
 * to keep the handler function as small as possible.
 * 
 * Some platforms may choose to provide a more optimal way of providing this 
 * functionality, such as an inlinable function accepted as a #define. Check 
 * the underlying implementation to see if this function is supported. 
 * 
 * The function is called with a uint8_t which can be used by the application 
 * to determine which interrupt fired. Generally, the uint8_t will be a 
 * number indicating a bit position. In platforms supporting separate rising
 * and falling interrupt flags, the MSB of this variable will indicate which 
 * edge has triggered the interrupt. 
 * 
 * Note that if both edges trigger simultaneously or otherwise get stacked, the 
 * handler will be called twice in such platforms. Platforms which have a single 
 * flag will only call the handler once.  
 * 
 * @param port Port number
 * @param pin Pin number
 * @param handler Interrupt handler function
 */
void gpio_conf_interrupt_handler(PORTSELECTOR_t port, PINSELECTOR_t pin, void (*handler)(uint8_t));

/** 
 * @brief Arm the Pin Change Interrupt
 * 
 * Note that this function does not check whether the correct interrupts are 
 * configured. This is left to the application code to correctly setup using 
 * other EBS HAL functions.
 * 
 * @param port Port number
 * @param pin Pin number
 */
void gpio_interrupt_arm(PORTSELECTOR_t port, PINSELECTOR_t pin);

/** 
 * @brief Disarm the Pin Change Interrupt
 * 
 * Note that this function does not check whether the correct interrupts are 
 * configured. This is left to the application code to correctly setup using 
 * other EBS HAL functions.
 * 
 * @param port Port number
 * @param pin Pin number
 */
void gpio_interrupt_disarm(PORTSELECTOR_t port, PINSELECTOR_t pin);
/**@}*/

// Set up the implentation
#include <hal_platform/gpio_impl.h>

#endif
#endif


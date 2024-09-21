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
 * @file hal_uc.h
 * @brief Highest level include for uC HAL.
 *
 * This file is the header that should be used to obtain the functions to control 
 * the microcontroller and it's peripherals, whose peripheral map is defined by 
 * `map.h`, which in turn is configured by application headers `application.h`, 
 * `iomap.h`, and `irq_handlers.h`. HAL uC brings in the underlying drivers 
 * provided by specific HAL uC implementations, which use base libraries and 
 * provides wrappers around it to form a controlled interface. The compile-time
 * definitions within `map.h` determine which underlying interface / peripheral 
 * should be used to provide the required functionality.
 * 
 * Note that both HAL uC and the implementation are intended to be compiled with 
 * the application itself. HAL uC implementations will regularly import "map.h", 
 * which in turn will import `application.h`. `application.h` will exist and be 
 * valid only in the context of the application. This is different from the 
 * original structure using the CMake build system. It is better aligned with 
 * workflows managed by PlatformIO and results in far less complicated 
 * configuration flows.
 * 
 * The controlled HAL_uC interface should be preferred over the underlying 
 * driver APIs (such as `driverlib` for MSP430) when possible, even though 
 * both versions exist in the namespace. This will allow easier migration and 
 * code-sharing with other ICs and/or platforms.
 * 
 * The `hal/uc` library should ideally be portable across all implementations. 
 * MCU specific implementations go into `hal_platform` and would ideally be 
 * portable to entire MCU families. Some graceful way to bow out should be 
 * implemented when these functions are not provided by a certain uC. Generally,
 * this would be in the form of a compile time error.
 * 
 * The 'hal/uc' layer provides only function prototypes and should have zero 
 * bytecode footprint aside from weak noop implementations. The mapping from 
 * the HAL layer to the implementation layer is determined by the 
 * `(module)_impl.h` and `(module)_impl.c` files within the implementation 
 * layer (the `hal_platform` folder). 
 * 
 * Additionally, `(module)_handlers.h` and `(module)_handlers.c` can be added in 
 * the implementation layer to provide implementations of relevant IRQ handlers. 
 * 
 * One-time or low-frequency use functions, such as init and setup functions, will
 * typically be included in `(module)_impl.c`. Functions that are expected to be used 
 * more often in time-critical settings go instead into `(module)_impl.h` as 
 * `static inline` functions to minimize function call overhead. The optimal split 
 * between inline and regular functions is a somewhat subjective and application 
 * dependent. A reasonable split is attempted here, but if your use case is considerably 
 * different, you may need to modify the implementation layer to suit your particular 
 * needs. 
 * 
 */
 

#ifndef HAL_uC_ALL_H
#define HAL_uC_ALL_H


/* 
 * Include all the drivers. Each of these header files will then go on to 
 * inclde the corresponding implementation files for the platform that is being built. 
 * The implementation files should be within the predefined include paths. 
 * 
 * The expected location is `hal_platform/<driver>_impl.h`, and this will generally be
 * available from a separate HAL uC implementation library for each MCU Family.
 */

#include "map.h"
#include "core.h"     
#include "crypto.h"  
#include "gpio.h"       
#include "id.h"         
#include "eeprom.h"      
#include "entropy.h"    
#include "adc.h"
#include "timer.h"
#include "uart.h"
#include "spi.h"
#include "usb.h"        
#include "usbcdc.h"    

#endif

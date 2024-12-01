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
 * @file dma.h
 * @brief HAL for DMA
 *
 * This is a placeholder for a a potentially standardized DMA interface. 
 * For the moment, the DMA interface is defined in the implementation 
 * itself for use by the other components of the implementation. 
 * 
 */

#ifndef HAL_UC_DMA_H
#define HAL_UC_DMA_H

#include "map.h"

#if uC_DMA_ENABLED

// Set up the implementation
#include <hal_platform/dma_handlers.h>
#include <hal_platform/dma_impl.h>

#endif
#endif

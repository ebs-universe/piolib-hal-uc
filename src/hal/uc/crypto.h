/* 
 * Copyright (c)
 *   (c) 2024 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
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
 * @file aes.h
 * @brief HAL for uC AES hardware
 *
 * WARNING
 * This is entirely untested. Implementation is only done for AES GCM. 
 * It is unclear whether the results are actually valid. Most likely 
 * they aren't. More time needs to be spent on this to figure out what 
 * the actual algorithms are. It is unclear whether there is any value 
 * in pursuing this, given the complexity involved. 
 * 
 * This is the hardware abstraction layer for hardware crypto 
 * accelerators provided by the uC. 
 * 
 * The following alrogithms are supported by this interface:
 *   - AES ECB
 *   - AES CBC
 *   - AES CTR
 *   - AES GCM
 *   - AES GMAC
 *   - AES CCM
 * 
 * Not all platforms will support all modes. Implementations may fall back to 
 * software solutions, such as AVR Cryptolib, when hardware accelerators are 
 * not available. Applications can set APP_CRYPTO_HARDWARE to configure what 
 * underlying implementation can be used. Implementations should raise compile 
 * time / preprocessor errors if a suitable provider for the chosen configuration 
 * is not available. This would probably happen with a combination of 
 * uc_map_impl.h and crypto_impl.h (?)
 * 
 * This interface is based on the crypto features present in the STM32U083, 
 * and may evolve over time as other hardware is supported. For the moment, 
 * we're only implementing for CRYPTO_HW_ALWAYS.
 */

#ifndef HAL_UC_AES_H
#define HAL_UC_AES_H

#include "map.h"


#if uC_CRYPTO_ENABLED

/**
 * @name Cryptographic API Functions
 * 
 */
/**@{*/ 

/**
 * @brief Initialize Cryptography  
 * 
 * Initialize the crypto subsystem. If a default context exists, set it up and 
 * make it ready to recieve data. 
 * 
 */
void crypto_init(void);

/**
 * @brief Set the Cryptography Context  
 * 
 * Switch to the provided context. Apply the associated profile. 
 * 
 * This function should also do the following:
 *  - If there is an exisitng crypto operation in process, this should fail
 *    and return 0. 
 *  - If the existing contex is the same as the context to be set, this function
 *    should not do anything. If the application wishes to reset the context, it 
 *    should release it first.
 *  - If the existing context associated with the crypto subsystem is different 
 *    from the requested context, and if it is in a state in which it can be 
 *    suspended, the implementation should save whatever state needs to be saved 
 *    and suspend it. 
 *  - If the context to be set was earlier suspended, then it should be restored
 *    and put back into the same mode / phase it was in.  
 * 
 * Applications should generally call this before starting any transaction, or 
 * continuing a disjointed / polled transaction. When there is a possibility 
 * that another context may have taken over, this becomes quite important. 
 * Applications can also verify that the context is still active by checking 
 * the context's state or by checking that the crypto context currently active 
 * is the one expected. 
 * 
 * This is also where the application can set the following context parameters: 
 *   - mode : encryption / decryption
 *   - ingest callback 
 * 
 * The ingest callback can be used by the application to handle encryption or 
 * decryption of larger chunks of data than the ingest_size, though it must be
 * used with care.
 * 
 * In platforms with hardware crypto accelerators, the ingest callback 
 * will generally be called in an interrupt context when an ingest operation 
 * is complete. In these instances, the callback must be short.
 * 
 * Software crypto accelerators may call the callback within the ingest 
 * function's context itself. Using the callback to directly call the 
 * ingest function will lead to an unbounded recursion depth. Also note 
 * that none of these functions can be expected to be reentrant. On these 
 * platforms, therefore, the application must simply set a flag here. 
 * 
 * Applications targeting the general platform should, therefore, simply 
 * set an appropriate flag in this callback, and have something in the main
 * polling loop trigger the next ingest cycle.
 * 
 * Generally, applications can assume that when the ingest callback is 
 * called, the context is still active. The callback can return a non-zero 
 * value to keep the state as BUSY and avoid the context relinquishing 
 * control of the underlying resources. The application should only do 
 * this if it _knows_ it has data ready to go. It should also maintain some
 * flag informing it of this state, so that it can provide the necessary 
 * override to the ingest function's busy flag. 
 * 
 * If this function returns 1, then the crypto core is presently busy 
 * configuring the requested context and phase. Applications should 
 * wait for the crypto core state to change from busy to ready.  
 * 
 * Return values : 
 *   0 : crypto is busy
 *   1 : crypto context is set, but is not yet ready
 *   2 : crypto configuration is set and ready
 *   3 : crypto configuration is already set, nothing to do
 */
uint8_t crypto_set_ctx(crypto_ctx_t *const ctx, CRYPTO_CTX_MODE_t mode, 
                       uint8_t (*ingest_cb)(crypto_ctx_t *const, uint8_t));

/**
 * @brief Set the phase for the provided cryptographic context
 * 
 * The provided phase enum is intended as a generalized phase progression.
 * The implementation is only expected to manage the following phase transitions: 
 *   - When the context is reset, it is set to the INIT phase
 *   - In modes where the Init phase needs no additional information, the 
 *     implementation will trigger init when the init phase is set. Once init 
 *     is complete, phase will be rotated forward in the appropriate interrupt 
 *     handler or the software call. 
 * 
 * For example, the application will normally never see the init phase of the 
 * GCM mode. When the context is reset by the application, the crypto core 
 * will switch to the init phase, which then handles init and rotates the 
 * phase forward to the header phase.
 * 
 * All other context phase transistions are to be managed by the application.
 * It is upto the application to ensure the phase progression makes sense. 
 * Unsupported phase transitions, if requested, may result in unexpected 
 * behavior.
 *  
 * Not all cypto algorithms will support all phases, though to a reasonable 
 * extent the same algorithm should have the same expected phase progression
 * across implementations and platforms. 
 * 
 * Phases set on the context should persist until it is set once again to 
 * another phase by the application or, in the case of automatic phases, the 
 * implementation. The set phase should survive suspension, etc, and only 
 * rotate back to init when the context is reset.
 * 
 * If the return value is non-zero, this means additional work is needed to 
 * set the phase, or the phase is an automatic phase, and the application may 
 * need to wait before executing the next action. 
 * 
 * Note that if the provided context is not the active context, this function
 * does not change that. The application will need to set context before it 
 * can use it, and any phase preparation steps will be done at that time.
 * 
 * The included transactions interface should, ideally, manage most phase 
 * transitions. The degree of completeness of this interface is upto the 
 * implementation.
 */
uint8_t crypto_set_ctx_phase(crypto_ctx_t * ctx, CRYPTO_CTX_PHASE_t phase);

/**
 * @brief Reset the provided cryptographic context
 * 
 * Reset the phase, and mode for the context. If the context maintains any 
 * additional state, that is also reset. The ingest callback is also 
 * deregistered.
 * 
 * This also signals that the application is done with this context for now. 
 * The application should know that it is safe to release, and that there are 
 * no pending operations it cares about. This will also reset any state held 
 * inside this context. 
 * 
 * If the provided context is also the current context, then the crypto 
 * core is also reset and set to idle. 
 * 
 */
void crypto_reset_ctx(crypto_ctx_t * ctx);

/**
 * @brief Apply a Cryptography Profile
 * 
 * Configure the cryptography subsystem to implement the provided profile in 
 * the provided mode and phase, resetting the subsystem if needed by the 
 * platform. 
 * 
 * If configuring this phase requires intermediate states too long to block on, 
 * the implementation should return a non-null value. The caller can then use 
 * the ctx state, specifically its phase, to determine what needs to be done 
 * next.
 * 
 * Generally, applications should use crypto_set_ctx and crypto_set_ctx_phase
 * instead of directly calling this.  
 */
uint8_t crypto_apply_profile(
    const crypto_profile_t *const profile, 
    CRYPTO_CTX_MODE_t mode, 
    CRYPTO_CTX_PHASE_t phase
);

/**
 * @brief Trigger the provided transaction
 * 
 * High level function to trigger a single crypto transaction. The
 * transaction will be managed by the implementaion using interrupt 
 * callbacks. 
 * 
 * The application must prepare the context as needed and provide
 * all the necessary buffers. The application can know that the 
 * transaction is done by polling the transaction state or by 
 * providing a callback.
 *   
 */
uint8_t crypto_trigger_transaction(crypto_transaction_t *const transaction);

/**
 * @brief Append data to the current cryptographic context
 *  
 * The application should ensure that data of length equal to the ingest 
 * size expected by the context is available at the address. If the data 
 * is shorter than the ingest size, it should be padded with 0s on the end. 
 * This would generally only be allowed on the last ingest block. For 
 * encryption algorithms which care about this sort of thing (AES GCM and 
 * CCM), provide the number of non-valid bytes in the input as well, ie, 
 * the number of zero bytes for padding.   
 * 
 * The busy override can be set if the application *knows* that the crypto 
 * core is not actually busy, i.e., the application requested the busy state
 * to not be released in the previous interrupt. There are no other 
 * circumstances in which this can be used.
 *  
 * Implementations should generally set the ingest size to the data size 
 * accepted by the underlying crypto accelerator hardware in a single 
 * operation. This is less critical for software crypto implementations. 
 * HAL_BASE_t might be a good place to start, though any size which has 
 * a meaningful benefit may be chosen.
 * 
 * If possible, such as when implemented by hardware, this function should 
 * not block. It should copy the data into the crypto core and set the 
 * appropriate busy state. 
 * 
 * Software implementations may choose to process the ingest within this
 * call itself and execute the callback. They may also choose to store the 
 * ingested data elsewhere and set a flag for the crypto core to operate on 
 * during some main loop polling. 
 * 
 * The application can register a callback on the context to know when the
 * ingest is complete. The callback will also be provided the length ingested. 
 * Note that the callback may be called from an interrupt context or from the
 * context of this function itself. If the callback returns a non-zero value, 
 * the busy state of the crypto system will not be cleared. 
 * 
 * The application may choose to poll crypto core state instead of using 
 * the callback, but note that this can be very risky if there are multiple 
 * crypto users in the application.
 * 
 * This function should return the number of bytes read, or 0 if the 
 * operation was blocked. 
 */
uint8_t crypto_ingest(void * data, uint8_t nvbytes, uint8_t override_busy);

/**
 * @brief Extract the latest calculated ingest length of the result
 *  
 * Copy the latest result into the provided target buffer. The application
 * should know by other means that there is valid data here and how much of 
 * this data is valid. Generally, this function would be called in the 
 * application provided ingest callback. 
 * 
 * The application should ensure that there is enough space available in 
 * the target buffer to hold the requested data. For AES, the length here 
 * would be generally equal to the ingest size, except for the last block 
 * ingested, where the length would the number of valid (non-padding) bytes 
 * in the last block.    
 * 
 * This function is inlined because it is expected to be called deep in an 
 * interrupt context. 
 */
void crypto_egest(void * data, uint8_t length);
/**@}*/



// Set up the implementation
#include <hal_platform/crypto_impl.h> 

#endif
#endif


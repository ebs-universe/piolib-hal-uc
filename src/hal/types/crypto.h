

#ifndef HAL_TYPES_CRYPTO_H
#define HAL_TYPES_CRYPTO_H

#include <stdint.h>


typedef enum{
    CRYPTO_HW_NEVER,
    CRYPTO_HW_IFAVAIL,
    CRYPTO_HW_ALWAYS
} CRYPTO_HARDWARE_t;

typedef enum{
    CRYPTO_AES_ECB,
    CRYPTO_AES_CBC,
    CRYPTO_AES_CTR,
    CRYPTO_AES_GCM,
    CRYPTO_AES_GMAC,
    CRYPTO_AES_CCM,
} CRYPTO_ALG_t;

typedef enum{
    CRYPTO_PREINIT,
    CRYPTO_IDLE,
    CRYPTO_READY,
    CRYPTO_BUSY
} CRYPTO_ST_t;

// Swap operations to deal with endianness differences.
typedef enum{
    CRYPTO_SWAP_1B,
    CRYPTO_SWAP_8B,
    CRYPTO_SWAP_16B,
    CRYPTO_SWAP_NONE
} CRYPTO_SWAP_t;

typedef enum{
    CRYPTO_CTX_MODE_IDLE,
    CRYPTO_CTX_MODE_ENCRYPTION,
    CRYPTO_CTX_MODE_DECRYPTION,
    CRYPTO_CTX_MODE_SUSPENDED,
} CRYPTO_CTX_MODE_t;

typedef enum{
    CRYPTO_CTX_PHASE_INIT,
    CRYPTO_CTX_PHASE_PREPKEY,
    CRYPTO_CTX_PHASE_DERIVATION,
    CRYPTO_CTX_PHASE_HEADER,
    CRYPTO_CTX_PHASE_PAYLOAD,
    CRYPTO_CTX_PHASE_FINAL
} CRYPTO_CTX_PHASE_t;

typedef enum{
    CRYPTO_TR_PENDING,
    CRYPTO_TR_INPROGRESS,
    CRYPTO_TR_DONE
} CRYPTO_TR_STATE_t;

typedef struct CRYPTO_PROFILE_t{
    const CRYPTO_ALG_t  alg;
    const CRYPTO_SWAP_t swap;
    const uint8_t       key_size;
    const uint8_t       iv_size;
    const uint8_t       ingest_size;
    const uint8_t       block_size;
    const uint8_t       header_size;
    const uint8_t       mac_size;
    void *const         key;
    void *const         iv;
} crypto_profile_t;

typedef struct CRYPTO_CTX_t{
    CRYPTO_CTX_MODE_t mode;
    CRYPTO_CTX_PHASE_t phase;
    const crypto_profile_t *const profile;
    void (*reset)(struct CRYPTO_CTX_t *const);
    uint8_t (*ingest_cb)(struct CRYPTO_CTX_t *const, uint8_t);
} crypto_ctx_t;

typedef struct CRYPTO_TRANSACTION_t{
    crypto_ctx_t * ctx;
    CRYPTO_CTX_MODE_t mode;
    CRYPTO_TR_STATE_t state;
    uint16_t input_len;
    uint16_t output_len;
    uint16_t input_len_done;
    void * input;
    void * output;
    uint8_t (*trc_cb)(struct CRYPTO_TRANSACTION_t *const);
} crypto_transaction_t;

typedef struct CRYPTO_STATE_t{
    CRYPTO_ST_t state;
    crypto_ctx_t * ctx;
    crypto_transaction_t * transaction;
} crypto_state_t;

#endif

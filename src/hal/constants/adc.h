
#ifndef HAL_CONSTANTS_ADC_H
#define HAL_CONSTANTS_ADC_H

#define ADC_MODE_DISABLED       0   // ADC Disabled
#define ADC_MODE_IDLE           1   // Single Shot, Single Channel Conversion on trigger
#define ADC_MODE_SINGLE         2   // Single Shot, Single Channel Conversion on trigger
#define ADC_MODE_SCAN           3   // Scan though all enabled ADC channels on trigger
#define ADC_MODE_CONTINUOUS     4   // Continuous Scan though all enabled ADC channels ono trigger

typedef enum {
    _ADC_MODE_DISABLED = ADC_MODE_DISABLED,
    _ADC_MODE_IDLE = ADC_MODE_IDLE,
    _ADC_MODE_SINGLE = ADC_MODE_SINGLE,
    _ADC_MODE_SCAN = ADC_MODE_SCAN,
    _ADC_MODE_CONTINUOUS = ADC_MODE_CONTINUOUS
} ADC_MODE_t;

#define ADC_DM_POLLING          0
#define ADC_DM_INTERRUPT        1
#define ADC_DM_DMA              2

typedef enum {
    _ADC_DM_POLLING = ADC_DM_POLLING,
    _ADC_DM_INTERRUPT = ADC_DM_INTERRUPT,
    _ADC_DM_DMA = ADC_DM_DMA
} ADC_DATA_MAMANGEMENT_MODE_t;

#endif
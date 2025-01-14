

#ifndef HAL_CONSTANTS_COMMON_H
#define HAL_CONSTANTS_COMMON_H

#include <platform/constants.h>

#define EBS_INTF_UART       0
#define EBS_INTF_USBCDC     1
#define EBS_INTF_APP        0xFE

typedef enum {
    _EBS_INTF_UART   = EBS_INTF_UART,
    _EBS_INTF_USBCDC = EBS_INTF_USBCDC,
    _EBS_INTF_APP    = EBS_INTF_APP
} EBS_INTF_TYPE_t;

#endif
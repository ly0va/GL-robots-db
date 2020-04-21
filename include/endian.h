#ifndef _ENDIAN_H_
#define _ENDIAN_H_

#include <inttypes.h>

enum {
    LITTLE_ENDIAN = 0x0100u,
    BIG_ENDIAN    = 0x0001u
};

static const union {
    uint8_t bytes[2];
    uint16_t value;
} host_order = {{0, 1}};

#define HOST_ORDER (host_order.value)

#endif

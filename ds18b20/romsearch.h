#ifndef DS18B20_ROMSEARCH_H
#define DS18B20_ROMSEARCH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <inttypes.h>
extern uint8_t ds18b20search( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *romcnt, uint8_t *roms, uint16_t buflen );

#ifdef __cplusplus
}
#endif

#endif

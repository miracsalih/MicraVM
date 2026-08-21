#ifndef GCR_H
#define GCR_H

#include <stdint.h>

void PORT_CPU_PIN(uint8_t portid, bool value);
void PORT_DEV_PIN(uint8_t portid, bool value);
void PORT_IN(uint8_t portid);
void PORT_OUT(uint8_t portid);
bool PORT_IN_DEV(uint8_t portid);
bool PORT_IN_CPU(uint8_t portid);
int PORT_OUT_DEVC(uint8_t portid);

#endif
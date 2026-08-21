#ifndef IRQ_H
#define IRQ_H
#include <stdint.h>

struct IRQ_GC {
    bool irq_ack;
    uint8_t irq_id;
    uint8_t irq_port;
    uint32_t address;
};

extern IRQ_GC irq_gc;

void IRQ_DEVC(uint8_t id, uint8_t port);
uint8_t IRQ_CPU_AL(uint8_t wh);

#endif
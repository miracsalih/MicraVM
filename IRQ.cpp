#include <stdint.h>
#include "GCR.h"
#include "IRQ.h"

IRQ_GC irq_gc;

void IRQ_DEVC(uint8_t id, uint8_t port) {
    irq_gc.irq_id = id;
    irq_gc.irq_port = port;
    irq_gc.irq_ack = true;
    return;
}

uint8_t IRQ_CPU_AL(uint8_t wh) {
    if (wh == 0x0) {
        uint8_t irq = irq_gc.irq_ack;
        irq_gc.irq_ack = 0;
        return irq;
    }
    if (wh == 0x1) {
        uint8_t irq = irq_gc.irq_id;
        irq_gc.irq_id = 0;
        return irq;
    }
    if (wh == 0x2) {
        uint8_t irq = irq_gc.irq_port;
        irq_gc.irq_port = 0;
        return irq;
    }
    return 0;
}
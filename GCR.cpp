#include "GCR.h"
#include "CPU.h"
#include <stdint.h>

struct abc {
    int ports[256];
    bool dev[256];
    bool cpu[256];
};

abc abc;

void PORT_CPU_PIN(uint8_t portid, bool value) {
    abc.cpu[portid] = value;
}

void PORT_DEV_PIN(uint8_t portid, bool value) {
    abc.dev[portid] = value;
}

void PORT_IN(uint8_t portid) {
    abc.ports[portid] = cpu_reg.gcr;
}

void PORT_OUT(uint8_t portid) {
    cpu_reg.gcr = abc.ports[portid];
}

bool PORT_IN_DEV(uint8_t portid) {
    return abc.dev[portid];
}

bool PORT_IN_CPU(uint8_t portid) {
    return abc.cpu[portid];
}

int PORT_OUT_DEVC(uint8_t portid) {
    return abc.ports[portid];
}
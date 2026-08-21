#include "CPU.h"
#include "REB.h"
#include <stdint.h>

REB reb;

void REB_YAZ() {
    reb.ram[cpu_reg.badr] = cpu_reg.bgcr;
    return;
}

void REB_OKU() {
    cpu_reg.bgcr = reb.ram[cpu_reg.badr];
    return;
}
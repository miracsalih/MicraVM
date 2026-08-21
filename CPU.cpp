#include "CPU.h"
#include "REB.h"
#include "IRQ.h"
#include "GCR.h"
#include "MVS.h"
#include "BSD.h"
#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

CPU_REG cpu_reg;

struct EGER {
    bool big;           // Büyükse?
    bool small;         // Küçükse?
    bool equals;        // Eşitse?
    bool nequals;       // Eşit değilse?
};

EGER eger;

int* REG_TABLE[17] {
    &cpu_reg.ar,
    &cpu_reg.badr,
    &cpu_reg.bgcr,
    &cpu_reg.zr,
    &cpu_reg.gcr,
    &cpu_reg.r0,
    &cpu_reg.r1,
    &cpu_reg.r2,
    &cpu_reg.r3,
    &cpu_reg.r4,
    &cpu_reg.r5,
    &cpu_reg.r6,
    &cpu_reg.r7,
    &cpu_reg.bsd_data[0],
    &cpu_reg.bsd_data[1]
};

int* GET_REG(bool data1_2) {
    switch (data1_2 ? cpu_reg.bsd_data[0] : cpu_reg.bsd_data[1]) {
        case 0x01: return REG_TABLE[0];
        case 0x02: return REG_TABLE[1];
        case 0x03: return REG_TABLE[2];
        case 0x04: return REG_TABLE[3];
        case 0x05: return REG_TABLE[4];
        case 0x06: return REG_TABLE[5];
        case 0x07: return REG_TABLE[6];
        case 0x08: return REG_TABLE[7];
        case 0x09: return REG_TABLE[8];
        case 0x0A: return REG_TABLE[9];
        case 0x0B: return REG_TABLE[10];
        case 0x0C: return REG_TABLE[11];
        case 0x0D: return REG_TABLE[12];
        case 0x0E: return REG_TABLE[13];
        case 0x0F: return REG_TABLE[14];
        case 0x10: return REG_TABLE[15];
        case 0x11: return REG_TABLE[16];
        default: return nullptr;
    }
}

extern BSD bsd;

void CPU_OPCODI(uint16_t opcopr) {
    if (opcopr == 0x2) { // TAŞI (REG)
        int* b = GET_REG(false);
        if (!b) {
            return;
        }
        int* a = GET_REG(true);
        if (!a) {
            return;
        }
        *a = *b;
    } else if (opcopr == 0x3) { // TAŞI (IMM)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        *b = cpu_reg.bsd_data[1];
    }


    else if (opcopr == 0x4) { // EKLE (REG)
        int* b = GET_REG(false);
        if (!b) {
            return;
        }
        int* a = GET_REG(true);
        if (!a) {
            return;
        }
        *a = *a + *b;
    } else if (opcopr == 0x5) { // EKLE (IMM)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        *b += cpu_reg.bsd_data[1];
    }


    else if (opcopr == 0x6) { // ÇIKAR (REG)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        int* a = GET_REG(false);
        if (!a) {
            return;
        }
        *b -= *a;
    } else if (opcopr == 0x7) { // ÇIKAR (IMM)
        int* b = GET_REG(true);
        if (!b)
        {
            return;
        }
        int a = cpu_reg.bsd_data[1];
        *b -= a;
    }


    else if (opcopr == 0x8) { // ARR ++
        int* b = GET_REG(true);
        if (!b)
        {
            return;
        }
        *b += 1;
    } else if (opcopr == 0x9) { // ARR --
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        *b -= 1;
    }


    else if (opcopr == 0xA) { // BÖL (REG)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        int* a = GET_REG(false);
        if (!a) {
            return;
        }
        cpu_reg.ar = *b / *a;
        *b = *b % *a;
    } else if (opcopr == 0xB) { // BÖL (IMM)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        int a = cpu_reg.bsd_data[1];
        cpu_reg.ar = *b / a;
        *b = *b % a;
    }


    else if (opcopr == 0xC) { // ÇARP (REG)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        int* a = GET_REG(false);
        if (!a) {
            return;
        }
        cpu_reg.ar = *b * *a;
    } else if (opcopr == 0xD) { // ÇARP (IMM)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        int a = cpu_reg.bsd_data[1];
        cpu_reg.ar = *b * a;
    }


    else if (opcopr == 0xE) { // EĞER (REG)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        int* a = GET_REG(false);
        if (!a) {
            return;
        }
        if (*a == *b) {
            eger.equals = true;
        }
        if (*a > *b) {
            eger.small = true;
        }
        if (*a <*b) {
            eger.big = true;
        }
    } else if (opcopr == 0xF) { // EĞER (IMM)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        if (cpu_reg.bsd_data[1] == *b) {
            eger.equals = true;
        }
        if (cpu_reg.bsd_data[1] > *b) {
            eger.small = true;
        }
        if (cpu_reg.bsd_data[1] < *b) {
            eger.big = true;
        }
    }


    else if (opcopr == 0x10) { // BÜYÜK (REG)
        if (eger.big == true) {
            int* b = GET_REG(true);
            if (!b) {
                return;
            }
            bsd.pc = *b;
        }
    } else if (opcopr == 0x11) { // BÜYÜK (IMM)
        if (eger.big == true) {
            bsd.pc = cpu_reg.bsd_data[0];
        }
    }


    else if (opcopr == 0x12) { // KÜÇÜK (REG)
        if (eger.small == true) {
            int* b = GET_REG(true);
            if (!b) {
                return;
            }
            bsd.pc = *b;
        }
    } else if (opcopr == 0x13) { // KÜÇÜK (IMM)
        if (eger.small == true) {
            bsd.pc = cpu_reg.bsd_data[0];
        }
    }


    else if (opcopr == 0x14) { // DOĞRU (REG)
        if (eger.equals == true) {
            int* b = GET_REG(true);
            if (!b) {
                return;
            }
            bsd.pc = *b;
        }
    } else if (opcopr == 0x15) { // DOĞRU (IMM)
        if (eger.equals == true) {
            bsd.pc = cpu_reg.bsd_data[0];
        }
    }


    else if (opcopr == 0x16) { // YANLIŞ (REG)
        if (eger.nequals == true) {
            int* b = GET_REG(true);
            if (!b) {
                return;
            }
            bsd.pc = *b;
        }
    } else if (opcopr == 0x17) { // YANLIŞ (IMM)
        if (eger.equals == true) {
            bsd.pc = cpu_reg.bsd_data[0];
        }
    }


    else if (opcopr == 0x18) { // ZIPLA
        bsd.pc = cpu_reg.bsd_data[0];
    }


    else if (opcopr == 0x19) { // REB YAZ
        REB_YAZ();
    } else if (opcopr == 0x1A) { // REB OKU
        REB_OKU();
    }


    else if (opcopr == 0x1B) { // PORT YAZ (REG)
        PORT_IN(cpu_reg.gcr);
        PORT_CPU_PIN(cpu_reg.bsd_data[0], true);
    } else if (opcopr == 0x1C) { // PORT YAZ (IMM)
        PORT_IN(cpu_reg.gcr);
        PORT_CPU_PIN(cpu_reg.bsd_data[0], true);
    } else if (opcopr == 0x1D) { // PORT OKU (REG)
        PORT_OUT(cpu_reg.gcr);
        PORT_CPU_PIN(cpu_reg.bsd_data[0], false);
    } else if (opcopr == 0x1E) { // PORT OKU (IMM)
        PORT_OUT(cpu_reg.gcr);
        PORT_CPU_PIN(cpu_reg.bsd_data[0], false);
    }


    else if (opcopr == 0x1F) { // VE (REG)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        int* a = GET_REG(false);
        if (!a) {
            return;
        }
        if (*a & *b) {
            eger.equals = true;
        } else {
            eger.equals = false;
        }
    } else if (opcopr == 0x20) { // VE (IMM)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        if (cpu_reg.bsd_data[1] & *b) {
            eger.equals = true;
        } else {
            eger.equals = false;
        }
    }


    else if (opcopr == 0x21) { // VEYA (REG)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        int* a = GET_REG(false);
        if (!a) {
            return;
        }
        if (*a | *b) {
            eger.equals = true;
        } else {
            eger.equals = false;
        }
    } else if (opcopr == 0x22) { // VEYA (IMM)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        if (cpu_reg.bsd_data[1] | *b) {
            eger.equals = true;
        } else {
            eger.equals = false;
        }
    }


    else if (opcopr == 0x23) { // DEĞİL (REG)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        int* a = GET_REG(false);
        if (!a) {
            return;
        }
        if (*a != *b) {
            eger.equals = true;
        } else {
            eger.equals = false;
        }
    } else if (opcopr == 0x24) { // DEĞİL (IMM)
        int* b = GET_REG(true);
        if (!b) {
            return;
        }
        if (cpu_reg.bsd_data[1] != *b) {
            eger.equals = true;
        } else {
            eger.equals = false;
        }
    }


    else if (opcopr == 0x25) { // IRQ
        irq_gc.address = cpu_reg.bsd_data[0];
    }
}

int main(int argc, char* argv[]) {
    if (argc < 1) {
        cout << "TRBIN verilmedi!" << endl;
        return 0;
    }

    uint16_t op = 0;
    bsd.pc = 0;
    uint8_t ao = SGET_BIN(argv[1]);
    if (ao) {
        cout << "Hatalı dosya!" << endl;
        return 0;
    }

    if (argc < 2) {
        cout << "CONFIG verilmedi!" << endl;
        return 0;
    }

    ofstream cfg(argv[2]);
    int pc = bsd.pc;
    bsd.pc = 0;

    while (pc != 0) {
        if (IRQ_CPU_AL(0)) {
            cpu_reg.r0 = cpu_reg.gcr;
            cpu_reg.gcr = 0;
            uint8_t a = IRQ_CPU_AL(1);
            uint8_t b = IRQ_CPU_AL(2);

            cpu_reg.gcr = a | b;
            bsd.pc = irq_gc.address;
        }
        op = 0;
        cpu_reg.bsd_data[0] = bsd.DATA1[bsd.pc];
        cpu_reg.bsd_data[1] = bsd.DATA2[bsd.pc];
        if (bsd.OPCODE[bsd.pc] & 1) {
            op = 0x1;
        } else {
            op = 0x0;
        }
        op = ((bsd.OPCODE[bsd.pc] >> 8) & 0xff);
        CPU_OPCODI(op);
        cfg << "Alınan komut seti: " << (int)bsd.OPCODE[bsd.pc] << "|" << (int)op << "|" << (int)bsd.DATA1[bsd.pc] << (int)bsd.DATA2[bsd.pc] << endl;
        MVS(true);
        bsd.pc++;
        pc--;
    }

    MVS(false);
    return 0;
}
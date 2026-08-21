#ifndef BSD_H
#define BSD_H

#include <string>
#include <stdint.h>
using namespace std;

struct BSD {
    uint8_t OPCODE[1048577];
    uint8_t DATA1[1048577];
    int DATA2[1048577];
    int pc;
};

extern BSD bsd;

uint8_t DEBUG_BSD(string KOD);
uint8_t SGET_BIN(string KOD);

#endif
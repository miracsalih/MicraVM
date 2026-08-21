#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// Sadece struct ve extern tanımla
struct CPU_REG {
    int ar;
    int badr;
    int bgcr;
    int zr;
    int gcr;
    int r0,r1,r2,r3,r4,r5,r6,r7;
    int bsd_data[2];
};

extern CPU_REG cpu_reg;         // Tanım .cpp'de olacak
extern int* REG_TABLE[17]; // Tanım .cpp'de olacak

// Fonksiyon deklarasyonları
int* GET_REG(bool data1_2);
void CPU_OPCODI(uint16_t opcopr);

#endif
#ifndef REB_H
#define REB_H

struct REB
{
    uint8_t ram[524287] = {0x0};
};

extern REB reb;

void REB_YAZ();
void REB_OKU();

#endif
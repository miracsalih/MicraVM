#include <C:\msys64\ucrt64\include\SDL2\SDL.h>
#include <cstdint>
#include <iostream>
using namespace std;
#include "GCR.h"
#include "IRQ.h"
#include "MVS.h"

struct INFO {
    uint16_t x;
    uint16_t y;
    uint32_t color;
};

INFO info;

void MVS(bool active) {
    info.x = 0;
    info.y = 0;
    info.color = 0;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_Window* pencere = SDL_CreateWindow("MVS (640x360)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_SHOWN);
    SDL_Surface* yuzey = SDL_GetWindowSurface(pencere);

    if (!active) {
        SDL_Delay(3000);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
    }

    IRQ_DEVC(1, 0xA);
    if (!PORT_IN_CPU(0xA)) return;
    uint32_t port = PORT_OUT_DEVC(0xA);
    info.x = port & 640;
    info.y = (port >> 12) & 360;

    if (PORT_IN_CPU(0xB)) {
        port = PORT_OUT_DEVC(0xB);
        info.color = port;
    }

    if (info.x > 640 || info.y > 360) return;

    int* pikseller = (int*)yuzey->pixels;
    int genislik = yuzey->w;

    pikseller[info.x + info.y * genislik] = (int)SDL_MapRGBA(yuzey->format, (info.color >> 24) & 0xFF, (info.color >> 16) & 0xFF, (info.color >> 8) & 0xFF, info.color & 0xFF);

    SDL_UpdateWindowSurface(pencere);
    SDL_Delay(10);
}
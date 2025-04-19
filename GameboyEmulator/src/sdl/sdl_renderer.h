#pragma once
#include <cstdint>
#include <vector>

#include <SDL.h>

class SDLRenderer {
public:
    void Initialize();
    void Render(std::vector<uint8_t>& display);
private:
    static constexpr uint8_t scale = 10;
    static constexpr SDL_Color whiteColor = { 155, 188, 15, 255 };
    static constexpr SDL_Color lightGrayColor = { 139, 172, 15, 255 };
    static constexpr SDL_Color darkGrayColor = { 48, 98, 48, 255 };
    static constexpr SDL_Color blackColor = { 15, 56,15, 255 };

    SDL_Window* window;
    SDL_Renderer* renderer;
};
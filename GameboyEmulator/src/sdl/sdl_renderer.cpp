#include "sdl_renderer.h"

void SDLRenderer::Initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	window = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		160 * scale, 144 * scale, SDL_WINDOW_SHOWN);
}

void SDLRenderer::Render(std::vector<uint8_t>& display) {

}
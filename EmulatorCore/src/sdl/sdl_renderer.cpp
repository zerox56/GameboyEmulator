#include "sdl_renderer.h"

void SDLRenderer::Initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	window = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		160 * scale, 144 * scale, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
}

void SDLRenderer::Render(std::vector<uint8_t>& display) {
	SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch);

	for (int y = 0; y < 144; y++) {
		for (int x = 0; x < 160; x++) {
			uint8_t pixel = display[y * 160 + x];
			SDL_Color color = colors[pixel];
			pixels[y * (pitch / 4) + x] = (color.r << 16) | (color.g << 8) | color.b;;
		}
	}

	SDL_UnlockTexture(texture);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}
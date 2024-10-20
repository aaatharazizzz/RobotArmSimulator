#pragma once

#include <SDL.h>

#include <cinttypes>
#include <vector>
#include <array>

#include "Vector2.h"
#include "SDLRaii.h"

const int TYPERSCREEN_FONTSIZE = 12;

class TyperScreen {
public:
	TyperScreen(Vec2d pos, int screen_w, int screen_h);
	void AddCharacter(uint8_t c);
	void Draw(SDL_Renderer *renderer, std::array<sdl_raii::Texture, 256> &font_atlas);
private:
	Vec2d pos;
	int curr_r, curr_c;
	int screen_w, screen_h;
	std::vector<std::vector<uint8_t>> text;
};


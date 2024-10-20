#include "TyperScreen.h"

TyperScreen::TyperScreen(Vec2d pos, int screen_w, int screen_h) {
	this->pos = pos;
	this->screen_w = screen_w;
	this->screen_h = screen_h;
	this->curr_r = 0;
	this->curr_c = 0;
	for (int i = 0; i < screen_h; i++) {
		text.push_back(std::vector<uint8_t>(screen_w, 0));
	}

}

void TyperScreen::AddCharacter(uint8_t c) {
	if (c == '\r') {
		return;
	}
	if (c == '\n') {
		curr_r++;
		curr_c = 0;
	} else {
		text[curr_r][curr_c] = c;
		curr_c++;
	}
	if (curr_c >= screen_w) {
		curr_r++;
		curr_c = 0;
	}
	if (curr_r >= screen_h) {
		curr_r = screen_h - 1;
		for (int i = 0; i < screen_h; i++) {
			for (int j = 0; j < screen_w; j++) {
				if (i == screen_h - 1) {
					text[i][j] = 0;
				} else {
					text[i][j] = text[i + 1][j];
				}
			}
		}
	}
}

void TyperScreen::Draw(SDL_Renderer* renderer, std::array<sdl_raii::Texture, 256> &font_atlas) {
	int view_h;
	SDL_GetRendererOutputSize(renderer, NULL, &view_h);
	SDL_Rect outline_rect = { 
		pos.x - TYPERSCREEN_FONTSIZE/2,
		view_h - (pos.y + TYPERSCREEN_FONTSIZE/2),
		TYPERSCREEN_FONTSIZE * screen_w + TYPERSCREEN_FONTSIZE,
		TYPERSCREEN_FONTSIZE * screen_h + TYPERSCREEN_FONTSIZE 
	};
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &outline_rect);

	for (int i = 0; i < screen_h; i++) {
		for (int j = 0; j < screen_w; j++) {
			SDL_SetTextureColorMod(font_atlas[text[i][j]], 0, 255, 0);
			SDL_Rect draw_rect = { 
				pos.x + (j * TYPERSCREEN_FONTSIZE),
				view_h - (pos.y - (i * TYPERSCREEN_FONTSIZE)), 
				TYPERSCREEN_FONTSIZE,
				TYPERSCREEN_FONTSIZE
			};
			SDL_RenderCopy(renderer, font_atlas[text[i][j]], NULL, &draw_rect);
		}
	}
	
}



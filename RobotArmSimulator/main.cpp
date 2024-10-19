#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <stdexcept>
#include <array>
#include <fstream>
#include <sstream>

#include "SDLRaii.h"
#include "Vector2.h"
#include "InputState.h"
#include "Util.h"
#include "TyperRobot.h"
#include "TyperScreen.h"

const int FPS = 60;

int main(int argc, char **argv) {
	try {
		sdl_raii::Init init(SDL_INIT_EVERYTHING);
		sdl_raii::TTFInit ttf_init;
		sdl_raii::Window window("Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
		sdl_raii::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
		InputState input_state;
		SDL_Event sdl_event;
		TyperRobot typer_robot(Vec2d(128, 96), "res/typer_robot_cfg.txt");
		TyperScreen typer_screen(Vec2d(160, 580), 40, 30);
		sdl_raii::Font font("res/AcPlus_ToshibaSat_8x8.ttf", 16);
		std::array<sdl_raii::Texture, 256> font_atlas;
		for (int i = 0; i < 256; i++) {
			SDL_Surface* char_surface_raw = TTF_RenderGlyph_Solid(font, i, { 255, 255, 255, 255 });
			if (char_surface_raw == NULL) {
				char_surface_raw = TTF_RenderGlyph_Solid(font, ' ', {255, 255, 255, 255});
			}
			sdl_raii::Surface char_surface(char_surface_raw); 
			font_atlas[i].Load(SDL_CreateTextureFromSurface(renderer, char_surface));
		}

		std::ifstream read_file;
		if (argc >= 2) {
			read_file.open(argv[1]);
		} else {
			read_file.open("res/default.txt");
		}
		std::stringstream buffer;
		buffer << read_file.rdbuf();
		std::string text = buffer.str();
		
		size_t text_i = 0;

		uint32_t t0 = SDL_GetTicks();
		uint32_t t1 = SDL_GetTicks();
		float deltatime = (1000.0/FPS) / 1000.0;
		bool is_running = true;
		while (is_running) {
			while (SDL_PollEvent(&sdl_event)) {
				if (sdl_event.type == SDL_QUIT) {
					is_running = false;
				}
			}
			input_state.Update();
			
			t1 = SDL_GetTicks();
			if (t1 - t0 > deltatime * 1000) {
				typer_robot.StartTarget(text[text_i], .1, .1);
				typer_robot.Update(deltatime);
				if (!typer_robot.is_busy) {
					typer_screen.AddCharacter(text[text_i]);
					text_i++;
					if (text_i >= text.size()) {
						text_i = 0;
						typer_screen.AddCharacter('\n');
					}
				}

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);
				typer_robot.Draw(renderer, font_atlas);
				typer_screen.Draw(renderer, font_atlas);
				SDL_RenderPresent(renderer);
				t0 = t1;
			}
			input_state.UpdatePrevious();
		}
	}
	catch (std::runtime_error e) {
		printf(e.what());
	}
	return 0;
}

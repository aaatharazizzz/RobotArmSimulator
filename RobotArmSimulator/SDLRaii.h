#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <stdexcept>

namespace sdl_raii {

class Init {
public: 
	Init(uint32_t flags) {
		if (SDL_Init(flags) != 0) {
			std::string error_msg = "Unable to init SDL: ";
			error_msg.append(SDL_GetError());
			throw std::runtime_error(error_msg);
		}
	}
	~Init() { SDL_Quit(); }
};


class TTFInit {
public:
	TTFInit() {
		if (TTF_Init() == -1) {
			std::string error_msg = "Unable to init SDL_TTF: ";
			error_msg.append(SDL_GetError());
			throw std::runtime_error(error_msg);
		}
	}
	~TTFInit() { TTF_Quit(); }
};

class Window {
public:
	Window(const char* title, int x, int y, int w, int h, uint32_t flags) {
		m_window = SDL_CreateWindow(title, x, y, w, h, flags);
		if (m_window == nullptr) {
			std::string error_msg = "Unable to init SDL_Window: ";
			error_msg.append(SDL_GetError());
			throw std::runtime_error(error_msg);
		}
	}
	~Window() { SDL_DestroyWindow(m_window); };
	operator SDL_Window* () { return m_window; }
private:
	SDL_Window* m_window;
};

class Renderer {
public:
	Renderer(SDL_Window* window, int index, uint32_t flags) {
		m_renderer = SDL_CreateRenderer(window, index, flags);
		if (m_renderer == nullptr) {
			std::string error_msg = "Unable to init SDL_Renderer: ";
			error_msg.append(SDL_GetError());
			throw std::runtime_error(error_msg);
		}
	}
	~Renderer() { SDL_DestroyRenderer(m_renderer); };
	operator SDL_Renderer* () { return m_renderer; }
private:
	SDL_Renderer* m_renderer;
};

class Surface {
public:
	Surface() : m_surface(nullptr) {}
	Surface(SDL_Surface* surface) {
		m_surface = surface;
		if (m_surface == nullptr) {
			std::string error_msg = "Unable to init SDL_Surface: ";
			error_msg.append(SDL_GetError());
			throw std::runtime_error(error_msg);
		}
	}
	void Load(SDL_Surface* surface) {
		if (m_surface != nullptr) {
			SDL_FreeSurface(m_surface);
		}
		m_surface = surface;
		if (m_surface == nullptr) {
			std::string error_msg = "Unable to load SDL_Surface: ";
			error_msg.append(SDL_GetError());
			throw std::runtime_error(error_msg);
		}
	}
	~Surface() { SDL_FreeSurface(m_surface); };
	operator SDL_Surface* () { return m_surface; }
private:
	SDL_Surface* m_surface;
};

class Texture {
public:
	Texture() : m_texture(nullptr) {}
	Texture(SDL_Texture* texture) {
		m_texture = texture;
		if (m_texture == nullptr) {
			std::string error_msg = "Unable to init SDL_Texture: ";
			error_msg.append(SDL_GetError());
			throw std::runtime_error(error_msg);
		}
	}
	void Load(SDL_Texture* texture) {
		if (m_texture != nullptr) {
			SDL_DestroyTexture(m_texture);
		}
		m_texture = texture;
		if (m_texture == nullptr) {
			std::string error_msg = "Unable to load SDL_Texture: ";
			error_msg.append(SDL_GetError());
			throw std::runtime_error(error_msg);
		}
	}
	~Texture() { SDL_DestroyTexture(m_texture); };
	operator SDL_Texture* () { return m_texture; }
private:
	SDL_Texture* m_texture;
};


class Font {
public:
	Font(const char* file, int ptsize) {
		m_font = TTF_OpenFont(file, ptsize);
		if (m_font == nullptr) {
			std::string error_msg = "Unable to init TTF_Font (";
			error_msg.append(file);
			error_msg.append("): ");
			error_msg.append(SDL_GetError());
			throw std::runtime_error(error_msg);
		}
	}
	~Font() { TTF_CloseFont(m_font); }
	operator TTF_Font* () { return m_font; }
private:
	TTF_Font* m_font;
};

}
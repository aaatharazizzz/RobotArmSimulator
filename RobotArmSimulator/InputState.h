#pragma once

#include <SDL.h>

class InputState {
public:
	InputState();
	~InputState();
	void Update();
	void UpdatePrevious();
	bool KeyDown(SDL_Scancode scancode);
	bool KeyPressed(SDL_Scancode scancode);
private:
	int numkeys;
	const uint8_t* keystate;
	uint8_t* prev_keystate;
	
};


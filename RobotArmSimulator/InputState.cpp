#include "InputState.h"

#include <iterator>

InputState::InputState() {
	keystate = SDL_GetKeyboardState(&numkeys);
	prev_keystate = new uint8_t[numkeys];
	std::copy_n(keystate, numkeys, prev_keystate);
}

InputState::~InputState() {
	delete[] prev_keystate;
}

void InputState::Update() {
	keystate = SDL_GetKeyboardState(&numkeys);
}

void InputState::UpdatePrevious() {
	std::copy_n(keystate, numkeys, prev_keystate);
}

bool InputState::KeyDown(SDL_Scancode scancode) {
	return keystate[scancode];
}

bool InputState::KeyPressed(SDL_Scancode scancode) {

	return (!prev_keystate[scancode] && keystate[scancode]);
}

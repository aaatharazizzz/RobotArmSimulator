#pragma once

#include <SDL.h>

#include <vector>
#include <string>

#include "SDLRaii.h"
#include "Vector2.h"
#include "Arm.h"
#include "LerpStack.h"

enum class RobotKeyType {
	NonShifted,
	Shifted,
	Other,
	None
};

enum class RobotArmType {
	Left,
	Right,
	Shift
};

struct RobotKeyConfig {
	RobotKeyConfig() : r(0), c(0), w(0), type(RobotKeyType::None) {}
	RobotKeyConfig(int r, int c, int w, RobotKeyType type) : r(r), c(c), w(w), type(type) {}
	int r;
	int c;
	int w;
	RobotKeyType type;
};

enum class ArmLerpState {
	None,
	PressKey,
	Rebound
};

class TyperRobot {
public:
	TyperRobot(Vec2d pos, const char* config_file);
	void StartTarget(uint8_t c, double presskey_time, double rebound_time);
	void Update(float deltatime);
	void Draw(SDL_Renderer* renderer, std::array<sdl_raii::Texture, 256>& font_atlas);
	Vec2d pos;
	Arm left_arm;
	Arm right_arm;
	Arm shift_arm;
	bool is_busy;
private:
	bool is_pressing_shift;
	RobotArmType arm_lerp_type;
	LerpStack arm_lerp;
	LerpStack shift_arm_lerp;
	Vec2d left_arm_target;
	Vec2d right_arm_target;
	Vec2d shift_arm_target;
	std::vector<std::string> keyboard_noshift;
	std::vector<std::string> keyboard_w_shift;
	RobotKeyConfig key_config[256];
	RobotKeyConfig shiftkey_config;
	RobotKeyConfig target_config;
};
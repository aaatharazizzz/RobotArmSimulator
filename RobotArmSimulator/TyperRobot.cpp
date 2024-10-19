#include "TyperRobot.h"

#include <fstream>
#include <sstream>


#include "LerpStack.h"

TyperRobot::TyperRobot(Vec2d pos, const char* config_path) {
	this->pos = pos;
	this->left_arm = Arm(Vec2d(-50, 10) + pos, Limb(160), Limb(180), true);
	this->right_arm = Arm(Vec2d(544 + 30, 10) + pos, Limb(160, 180), Limb(180, 180), false);
	this->shift_arm = Arm(Vec2d(-50, -96) + pos, Limb(100), Limb(50), true);
	this->left_arm_target = Vec2d(pos.x, 20 + pos.y);
	this->right_arm_target = Vec2d(544 + pos.x, 20 + pos.y);
	left_arm.MoveArm(0 + pos.x, 20 + pos.y);
	right_arm.MoveArm(500 + pos.x, 20 + pos.y);
	this->arm_lerp = LerpStack(std::vector<LerpEntry>(3));
	this->shift_arm_lerp = LerpStack(std::vector<LerpEntry>(2));
	is_busy = false;
	is_pressing_shift = false;
	for (int i = 0; i < 256; i++) {
		key_config[i] = RobotKeyConfig();
	}
	std::ifstream config_file(config_path);
	if (config_file) {
		std::string read_line;
		std::getline(config_file, read_line);
		int keyboard_height = std::stoi(read_line);
		for (int i = 0; i < keyboard_height; i++) {
			std::getline(config_file, read_line);
			keyboard_noshift.push_back(read_line);
		}
		for (int i = 0; i < keyboard_height; i++) {
			std::getline(config_file, read_line);
			keyboard_w_shift.push_back(read_line);
		}
		while (std::getline(config_file, read_line)) {
			std::istringstream iss(read_line);
			std::string token;
			uint8_t ascii_code;
			bool write_shiftkey_config = false;
			std::getline(iss, token, ' ');
			if (token == "SHIFT") {
				write_shiftkey_config = true;
			}
			else {
				ascii_code = std::stoi(token);
			}
			std::getline(iss, token, ' ');
			int read_r = std::stoi(token);
			std::getline(iss, token, ' ');
			int read_c = std::stoi(token);
			std::getline(iss, token, ' ');
			int read_w = std::stoi(token);
			std::getline(iss, token, ' ');
			RobotKeyType read_type;
			switch (std::stoi(token)) {
			case 0: read_type = RobotKeyType::NonShifted; break;
			case 1: read_type = RobotKeyType::Shifted; break;
			case 2: read_type = RobotKeyType::Other; break;
			default: read_type = RobotKeyType::None; break;
			}
			if (!write_shiftkey_config) {
				key_config[ascii_code] = RobotKeyConfig(read_r, read_c, read_w, read_type);
			}
			else {
				shiftkey_config = RobotKeyConfig(read_r, read_c, read_w, read_type);
			}
		}
	}
	else {
		printf("Oops");
	}
	this->shift_arm_target = Vec2d(-5, 16) + pos + Vec2d(shiftkey_config.c + shiftkey_config.w / 2, -shiftkey_config.r) * FONT_SIZE + Vec2d(FONT_SIZE / 2, -FONT_SIZE / 2);
}

void TyperRobot::StartTarget(uint8_t c, double presskey_time, double rebound_time) {
	RobotKeyConfig curr_config = key_config[c];
	if (key_config[c].type == RobotKeyType::None) {
		return;
	}
	if (!is_busy) {
		target_config = curr_config;
		Vec2d follow_point = pos + Vec2d(curr_config.c + curr_config.w / 2, -curr_config.r) * FONT_SIZE + Vec2d(FONT_SIZE / 2, -FONT_SIZE / 2);
		double left_dist = left_arm.origin_pos.ManhattanTo(follow_point);
		double right_dist = right_arm.origin_pos.ManhattanTo(follow_point);
		if (left_dist < right_dist) {
			arm_lerp_type = RobotArmType::Left;
			arm_lerp.entries[0].target = left_arm_target;
			arm_lerp.entries[1].target = follow_point;
			arm_lerp.entries[2].target = follow_point + Vec2d(0, 5);
		}
		else {
			arm_lerp_type = RobotArmType::Right;
			arm_lerp.entries[0].target = right_arm_target;
			arm_lerp.entries[1].target = follow_point;
			arm_lerp.entries[2].target = follow_point + Vec2d(0, 5);
		}
		arm_lerp.entries[1].duration = presskey_time;
		arm_lerp.entries[2].duration = rebound_time;
		shift_arm_lerp.entries[0].target = shift_arm_target;
		shift_arm_lerp.entries[1].target = pos + Vec2d(shiftkey_config.c + shiftkey_config.w / 2, -shiftkey_config.r) * FONT_SIZE + Vec2d(FONT_SIZE / 2, -FONT_SIZE / 2);
		if (curr_config.type != RobotKeyType::Shifted) {
			shift_arm_lerp.entries[1].target += Vec2d(-5, 16);
		}
		shift_arm_lerp.entries[1].duration = presskey_time / 3;
		arm_lerp.Reset();
		shift_arm_lerp.Reset();
		is_busy = true;
	}
}

void TyperRobot::Update(float deltatime) {
	
	if (is_busy) {
		Vec2d arm_lerped = arm_lerp.Step(deltatime);
		
		if (arm_lerp_type == RobotArmType::Left) {
			left_arm.MoveArm(arm_lerped.x, arm_lerped.y);
		} else if (arm_lerp_type == RobotArmType::Right) {
			right_arm.MoveArm(arm_lerped.x, arm_lerped.y);
		}

		Vec2d shift_arm_lerped = shift_arm_lerp.Step(deltatime);
		if (!shift_arm_lerp.Finished()) {
			shift_arm.MoveArm(shift_arm_lerped.x, shift_arm_lerped.y);
		} else {
			is_pressing_shift = (target_config.type == RobotKeyType::Shifted);
			shift_arm_target = shift_arm_lerped;
		}

		if (arm_lerp.Finished()) {
			if (arm_lerp_type == RobotArmType::Left) {
				left_arm_target = arm_lerped;
			} else if (arm_lerp_type == RobotArmType::Right) {
				right_arm_target = arm_lerped;
			}
			is_busy = false;
		}
		
	}
}

void TyperRobot::Draw(SDL_Renderer* renderer, std::array<sdl_raii::Texture, 256>& font_atlas) {
	int view_h;
	SDL_GetRendererOutputSize(renderer, NULL, &view_h);
	std::vector<std::string>* keyboard_to_draw;
	if (is_pressing_shift) {
		keyboard_to_draw = &keyboard_w_shift;
	} else {
		keyboard_to_draw = &keyboard_noshift;
	}
	int line_ctr = 0;
	int col_ctr = 0;
	for (std::string& line : *keyboard_to_draw) {
		for (char c : line) {
			uint8_t atlas_index = (uint8_t)c;
			SDL_Rect char_draw_rect = { pos.x + col_ctr * 16, view_h - (pos.y - line_ctr * 16), 16, 16 };
			if ((arm_lerp.GetStepIndex() > 0 && line_ctr == target_config.r && col_ctr >= target_config.c && col_ctr < target_config.c + target_config.w)
				|| (is_pressing_shift && line_ctr == shiftkey_config.r && col_ctr >= shiftkey_config.c && col_ctr < shiftkey_config.c + shiftkey_config.w)) {
				SDL_SetTextureColorMod(font_atlas[atlas_index], 0, 255, 0);
			} else {
				SDL_SetTextureColorMod(font_atlas[atlas_index], 0, 100, 0);
			}
			SDL_RenderCopy(renderer, font_atlas[atlas_index], NULL, &char_draw_rect);
			col_ctr++;
		}
		col_ctr = 0;
		line_ctr++;
	}
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	left_arm.Draw(renderer);
	right_arm.Draw(renderer);
	shift_arm.Draw(renderer);

}

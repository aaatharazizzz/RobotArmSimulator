#pragma once

#include <vector>
#include <string>
#include <array>

#include <SDL_ttf.h>

#include "Util.h"
#include "Vector2.h"


const int FONT_SIZE = 16;

class Limb {
public:
	Limb() : angle_rad(0), length(0) {}
	Limb(double length, double angle_degrees = 0) : angle_rad(DegreesToRad(angle_degrees)), length(length) {}
	double angle_rad;
	double length;
};

class Arm {
public:
	Arm() {
		origin_pos = Vec2d();
		limbs[0] = limbs[1] = Limb();
		limb_ends[0] = limb_ends[1] = Vec2d();
		is_clockwise = false;
	};
	Arm(Vec2d origin_pos, Limb limb0, Limb limb1, bool is_clockwise) {
		this->origin_pos = origin_pos;
		limbs[0] = limb0;
		limbs[1] = limb1;
		limb_ends[0] = origin_pos + (MakeUnitVec2d(limbs[0].angle_rad) * limbs[0].length);
		limb_ends[1] = limb_ends[0] + (MakeUnitVec2d(limbs[1].angle_rad) * limbs[1].length);
		this->is_clockwise = is_clockwise;
	}
	void MoveArm(double target_x, double target_y);
	void Draw(SDL_Renderer *renderer);

	Vec2d origin_pos;
	Limb limbs[2];
	Vec2d limb_ends[2];
	bool is_clockwise;
};

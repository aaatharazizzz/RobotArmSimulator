#include "Arm.h"

void Arm::MoveArm(double target_x, double target_y) {
	if (origin_pos.x == target_x && origin_pos.y == target_y) {
		return;
	}
	double max_arm_length = limbs[0].length + limbs[1].length;
	Vec2d local_target = Vec2d(target_x, target_y) - origin_pos;
	if (local_target.Hypot() < abs(limbs[1].length - limbs[0].length)) {
		local_target = local_target.UnitVector() * abs(limbs[1].length - limbs[0].length);
	}
	double target_distance = local_target.Hypot();
	
	if (target_distance > max_arm_length) {
		limbs[0].angle_rad = local_target.GetAngle();
		limbs[1].angle_rad = 0;
	}
	else {
		double b = acos(
			(limbs[0].length * limbs[0].length + limbs[1].length * limbs[1].length - target_distance * target_distance)
			/ (2 * limbs[0].length * limbs[1].length)
		);
		double a = acos(
			(limbs[0].length * limbs[0].length + target_distance * target_distance - limbs[1].length * limbs[1].length)
			/ (2 * limbs[0].length * target_distance)
		);

		if (isnan(a) || isnan(b)) {
			return;
		}
		double a_prime = local_target.GetAngle();

		if (bends_clockwise) {
			limbs[0].angle_rad = a + a_prime;
			limbs[1].angle_rad = M_PI + b;
		}
		else {
			limbs[0].angle_rad = -a + a_prime;
			limbs[1].angle_rad = M_PI - b;
		}
	}

	limb_ends[0] = origin_pos + (MakeUnitVec2d(limbs[0].angle_rad) * limbs[0].length);
	limb_ends[1] = limb_ends[0] + (MakeUnitVec2d(limbs[1].angle_rad + limbs[0].angle_rad) * limbs[1].length);
}

void Arm::Draw(SDL_Renderer* renderer) {
	int view_h;
	SDL_GetRendererOutputSize(renderer, NULL, &view_h);
	SDL_RenderDrawLine(renderer, origin_pos.x, view_h - origin_pos.y, limb_ends[0].x, view_h - limb_ends[0].y);
	SDL_RenderDrawLine(renderer, limb_ends[0].x, view_h - limb_ends[0].y, limb_ends[1].x, view_h - limb_ends[1].y);
}



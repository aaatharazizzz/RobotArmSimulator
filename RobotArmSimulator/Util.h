#pragma once

#include <cmath>
#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif

constexpr double DegreesToRad(double degrees) {
	return (degrees * M_PI) / 180;
}
constexpr double RadToDegrees(double rad) {
	return (rad * 180) / M_PI;
}

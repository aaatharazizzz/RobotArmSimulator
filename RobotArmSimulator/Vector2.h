#pragma once
#include <type_traits>
#include <cmath>


struct Vec2d {
public:
	Vec2d() :
		x(0), y(0)
	{}
	Vec2d(double x, double y) : 
		x(x), y(y) 
	{}
	double x;
	double y;
	double GetAngle();
	double Hypot();
	double ManhattanTo(Vec2d rhs);
	Vec2d UnitVector();


	Vec2d operator+ (Vec2d rhs);
	Vec2d operator+= (const Vec2d& rhs);
	Vec2d operator- (Vec2d rhs);
	Vec2d operator-= (const Vec2d& rhs);
	Vec2d operator* (Vec2d rhs);
	Vec2d operator* (double rhs);
	Vec2d operator/ (double rhs);
};

Vec2d MakeUnitVec2d(double angle_rad);
Vec2d Vec2dLerp(Vec2d a, Vec2d b, float t);


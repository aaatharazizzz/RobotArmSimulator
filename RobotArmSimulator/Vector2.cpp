#include "Vector2.h"
#include <cmath>

double Vec2d::GetAngle()
{
	return atan2(y, x);
}

double Vec2d::Hypot()
{
	return hypot(x, y);
}

double Vec2d::ManhattanTo(Vec2d rhs)
{
	return abs(x - rhs.x) + abs(y - rhs.y);
}

Vec2d Vec2d::GetNormal()
{
	if (x == 0 && y == 0) {
		return Vec2d(0, 0);
	}
	return (Vec2d(x, y) / hypot(x, y));
}

Vec2d Vec2d::operator+(Vec2d rhs)
{
	return Vec2d(this->x + rhs.x, this->y + rhs.y);
}

Vec2d Vec2d::operator+=(const Vec2d& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

Vec2d Vec2d::operator-(Vec2d rhs)
{
	return Vec2d(this->x - rhs.x, this->y - rhs.y);
}

Vec2d Vec2d::operator-=(const Vec2d& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	return *this;
}

Vec2d Vec2d::operator*(Vec2d rhs)
{
	return Vec2d(x * rhs.x, y * rhs.x);
}

Vec2d Vec2d::operator*(double rhs)
{
	return Vec2d(x * rhs, y * rhs);
}

Vec2d Vec2d::operator/(double rhs)
{
	return Vec2d(x / rhs, y / rhs);
}

Vec2d MakeUnitVec2d(double angle_rad)
{
	return Vec2d(cos(angle_rad), sin(angle_rad));
}

Vec2d Vec2dLerp(Vec2d a, Vec2d b, float t)
{
	if (t < 0) {
		t = 0;
	} else if (t > 1) {
		t = 1;
	}

	return a + (b - a) * t;
}

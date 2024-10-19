#pragma once

#include "Vector2.h"
#include <vector>

struct LerpEntry {
	LerpEntry() : target(Vec2d()), duration(0) {}
	LerpEntry(Vec2d target, double duration) : target(target), duration(duration) {}
	Vec2d target;
	double duration;
};

class LerpStack {
public:
	LerpStack() : entries(std::vector<LerpEntry>()), elapsed_time(0), step_index(0) {}
	LerpStack(std::vector<LerpEntry> entries);
	Vec2d Step(double deltatime);
	void Reset();
	size_t GetStepIndex();
	bool Finished();
	std::vector<LerpEntry> entries;
private:
	double elapsed_time;
	size_t step_index;
};
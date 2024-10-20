#include "LerpStack.h"

LerpStack::LerpStack(std::vector<LerpEntry> entries) {
	this->entries = entries;
	this->elapsed_time = 0;
	this->step_index = 0;
}

Vec2d LerpStack::Step(double deltatime) {
	if (entries.empty()) {
		return Vec2d();
	}
	if (step_index < entries.size() - 1 && elapsed_time > entries[step_index + 1].duration) {
		step_index++;
		elapsed_time = 0;
	}
	if (Finished()) {
		return entries.back().target;
	}
	elapsed_time += deltatime;
	return Vec2dLerp(
		entries[step_index].target, 
		entries[step_index + 1].target, 
		(entries[step_index + 1].duration == 0 ? 1 : elapsed_time / entries[step_index + 1].duration)
	);
}

void LerpStack::Reset() {
	elapsed_time = 0;
	step_index = 0;
}

size_t LerpStack::GetStepIndex() {
	return step_index;
}

bool LerpStack::Finished() {
	return entries.size() <= 1 || step_index >= (entries.size() - 1);
}



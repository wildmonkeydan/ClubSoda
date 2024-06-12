#pragma once
#include "render.h"
#include <psxgte.h>

class Day {
public:
	Day();
	void Update();
	void Draw(RenderContext& ctx);
	CVECTOR GetSunlight();
private:
	CVECTOR InterpColour(const CVECTOR& origin, const CVECTOR& goal, int timer, int step);

	int timer;
	int interpTime;

	CVECTOR sunlight;

	CVECTOR top;
	CVECTOR bottom;
};
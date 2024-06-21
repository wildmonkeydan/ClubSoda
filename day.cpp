#include "day.h"

#include <inline_c.h>

#define setColour(c, red, blue, green) c.r = red; c.b = blue; c.g = green;

constexpr int CYCLE_TIME = 18000;
constexpr int HOUR = CYCLE_TIME / 24;
constexpr int SUNSET = HOUR * 9;
constexpr int MIDNIGHT = CYCLE_TIME / 2;
constexpr int SUNRISE = MIDNIGHT + (HOUR * 6);
constexpr int SUNMOVE = HOUR * 3;
constexpr int DAYBREAK = SUNRISE + SUNMOVE;

constexpr CVECTOR DAYBLUE = { 20, 130, 230, 0 };
constexpr CVECTOR NIGHTBLUE = { 0, 9, 150, 0 };
constexpr CVECTOR BLACK = { 0, 0, 0, 0 };
constexpr CVECTOR WHITE = { 255, 255, 255, 0 };
constexpr CVECTOR SUNORANGE = { 255, 144, 49, 0 };

Day::Day() {
	timer = 0;
	interpTime = 0;
	setColour(sunlight, 128, 128, 128);
	setColour(top, 20, 230, 130);
	setColour(bottom, 255, 255, 255);
}

void Day::Update() {
	timer++;

	if (timer > CYCLE_TIME) {
		timer = 0;
	}

	if (timer > SUNSET && timer < MIDNIGHT) {
		bottom = InterpColour(WHITE, SUNORANGE, SUNMOVE, timer - SUNSET);
		top = InterpColour(DAYBLUE, WHITE, SUNMOVE, timer - SUNSET);
	}
	else if(timer > MIDNIGHT && timer < SUNRISE){
		bottom = InterpColour(SUNORANGE, BLACK, SUNMOVE << 1, timer - MIDNIGHT);
		top = InterpColour(WHITE, NIGHTBLUE, SUNMOVE << 1, timer - MIDNIGHT);
	}
	else if (timer > SUNRISE && timer < DAYBREAK) {
		bottom = InterpColour(BLACK, SUNORANGE, SUNMOVE, timer - SUNRISE);
		top = InterpColour(NIGHTBLUE, WHITE, SUNMOVE, timer - SUNRISE);
	}
	else if (timer > DAYBREAK) {
		bottom = InterpColour(SUNORANGE, WHITE, SUNMOVE, timer - DAYBREAK);
		top = InterpColour(WHITE, DAYBLUE, SUNMOVE, timer - DAYBREAK);
	}
}

void Day::Draw(RenderContext& ctx) {
	auto sky = ctx.new_primitive<POLY_G4>(DEFAULT_OT_LENGTH - 1);

	setPolyG4(sky);
	setRGB0(sky, top.r, top.g, top.b);
	setRGB1(sky, top.r, top.g, top.b);
	setRGB2(sky, bottom.r, bottom.g, bottom.b);
	setRGB3(sky, bottom.r, bottom.g, bottom.b);
	setXY4(sky, SCREEN_XRES, 0,
		0, 0,
		SCREEN_XRES, SCREEN_YRES,
		0, SCREEN_YRES);
}

CVECTOR Day::InterpColour(const CVECTOR& origin, const CVECTOR& goal, int timer, int step) {
	CVECTOR result;
	SVECTOR diff;

	setVector(&diff, ((((goal.r - origin.r) << 10) / timer) * step) >> 10, ((((goal.g - origin.g) << 10) / timer) * step) >> 10, ((((goal.b - origin.b) << 10) / timer) * step) >> 10);
	setColour(result, origin.r + diff.vx, origin.b + diff.vz, origin.g + diff.vy);

	return result;
}

CVECTOR Day::GetSunlight() {
	return sunlight;
}
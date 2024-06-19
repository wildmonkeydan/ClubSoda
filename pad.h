#pragma once
#include <psxpad.h>
#include <psxgte.h>

#define LEFT_STICK 0
#define RIGHT_STICK 1

class Pad {
public:
	Pad();
	bool IsButtonDown(PadButton btn);
	DVECTOR GetStick(int stick);
	DVECTOR GetMouseDelta();
	bool IsFaceButtonDown();
	unsigned char GetType();

	char GetToolbarScroll();
	char GetToolScroll();
private:
	PADTYPE* pad = nullptr;
	char pad_buff[2][34];
};
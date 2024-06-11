#include "pad.h"

#include <psxapi.h>
#include <stdio.h>

static constexpr unsigned char stickDeadzone = 32;

Pad::Pad() {
	// Init BIOS pad driver and set pad buffers (buffers are updated
	// automatically on every V-Blank)
	InitPAD((uint8_t*)&pad_buff[0][0], 34, (uint8_t*)&pad_buff[1][0], 34);

	// Start pad
	StartPAD();

	// Don't make pad driver acknowledge V-Blank IRQ (recommended)
	ChangeClearPAD(0);

	pad = (PADTYPE*)&pad_buff[0][0];
}

bool Pad::IsButtonDown(PadButton btn) {
	if (pad->stat == 0) {
		if ((pad->type == PadTypeID::PAD_ID_DIGITAL) || (pad->type == PadTypeID::PAD_ID_ANALOG_STICK) || (pad->type == PadTypeID::PAD_ID_ANALOG) || (pad->type == PadTypeID::PAD_ID_MOUSE)) {
			return !(pad->btn & btn);
		}
	}

	return false;
}

DVECTOR Pad::GetStick(int stick) {
	DVECTOR out = { 0 };

	if (pad->stat == 0) {
		if (pad->type == PadTypeID::PAD_ID_ANALOG || pad->type == PadTypeID::PAD_ID_ANALOG_STICK) {
			if (stick == LEFT_STICK) {
				if ((pad->ls_x - 128) > stickDeadzone || (pad->ls_x - 128) < -stickDeadzone) {
					out.vx = pad->ls_x - 128;
				}

				if ((pad->ls_y - 128) > stickDeadzone || (pad->ls_y - 128) < -stickDeadzone) {
					out.vy = pad->ls_y - 128;
				}
			}
			else {
				if ((pad->rs_x - 128) > stickDeadzone || (pad->rs_x - 128) < -stickDeadzone) {
					out.vx = pad->rs_x - 128;
				}
				if ((pad->rs_y - 128) > stickDeadzone || (pad->rs_y - 128) < -stickDeadzone) {
					out.vy = pad->rs_y - 128;
				}
			}
		}
	}

	return out;
}

DVECTOR Pad::GetMouseDelta() {
	DVECTOR out = { 0 };

	if (pad->stat == 0) {
		out.vx = pad->x_mov;
		out.vy = pad->y_mov;
	}

	return out;
}

unsigned char Pad::GetType() {
	return pad->type;
}

bool Pad::IsFaceButtonDown() {
	return IsButtonDown(PAD_CIRCLE) || IsButtonDown(PAD_CROSS) || IsButtonDown(PAD_SQUARE) || IsButtonDown(PAD_TRIANGLE);
}
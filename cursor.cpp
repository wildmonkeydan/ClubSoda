#include "cursor.h"

Cursor::Cursor() {
	pos.vx = SCREEN_XRES>>1;
	pos.vy = SCREEN_YRES>>1;
}

void Cursor::Update(Pad& pad) {
	PadTypeID type = (PadTypeID)pad.GetType();
	DVECTOR delta = { 0 };

	if (type == PadTypeID::PAD_ID_MOUSE) {
		delta = pad.GetMouseDelta();
	}

	pos.vx += delta.vx;
	pos.vy += delta.vy;
}

void Cursor::Draw(RenderContext& ctx) {
	auto tile = ctx.new_primitive<TILE>(2);

	setTile(tile);
	setXY0(tile, pos.vx, pos.vy);
	setWH(tile, 32, 64);
	setRGB0(tile, 255, 0, 0);
}
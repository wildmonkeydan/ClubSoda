#include "cursor.h"

Cursor::Cursor() {
	pos.vx = SCREEN_XRES>>1;
	pos.vy = SCREEN_YRES>>1;
}

void Cursor::Update(Pad& pad) {
	PadTypeID type = (PadTypeID)pad.GetType();
	DVECTOR delta = { 0 };

	if (type == PadTypeID::PAD_ID_MOUSE && !pad.IsButtonDown(PadButton::MOUSE_RIGHT)) {
		delta = pad.GetMouseDelta();
	}

	pos.vx += delta.vx;
	pos.vy += delta.vy;
}

void Cursor::Draw(RenderContext& ctx) {
	

	auto tile = ctx.new_primitive<SPRT>(2);
	RECT texRect = GetTexRect();

	setSprt(tile);
	setXY0(tile, pos.vx, pos.vy);
	setUV0(tile, texRect.x, texRect.y);
	setWH(tile, texRect.w, texRect.h);
	setRGB0(tile, 128, 128, 128);
	setClut(tile, 0, 480);

	auto tpage = ctx.new_primitive<DR_TPAGE>(2);

	setDrawTPage(tpage, 0, 0, getTPage(0, 0, 768, 0));
}

RECT Cursor::GetTexRect() {
	RECT tex = { 0 };

	switch (state)
	{
	case Cursor::Norm:
		tex.w = 11;
		tex.h = 15;
		break;
	case Cursor::Hover:
		tex.x = 12;
		tex.w = 11;
		tex.h = 15;
		break;
	case Cursor::Up:
		setRECT(&tex, 17, 32, 14, 17);
		break;
	case Cursor::Down:
		setRECT(&tex, 0, 16, 14, 17);
		break;
	case Cursor::Left:
		setRECT(&tex, 0, 33, 17, 14);
		break;
	case Cursor::Right:
		setRECT(&tex, 14, 18, 17, 14);
		break;
	default:
		break;
	}

	return tex;
}

SVECTOR Cursor::GetRay() {
	
}
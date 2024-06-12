#include "cursor.h"
#include "island.h"

Cursor::Cursor() {
	pos.vx = SCREEN_XRES>>1;
	pos.vy = SCREEN_YRES>>1;
}

void Cursor::Update(Pad& pad, Island& isle) {
	PadTypeID type = (PadTypeID)pad.GetType();
	DVECTOR delta = { 0 };

	if (type == PadTypeID::PAD_ID_MOUSE && !pad.IsButtonDown(PadButton::MOUSE_RIGHT)) {
		delta = pad.GetMouseDelta();
	}

	pos.vx += delta.vx;
	pos.vy += delta.vy;

	if (pad.IsButtonDown(PadButton::MOUSE_LEFT) && toolCooldown == 0 && selectedTile != -1) {
		printf("Tool use\n");
		switch (tool)
		{
		case Cursor::Select:
			break;
		case Cursor::Terra:
			Terraform(isle);
			break;
		case Cursor::Place:
			break;
		case Cursor::Destroy:
			break;
		default:
			break;
		}

		toolCooldown = 15;
	}

	selectedTile = -1;
	if (toolCooldown) {
		toolCooldown--;
	}
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

bool Cursor::CheckTile(POLY_FT4* tile, int index) {
	if (selectedTile == -1) {
		bool inside = false;

		if (tile->x0 > 1000) {
			return false;
		}

		if (((tile->y0 > pos.vy && tile->y1 < pos.vy) || (tile->y0 < pos.vy && tile->y1 > pos.vy)) &&
			(pos.vx < (tile->x1 - tile->x0) * (pos.vy - tile->y0) / (tile->y1 - tile->y0) + tile->x0))
		{
			inside = !inside;
		}

		if (((tile->y1 > pos.vy && tile->y3 < pos.vy) || (tile->y1 < pos.vy && tile->y3 > pos.vy)) &&
			(pos.vx < (tile->x3 - tile->x1) * (pos.vy - tile->y1) / (tile->y3 - tile->y1) + tile->x1))
		{
			inside = !inside;
		}

		if (((tile->y3 > pos.vy && tile->y2 < pos.vy) || (tile->y3 < pos.vy && tile->y2 > pos.vy)) &&
			(pos.vx < (tile->x2 - tile->x3) * (pos.vy - tile->y3) / (tile->y2 - tile->y3) + tile->x3))
		{
			inside = !inside;
		}

		if (((tile->y2 > pos.vy && tile->y0 < pos.vy) || (tile->y2 < pos.vy && tile->y0 > pos.vy)) &&
			(pos.vx < (tile->x0 - tile->x2) * (pos.vy - tile->y2) / (tile->y0 - tile->y2) + tile->x2))
		{
			inside = !inside;
		}

		/*
		for (int i = 0, j = pointCount - 1; i < pointCount; j = i++)
		{
			if ((points[i].y > point.y) != (points[j].y > point.y) &&
				(point.x < (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x))
			{
				inside = !inside;
			}
		}*/

		if (inside) {
			selectedTile = index;
		}

		return inside;
	}
	else {
		return false;
	}
}

void Cursor::Terraform(Island& isle) {
	Tile* main = isle.GetTileAtIndex(selectedTile);
	main->AlterDepth((short)256);
}
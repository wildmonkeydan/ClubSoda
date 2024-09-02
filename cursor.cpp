#include "cursor.h"
#include "island.h"
#include "toolbar.h"
#include "utils.h"

#include <stdlib.h>

constexpr int SURROUND_OFFSETS[] = { WORLD_DIMENSION - 1, WORLD_DIMENSION, WORLD_DIMENSION + 1,
									 - 1,									1,
									 -WORLD_DIMENSION - 1, -WORLD_DIMENSION, -WORLD_DIMENSION + 1 };

constexpr char SURROUND_VERTS[][2] = { { 1, -1 }, { 1, 0 }, { 0, -1 },
									   { 1, 3 },		     { 0, 2 },
									   { 3, -1 } , { 3, 2 }, { 2, -1 } };

constexpr short	TERRA_STEP = 128;

constexpr char* OBJ_PATHS[] = { "FLAT.SMD;1", "ICE.SMD;1", "PALM.SMD;1" };
constexpr short OBJ_COST[] = { -500, -150, -20 };

Cursor::Cursor() {
	pos.vx = SCREEN_XRES>>1;
	pos.vy = SCREEN_YRES>>1;
}

void Cursor::Update(Pad& pad, Island& isle, Toolbar& toolbar, Bank& bank) {
	PadTypeID type = (PadTypeID)pad.GetType();
	DVECTOR delta = { 0 };

	delta = pad.GetMouseDelta();

	pos.vx += delta.vx;
	pos.vy += delta.vy;

	pos.vx = clamp(24, 640, pos.vx);
	pos.vy = clamp(24, 480, pos.vy);

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
			DoPlace(isle, toolbar, bank);
			break;
		case Cursor::Destroy:
			break;
		case Cursor::Paint:
			DoPaint(isle, toolbar, bank);
			break;
		case Cursor::Expand:
			DoExpand(isle, toolbar, bank);
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

		if (abs(tile->y0 - pos.vy) > 50) {
			//printf("Skipped %d\n", index);
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
	if (main->isLand) {
		main->AlterDepth(TERRA_STEP);

		int nextPos;

		for (int i = 0; i < 8; i++) {
			nextPos = selectedTile + SURROUND_OFFSETS[i];
			if (nextPos < NUM_TILES && nextPos > 0) {
				isle.GetTileAtIndex(nextPos)->MoveVerts(SURROUND_VERTS[i][0], TERRA_STEP, SURROUND_VERTS[i][1], TERRA_STEP);
			}
		}
	}
}

void Cursor::DoPlace(Island& isle, Toolbar& toolbar, Bank& bank) {
	Tile* main = isle.GetTileAtIndex(selectedTile);

	if (main->isLand && !main->hasObject) {
		if (bank.Alter(OBJ_COST[toolbar.GetCurrentObject()])) {
			isle.AddObject((Object::Type)toolbar.GetCurrentObject(), objs[toolbar.GetCurrentObject()], main);
		}
	}
}

void Cursor::DoPaint(Island& isle, Toolbar& toolbar, Bank& bank) {
	Tile* main = isle.GetTileAtIndex(selectedTile);

	if (main->GetMaterial() != toolbar.GetCurrentMaterial()) {
		if (main->isLand && bank.Alter(-20)) {
			main->SetMaterial(toolbar.GetCurrentMaterial());
		}
	}
}

void Cursor::DoExpand(Island& isle, Toolbar& toolbar, Bank& bank) {
	Tile* main = isle.GetTileAtIndex(selectedTile);

	main->isLand = !main->isLand;

	if (main->isLand) {
		if(bank.Alter(-100))
			main->SetMaterial(Tile::Beach);
	}
	else {
		main->SetMaterial(Tile::Waves);
		bank.Alter(100);
	}
}

void Cursor::LoadObjects(CD& cd) {
	for (int i = 0; i < NUM_OBJECTS; i++) {
		objDat[i] = cd.LoadFile(OBJ_PATHS[i]);
		objs[i] = (SMD*)objDat[i];
		smdInitData(objs[i]);
	}
}
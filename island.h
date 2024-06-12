#pragma once
#include "tile.h"

class Cursor;

class Island {
public:
	Island();
	void Update(Cursor& pad);
	void Draw(RenderContext& ctx, Cursor& cursor);
	Tile* GetTileAtIndex(int index);
private:
	Tile map[NUM_TILES];
	RECT normRect;
	RECT screen_clip;
	VECTOR pos;

	unsigned char timer = 0;
	unsigned char waterFrame = 0;
};
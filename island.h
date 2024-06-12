#pragma once
#include "tile.h"
#include "cursor.h"

class Island {
public:
	Island();
	void Update(Cursor& pad);
	void Draw(RenderContext& ctx, Cursor& cursor);
private:
	Tile map[NUM_TILES];
	RECT normRect;
	RECT screen_clip;
	VECTOR pos;

	unsigned char timer = 0;
	unsigned char waterFrame = 0;
};
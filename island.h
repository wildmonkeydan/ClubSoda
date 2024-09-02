#pragma once
#include "tile.h"
#include "object.h"

class Cursor;
constexpr int NUM_INSTANCES = 128;

class Island {
public:
	Island();
	void Update(Cursor& pad, Bank& bank);
	void Draw(RenderContext& ctx, Cursor& cursor, Camera& cam);
	Tile* GetTileAtIndex(int index);
	void AddObject(Object::Type type, SMD* smd, Tile* on);
private:
	Object objs[NUM_INSTANCES];
	Tile map[NUM_TILES];
	RECT normRect;
	RECT screen_clip;
	VECTOR pos;

	unsigned char timer = 0;
	unsigned char waterFrame = 0;
	unsigned char instanceIndex = 0;
};
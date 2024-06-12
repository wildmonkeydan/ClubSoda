#pragma once
#include <psxgte.h>
#include <psxgpu.h>

#include "render.h"
#include "cursor.h"


constexpr int WORLD_DIMENSION = 128;
constexpr int NUM_TILES = WORLD_DIMENSION * WORLD_DIMENSION;
constexpr int START_ISLAND_SIZE = 8;
constexpr int START_ISLAND_POS = 56;
constexpr int START_ISLAND_OFFSET = (START_ISLAND_POS * WORLD_DIMENSION) + (WORLD_DIMENSION / 2) - START_ISLAND_SIZE;
constexpr int START_ISLAND_END = START_ISLAND_OFFSET + START_ISLAND_SIZE * WORLD_DIMENSION;

class Tile {
public:
	enum Material {
		Sand,
		Beach,
		Grass,
		Forest,
		Waves,
		None
	};

	Tile();
	Tile(bool land, int pos);
	Tile(bool land, Material mat, int pos);

	void Update();
	void Draw(VECTOR origin, RenderContext& ctx, RECT& screen_clip, unsigned char waterFrame, Cursor& cursor);
	void AlterDepth(short addend);
	void AlterDepth(int absolute);
private:
	void InitVerts(int pos);
	void SetMatUV(POLY_FT4* poly, unsigned char waterFrame);

	bool isLand;
	Material material;
	VECTOR verts[4];
	SVECTOR localVerts[4];

	short xDiff;
	short zDiff;
	int index;
};
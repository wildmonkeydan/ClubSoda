#pragma once
#include "pad.h"
#include "tile.h"

constexpr int NUM_TOOLS = 6;

class Toolbar {
public:
	enum Tool {
		Select,
		Terra,
		Place,
		Destroy,
		Paint,
		Expand
	};

	Toolbar();
	void Update(Pad& pad, Cursor& cursor);
	void Draw(RenderContext& ctx);
	Tile::Material GetCurrentMaterial();
	int GetCurrentObject();
private:
	void DrawPaintBar(RenderContext& ctx);

	Tool tool;
	Tile::Material material;
	char scroll[NUM_TOOLS];
	unsigned char timer = 0;
};
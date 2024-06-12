#pragma once
#include "pad.h"
#include "render.h"

class Island;
class Tile;

class Cursor {
public:
	enum State {
		Norm,
		Hover,
		Up,
		Down,
		Left,
		Right
	};
	enum Tool {
		Select,
		Terra,
		Place,
		Destroy
	};

	Cursor();
	void Update(Pad& pad, Island& isle);
	void Draw(RenderContext& ctx);
	SVECTOR GetRay();
	bool CheckTile(POLY_FT4* tile, int index);
	
	DVECTOR pos;
	State state = Norm;
	Tool tool = Terra;
private:
	RECT GetTexRect();
	void Terraform(Island& isle);

	int selectedTile = -1;
	int toolCooldown = 0;
};
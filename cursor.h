#pragma once
#include "pad.h"
#include "render.h"

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

	Cursor();
	void Update(Pad& pad);
	void Draw(RenderContext& ctx);
	SVECTOR GetRay();
	bool CheckTile(POLY_FT4* tile, int index);
	
	DVECTOR pos;
	State state = Norm;
private:
	RECT GetTexRect();

	int selectedTile = -1;
};
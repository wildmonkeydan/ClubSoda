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
	
	DVECTOR pos;
	State state = Norm;
private:
	RECT GetTexRect();
};
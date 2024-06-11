#pragma once
#include "pad.h"
#include "render.h"

class Cursor {
public:
	Cursor();
	void Update(Pad& pad);
	void Draw(RenderContext& ctx);
	
	DVECTOR pos;
private:
};
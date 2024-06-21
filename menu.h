#pragma once
#include "pad.h"
#include <smd/smd.h>
#include "cd.h"
#include "camera.h"

class Menu {
public:
	Menu(CD& cd);
	bool Update(Pad& pad);
	void Draw(RenderContext& ctx, Camera& cam);
	void Unload();
private:
	uint32_t* sodaDat;
	SMD* soda;
	SC_OT ot;

	VECTOR sodaPos;
	SVECTOR sodaRot;
};
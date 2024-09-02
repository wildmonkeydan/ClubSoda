#pragma once

#include "tile.h"
#include "camera.h"
#include <smd/smd.h>

class Object {
public:
	enum Type {
		Flat,
		IceCream,
		PalmTrees
	};
	Object();
	Object(Type ty, SMD* mdl, Tile* on);
	void Update(Bank& bank);
	void Draw(RenderContext& ctx, Camera& cam, VECTOR origin);
	VECTOR pos;
private:
	bool active = false;
	int timer = 0;

	Type type;
	SMD* model;
	SC_OT ot;

	Tile* onTile;
	
	VECTOR relPos;
	SVECTOR rot;
	MATRIX omtx;
};
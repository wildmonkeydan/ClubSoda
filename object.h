#pragma once

#include "tile.h"
#include "camera.h"
#include <smd/smd.h>

class Object {
public:
	enum Type {
		Flat,
		BigHouse,
		Cottage,
		PalmTrees,
		DeckChairs,
		Pool
	};
	Object(Type ty, SMD* mdl, Tile* on);
	void Draw(RenderContext& ctx, Camera& cam);
private:
	Type type;
	SMD* model;
	SC_OT ot;

	Tile* onTile;
	VECTOR pos;
};
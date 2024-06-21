#include "object.h"

constexpr int OBJECT_OFFSETS[] = { 0, 0, 0, 0, 0, 0 };

Object::Object(Type ty, SMD* mdl, Tile* on) {
	type = ty;
	model = mdl;
	onTile = on;

	pos = onTile->PlaceObject(OBJECT_OFFSETS[type]);
}

void Object::Draw(RenderContext& ctx, Camera& cam) {

}
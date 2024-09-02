#include "object.h"
#include "utils.h"

#include <inline_c.h>
#include <stdlib.h>

constexpr int OBJECT_OFFSETS[] = { 0, 0, 0, 0, 0, 0 };
constexpr short OBJECT_PROFITS[] = { 20, 10, 0, 0, 0, 0 };

Object::Object() {
	active = false;
}

Object::Object(Type ty, SMD* mdl, Tile* on) {
	type = ty;
	model = mdl;
	onTile = on;

	pos = onTile->PlaceObject(OBJECT_OFFSETS[type]);
	setVector(&rot, -1024, 0, 2048);
	if (type == PalmTrees) {
		pos.vx += (rand() % 1024) - 512;
		pos.vz += (rand() % 1024) - 512;
	}
	active = true;
	onTile->hasObject = true;
	onTile->obj = this;
}

void Object::Update(Bank& bank) {
	if (active) {
		timer++;

		if (timer >= 600) {
			bank.Alter(OBJECT_PROFITS[type]);
		}

		timer = loop(0, 600, timer);
	}
}

void Object::Draw(RenderContext& ctx, Camera& cam, VECTOR origin) {
	if (active) {
		if ((pos.vx - origin.vx) > 10000 || (pos.vx - origin.vx) < 0)
			return;
		if ((pos.vz - origin.vz) > 10000 || (pos.vz - origin.vz) < 0)
			return;
		

		relPos.vx = pos.vx - origin.vx;
		relPos.vy = pos.vy;
		relPos.vz = pos.vz - origin.vz;
		
		// Set object rotation and position
		RotMatrix(&rot, &omtx);
		TransMatrix(&omtx, &relPos);

		// Composite coordinate matrix transform, so object will be rotated and
		// positioned relative to camera matrix (mtx), so it'll appear as
		// world-space relative.
		CompMatrixLV(cam.GetMatrix(), &omtx, &omtx);

		// Save matrix
		PushMatrix();

		// Set matrices
		gte_SetRotMatrix(&omtx);
		gte_SetTransMatrix(&omtx);

		ot.ot = ctx._buffers[ctx._active_buffer]._ot;
		ot.otlen = DEFAULT_OT_LENGTH;

		ctx._next_packet = smdSortModel(&ot, ctx._next_packet, model);


		// Restore matrix
		PopMatrix();
	}
}
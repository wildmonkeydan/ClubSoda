#include "menu.h"

#include <inline_c.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <stdlib.h>


constexpr CVECTOR back[2] = { { 255, 56, 8, 0}, { 255, 0, 0, 0} };


Menu::Menu(CD& cd) {
	sodaDat = cd.LoadFile("SODA.SMD;1");
	soda = (SMD*)sodaDat;
	smdInitData(soda);

	setVector(&sodaPos, 0, 0, 0);
	setVector(&sodaRot, 0, -ONE * 1024, 0);
}

bool Menu::Update(Pad& pad) {
	sodaRot.vz += 8;
	return pad.IsButtonDown(PadButton::MOUSE_LEFT);
}

void Menu::Draw(RenderContext& ctx, Camera& cam) {
	// Object matrix for player
	MATRIX omtx;

	// Set object rotation and position
	RotMatrix(&sodaRot, &omtx);
	TransMatrix(&omtx, &sodaPos);

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
	ot.zdiv = 1;
	ot.zoff = 0;

	ctx._next_packet = smdSortModel(&ot, ctx._next_packet, soda);


	// Restore matrix
	PopMatrix();


	auto sky = ctx.new_primitive<POLY_G4>(DEFAULT_OT_LENGTH - 1);

	setPolyG4(sky);
	setRGB0(sky, back[0].r, back[0].g, back[0].b);
	setRGB1(sky, back[0].r, back[0].g, back[0].b);
	setRGB2(sky, back[1].r, back[1].g, back[1].b);
	setRGB3(sky, back[1].r, back[1].g, back[1].b);
	setXY4(sky, SCREEN_XRES, 0,
		0, 0,
		SCREEN_XRES, SCREEN_YRES,
		0, SCREEN_YRES);
}

void Menu::Unload() {
	free(sodaDat);
	soda = nullptr;
}
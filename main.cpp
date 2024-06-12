/*
 * PSn00bSDK C++ basic graphics example
 * (C) 2020-2023 Lameguy64, spicyjpeg - MPL licensed
 *
 * A C++ variant of the beginner/hello example showcasing the use of classes and
 * templates in place of structures, making the code more readable and less
 * error-prone. The OT and primitive buffer are now allocated on the heap and
 * automatically freed when the RenderContext class is destroyed or goes out of
 * scope.
 *
 * See the original example for more details.
 */

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <psxgpu.h>
#include <stdlib.h>
#include <sys/types.h>

#include "render.h"
#include "cursor.h"
#include "camera.h"
#include "island.h"
#include "cd.h"
#include "utils.h"
#include "day.h"

/* Main */

int main(int argc, const char **argv) {
	// Initialize the GPU and load the default font texture provided by
	// PSn00bSDK at (960, 0) in VRAM.
	ResetGraph(0);
	FntLoad(960, 0);

	// Set up our rendering context.
	RenderContext ctx;
	ctx.setup(SCREEN_XRES, SCREEN_YRES, 63, 0, 127);

	VECTOR camPos = { ONE * 950, ONE * 1468, ONE * 950 };
	VECTOR camRot = { ONE * -5992, ONE * 1520, 0 };

	CD cd;
	cd.Init();
	Pad pad;
	Cursor cursor;
	Camera cam(camPos, camRot);
	Island isle;
	Day day;

	TIM_IMAGE img;

	u_long* tilesetDat = (u_long*)cd.LoadFile("TILES.TIM;1");
	LoadTexture(tilesetDat, &img);
	free(tilesetDat);

	u_long* iconsDat = (u_long*)cd.LoadFile("ICONS.TIM;1");
	LoadTexture(iconsDat, &img);
	free(iconsDat);

	int x  = 96, y  = 54;
	int dx = 1, dy = 1;
	int factor = UINT16_MAX / 120;

	//FntOpen(0, 0, 320, 240, 0, 256);

	for (;;) {		
		day.Update();
		day.Draw(ctx);
		cursor.Update(pad, isle);
		cursor.Draw(ctx);

		/*
		// Draw the square by allocating a TILE (i.e. untextured solid color
		// rectangle) primitive at Z = 1.
		auto tile = ctx.new_primitive<TILE>(1);


		setTile(tile);
		setXY0 (tile, x, y);
		setWH  (tile, 64, 64);
		setRGB0(tile, 255, 255, 0);*/

		cam.Update(pad, ctx);
		isle.Update(cursor);
		isle.Draw(ctx, cursor);

		// Draw some text in front of the square (Z = 0, primitives with higher
		// Z indices are drawn first).
		//ctx.draw_text(8, 16, 0, "Hello from C++!");

		ctx.flip();
	}

	return 0;
}

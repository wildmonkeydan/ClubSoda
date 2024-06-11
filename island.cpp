#include "island.h"
#include "utils.h"

Island::Island() {
	for (int i = 0; i < NUM_TILES; i++) {
		if (!(i > START_ISLAND_OFFSET && i < START_ISLAND_END)) {
			map[i] = Tile(false, i);
		}
		else {
			int x = i - (WORLD_DIMENSION * (i / WORLD_DIMENSION));

			if (i > START_ISLAND_POS && i < START_ISLAND_POS + START_ISLAND_SIZE) {
				map[i] = Tile(true, i);
			}
			else
			{
				map[i] = Tile(false, i);
			}
		}
	}

	setRECT(&normRect, 32, 32, 576, 416);
	// Set clip region
	setRECT(&screen_clip, 0, 0, SCREEN_XRES, SCREEN_YRES);
	setVector(&pos, 0, 0, 0);
}

void Island::Update(Cursor& pad) {
	int dir = test_clip(&normRect, pad.pos.vx, pad.pos.vy);

	if (dir & CLIP_LEFT) {
		pos.vx += 64;
		pos.vz -= 64;
	}
	if (dir & CLIP_RIGHT) {
		pos.vx -= 64;
		pos.vz += 64;
	}
	if (dir & CLIP_TOP) {
		pos.vz += 64;
		pos.vx += 64;
	}
	if (dir & CLIP_BOTTOM) {
		pos.vz -= 64;
		pos.vx -= 64;
	}

	if (pos.vx < 0) {
		pos.vx = 0;
	}
	else if (pos.vx >= 114688) {
		pos.vx = 114688;
	}

	if (pos.vz < 0) {
		pos.vz = 0;
	}
	else if (pos.vz >= 114688) {
		pos.vz = 114688;
	}

	timer++;

	printf("%d %d\n", pos.vx, pos.vz);

	if (timer >= 4) {
		waterFrame++;

		if (waterFrame == 8) {
			waterFrame = 0;
		}

		timer = 0;
	}
}

void Island::Draw(RenderContext& ctx) {

	int start = (pos.vx >> 10) + ((pos.vz >> 10) << 7);
	for (int i = start; i < start + 2048; i++) {
		//if (i - (i >> 7) > (pos.vx >> 10)) {
			map[i].Draw(pos, ctx, screen_clip, waterFrame);
		//}
	}
}
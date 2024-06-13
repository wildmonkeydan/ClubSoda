#include "tile.h"
#include "utils.h"

#include <inline_c.h>

// Make an invalid tile
Tile::Tile() {
	isLand = false;
	material = None;
}

// Make a tile with default materials
Tile::Tile(bool land, int pos) {
	isLand = land;

	if (land) {
		material = Sand;
	}
	else {
		material = Waves;
	}

	InitVerts(pos);
	index = pos;
}

Tile::Tile(bool land, Material mat, int pos) {
	isLand = land;
	material = mat;

	InitVerts(pos);
	index = pos;
}


void Tile::Draw(VECTOR origin, RenderContext& ctx, RECT& screen_clip, unsigned char waterFrame, Cursor& cursor) {

	for (int i = 0; i < 4; i++) {
		// Draw distance, for performance and memory reasons
		if ((verts[i].vx - origin.vx) > 12000 || (verts[i].vx - origin.vx) < -12000)
			return;
		if ((verts[i].vz - origin.vz) > 12000 || (verts[i].vz - origin.vz) < -12000)
			return;

		localVerts[i].vx = (verts[i].vx - origin.vx);
		localVerts[i].vy = verts[i].vy;
		localVerts[i].vz = (verts[i].vz - origin.vz);
	}

	int p;
	// Flat-shaded textured quad
	POLY_FT4* poly;

	// Get the next available space in the GPU queue
	poly = (POLY_FT4*)ctx._next_packet;

	// Load first three vertices to GTE
	gte_ldv3(
		&localVerts[0],
		&localVerts[1],
		&localVerts[2]);

	// Rotation, Translation, Perspective Triple  -  Process coords
	gte_rtpt();

	// Get the average distance of the coords from the camera, used for depth sorting
	gte_avsz3();
	// Retieve the result from the register
	gte_stotz(&p);


	// Checks if it's too far or too close/behind cam  -  basically near/far clipping
	if (((p >> 2) >= DEFAULT_OT_LENGTH) || ((p >> 2) <= 0))
		return;


	

	// Set the projected vertices to the primitive
	gte_stsxy0(&poly->x0);
	gte_stsxy1(&poly->x1);
	gte_stsxy2(&poly->x2);

	// Compute the last vertex and set the result
	gte_ldv0(&localVerts[3]);
	gte_rtps();
	gte_stsxy(&poly->x3);

	if (quad_clip(&screen_clip,
		(DVECTOR*)&poly->x0, (DVECTOR*)&poly->x1,
		(DVECTOR*)&poly->x2, (DVECTOR*)&poly->x3))
		return;
	
	setPolyFT4(poly);

	gte_avsz4();
	gte_stotz(&p);

	// Checks if it's too far or too close/behind cam  -  basically near/far clipping
	if (((p >> 2) >= DEFAULT_OT_LENGTH) || ((p >> 2) <= 0))
		return;

	bool hover = cursor.CheckTile(poly, index);
	unsigned char colour = hover ? 200 : 128;

	setRGB0(poly, colour, colour, colour);
	// Set 256x256 area in VRAM the quad can use for it's texture
	setTPage(poly, 1, 1, 640, 0);
	// Set the Colour LUT as the texture is 8-bit indexed 
	setClut(poly, 640, 472);
	SetMatUV(poly, waterFrame);

	// Add the polygon to the draw queue
	addPrim(ctx._buffers._ot + p, poly);
	poly++;

	ctx._next_packet = (uint8_t*)poly;
}

void Tile::SetMatUV(POLY_FT4* poly, unsigned char waterFrame) {
	unsigned char u, v;

	switch (material) {
	case Waves:
		u = 128 + ((waterFrame / 4) * 64);
		v = (waterFrame % 4) * 64;
		break;
	case Sand:
		u = 0;
		v = 0;
		break;
	}

	setUV4(poly, u, v, u + 63, v,
		u, v + 63, u + 63, v + 63);
}

void Tile::Update() {
	//timer++;
}

void Tile::InitVerts(int pos) {
	verts[0].vx = (pos % WORLD_DIMENSION) * 1024;
	verts[0].vy = 0;
	verts[0].vz = (pos / WORLD_DIMENSION) * 1024;

	verts[1].vx = (pos % WORLD_DIMENSION + 1) * 1024;
	verts[1].vy = 0;
	verts[1].vz = (pos / WORLD_DIMENSION) * 1024;

	verts[2].vx = (pos % WORLD_DIMENSION) * 1024;
	verts[2].vy = 0;
	verts[2].vz = (pos / WORLD_DIMENSION + 1) * 1024;

	verts[3].vx = (pos % WORLD_DIMENSION + 1) * 1024;
	verts[3].vy = 0;
	verts[3].vz = (pos / WORLD_DIMENSION + 1) * 1024;
}

void Tile::AlterDepth(short addend) {
	printf("added %d to tile %d\n", addend, index);

	for (int i = 0; i < 4; i++) {
		verts[i].vy += addend;
	}
}

void Tile::AlterDepth(int absolute) {
	for (int i = 0; i < 4; i++) {
		verts[i].vy = absolute;
	}
}

void Tile::MoveVerts(char index1, short value1, char index2, short value2) {
	printf("added %d to vert %d and %d to vert %d in tile %d\n", value1, index1, value2, index2, index);

	if (index1 != -1) {
		verts[index1].vy += value1;
	}

	if (index2 != -1) {
		verts[index2].vy += value2;
	}
}
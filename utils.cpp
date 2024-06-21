#include "utils.h"

int test_clip(RECT* clip, short x, short y) {

	// Tests which corners of the screen a point lies outside of

	int result = 0;

	if (x < clip->x) {
		result |= CLIP_LEFT;
	}

	if (x >= (clip->x + (clip->w - 1))) {
		result |= CLIP_RIGHT;
	}

	if (y < clip->y) {
		result |= CLIP_TOP;
	}

	if (y >= (clip->y + (clip->h - 1))) {
		result |= CLIP_BOTTOM;
	}

	return result;

}

int tri_clip(RECT* clip, DVECTOR* v0, DVECTOR* v1, DVECTOR* v2) {

	// Returns non-zero if a triangle is outside the screen boundaries

	short c[3];

	c[0] = test_clip(clip, v0->vx, v0->vy);
	c[1] = test_clip(clip, v1->vx, v1->vy);
	c[2] = test_clip(clip, v2->vx, v2->vy);

	if ((c[0] & c[1]) == 0)
		return 0;
	if ((c[1] & c[2]) == 0)
		return 0;
	if ((c[2] & c[0]) == 0)
		return 0;

	return 1;
}

int quad_clip(RECT* clip, DVECTOR* v0, DVECTOR* v1, DVECTOR* v2, DVECTOR* v3) {

	// Returns non-zero if a quad is outside the screen boundaries

	short c[4];

	c[0] = test_clip(clip, v0->vx, v0->vy);
	c[1] = test_clip(clip, v1->vx, v1->vy);
	c[2] = test_clip(clip, v2->vx, v2->vy);
	c[3] = test_clip(clip, v3->vx, v3->vy);

	if ((c[0] & c[1]) == 0)
		return 0;
	if ((c[1] & c[2]) == 0)
		return 0;
	if ((c[2] & c[3]) == 0)
		return 0;
	if ((c[3] & c[0]) == 0)
		return 0;
	if ((c[0] & c[2]) == 0)
		return 0;
	if ((c[1] & c[3]) == 0)
		return 0;

	return 1;
}

void LoadTexture(u_long* tim, TIM_IMAGE* tparam) {
	GetTimInfo((uint32_t*)tim, tparam);

	// Upload pixel data to framebuffer
	LoadImage(tparam->prect, tparam->paddr);
	DrawSync(0);

	// Upload CLUT to framebuffer
	if (tparam->mode & 0x8) {
		LoadImage(tparam->crect, tparam->caddr);
		DrawSync(0);
	}
}

bool CheckRecs(RECT& rec1, RECT& rec2) {
	return (rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y);
}

int clamp(int min, int max, int value) {
	if (value > max) {
		value = max;
	}

	if (value < min) {
		value = min;
	}

	return value;
}

int loop(int min, int max, int value) {
	if (value > max) {
		value = min;
	}

	if (value < min) {
		value = max;
	}

	return value;
}

void crossProduct(SVECTOR* v0, SVECTOR* v1, VECTOR* out) {

	out->vx = ((v0->vy * v1->vz) - (v0->vz * v1->vy)) >> 12;
	out->vy = ((v0->vz * v1->vx) - (v0->vx * v1->vz)) >> 12;
	out->vz = ((v0->vx * v1->vy) - (v0->vy * v1->vx)) >> 12;

}

void LookAt(VECTOR* eye, VECTOR* at, SVECTOR* up, MATRIX* mtx) {

	VECTOR taxis;
	SVECTOR zaxis;
	SVECTOR xaxis;
	SVECTOR yaxis;
	VECTOR pos;
	VECTOR vec;


	setVector(&taxis, at->vx - eye->vx, at->vy - eye->vy, at->vz - eye->vz);
	VectorNormalS(&taxis, &zaxis);
	crossProduct(&zaxis, up, &taxis);
	VectorNormalS(&taxis, &xaxis);
	crossProduct(&zaxis, &xaxis, &taxis);
	VectorNormalS(&taxis, &yaxis);

	mtx->m[0][0] = xaxis.vx;	mtx->m[1][0] = yaxis.vx;	mtx->m[2][0] = zaxis.vx;
	mtx->m[0][1] = xaxis.vy;	mtx->m[1][1] = yaxis.vy;	mtx->m[2][1] = zaxis.vy;
	mtx->m[0][2] = xaxis.vz;	mtx->m[1][2] = yaxis.vz;	mtx->m[2][2] = zaxis.vz;

	pos.vx = -eye->vx;;
	pos.vy = -eye->vy;;
	pos.vz = -eye->vz;;

	ApplyMatrixLV(mtx, &pos, &vec);
	TransMatrix(mtx, &vec);
}
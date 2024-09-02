#include "toolbar.h"
#include "utils.h"

constexpr char* TOOL_NAMES[] = { "Select", "Terraform - LMB Raise Ground", "Place - LMB Place Object", "Destroy - NOT IMPLEMENTED", "Paint - LMB Paint", "Expand - LMB Toggle Ground" };

Toolbar::Toolbar() {
	tool = Paint;
	material = Tile::Material::Grass;

	for (int i = 0; i < 6; i++) {
		scroll[i] = 0;
	}
}

void Toolbar::Update(Pad& pad, Cursor& cursor) {
	char move = 0;

	if (!timer) {
		move = pad.GetToolbarScroll();
		scroll[tool] += move;

		
		move = pad.GetToolScroll();
		char dst = cursor.tool + move;
		cursor.tool = (Cursor::Tool)loop(0, NUM_TOOLS - 1, dst);
		tool = (Tool)cursor.tool;

		timer = 15;
	}
	else {
		timer--;
	}

	switch (tool)
	{
	case Toolbar::Select:
		break;
	case Toolbar::Terra:
		break;
	case Toolbar::Place:
		break;
	case Toolbar::Destroy:
		break;
	case Toolbar::Paint:
		scroll[Paint] = loop(0, NUM_MAT, scroll[Paint]);

		material = (Tile::Material)scroll[Paint];
		break;
	default:
		break;
	}
}

void Toolbar::Draw(RenderContext& ctx) {
	switch (tool)
	{
	case Toolbar::Select:
		break;
	case Toolbar::Terra:
		break;
	case Toolbar::Place:
		break;
	case Toolbar::Destroy:
		break;
	case Toolbar::Paint:
		DrawPaintBar(ctx);
		break;
	default:
		break;
	}

	ctx.draw_text(16, 472, 0, TOOL_NAMES[tool]);
}

Tile::Material Toolbar::GetCurrentMaterial() {
	return material;
}

void Toolbar::DrawPaintBar(RenderContext& ctx) {
	auto back = ctx.new_primitive<TILE>(2);

	setTile(back);
	setXY0(back, 0, 408);
	setWH(back, 640, 72);
	setRGB0(back, 0, 35, 66);

	for (int i = 0; i < NUM_MAT; i++) {
		auto tile = ctx.new_primitive<SPRT>(1);

		setSprt(tile);
		setXY0(tile, (i << 6) + (i << 1), 412);
		setWH(tile, 64, 64);
		setUV0(tile, MAT_UVS[i][0], MAT_UVS[i][1]);
		setRGB0(tile, 128, 128, 128);
		setClut(tile, 640, 472);

		if (i == scroll[Paint]) {
			auto highlight = ctx.new_primitive<TILE>(1);

			setTile(highlight);
			setXY0(highlight, (i << 6) + (i << 1) - 1, 411);
			setWH(highlight, 66, 66);
			setRGB0(highlight, 255, 255, 0);
		}
	}

	auto tpage = ctx.new_primitive<DR_TPAGE>(1);

	setDrawTPage(tpage, 1, 1, getTPage(1, 1, 640, 0));
}

int Toolbar::GetCurrentObject() {
	return scroll[Place];
}
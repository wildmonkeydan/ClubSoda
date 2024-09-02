#pragma once
#include "pad.h"
#include "bank.h"
#include "cd.h"

#include <smd/smd.h>

class Island;
class Tile;
class Toolbar;

constexpr int NUM_OBJECTS = 3;

class Cursor {
public:
	enum State {
		Norm,
		Hover,
		Up,
		Down,
		Left,
		Right
	};
	enum Tool {
		Select,
		Terra,
		Place,
		Destroy,
		Paint,
		Expand
	};

	Cursor();
	void Update(Pad& pad, Island& isle, Toolbar& toolbar, Bank& bank);
	void Draw(RenderContext& ctx);
	SVECTOR GetRay();
	bool CheckTile(POLY_FT4* tile, int index);
	void LoadObjects(CD& cd);
	
	DVECTOR pos;
	State state = Norm;
	Tool tool = Tool::Paint;
private:
	RECT GetTexRect();
	void Terraform(Island& isle);
	void DoPlace(Island& isle, Toolbar& toolbar, Bank& bank);
	void DoPaint(Island& isle, Toolbar& toolbar, Bank& bank);
	void DoExpand(Island& isle, Toolbar& toolbar, Bank& bank);

	int selectedTile = -1;
	int toolCooldown = 0;

	SMD* objs[NUM_OBJECTS];
	uint32_t* objDat[NUM_OBJECTS];
};
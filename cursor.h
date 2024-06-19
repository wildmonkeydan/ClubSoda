#pragma once
#include "pad.h"
#include "bank.h"

class Island;
class Tile;
class Toolbar;

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
	
	DVECTOR pos;
	State state = Norm;
	Tool tool = Tool::Paint;
private:
	RECT GetTexRect();
	void Terraform(Island& isle);
	void DoPaint(Island& isle, Toolbar& toolbar, Bank& bank);
	void DoExpand(Island& isle, Toolbar& toolbar, Bank& bank);

	int selectedTile = -1;
	int toolCooldown = 0;
};
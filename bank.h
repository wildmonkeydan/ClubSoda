#pragma once
#include "render.h"

class Bank {
public:
	Bank();
	void Draw(RenderContext& ctx);
	bool Alter(short addend);
private:
	int money;
};
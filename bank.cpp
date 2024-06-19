#include "bank.h"

Bank::Bank() {
	money = 1000;
}

void Bank::Draw(RenderContext& ctx) {
	char moneyStr[24];
	snprintf(moneyStr, 24, "MONEY: %d", money);

	ctx.draw_text(0, 0, 0, moneyStr);
}

bool Bank::Alter(short addend) {
	if (money + addend < 0) {
		return false;
	}
	else {
		money += addend;
		return true;
	}
}
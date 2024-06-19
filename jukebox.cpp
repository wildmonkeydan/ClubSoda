#include "jukebox.h"
#include <psxcd.h>
#include <psxspu.h>
#include <stdio.h>
#include <stdlib.h>

Jukebox::Jukebox() {
	currentTrack = 2;
}

void Jukebox::Begin() {
	uint8_t param = CdlModeDA;
	CdControlB(CdlSetmode, &param, nullptr);
	param = itob(currentTrack);
	CdControl(CdlPlay, &param, nullptr);
}

void Jukebox::SetTrack(int track) {
	uint8_t param = CdlModeDA;
	CdControlB(CdlSetmode, &param, nullptr);
	param = itob(track);
	CdControl(CdlPlay, &param, nullptr);
}
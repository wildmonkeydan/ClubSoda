#pragma once

class Jukebox {
public:
	Jukebox();
	void Begin();
	void SetTrack(int track);
private:
	int currentTrack;
};
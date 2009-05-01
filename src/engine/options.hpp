#pragma once

#include "singleton.hpp"

class Options : public Singleton<Options>
{
public:
	Options();
	bool GetFullscreen() const;
	void SetFullscreen(bool);
	int GetWindowWidth() const;
	int GetWindowHeight() const;
	void Save() const;
	int GetStartJunks() const;
	int GetStartLevel() const;
	void SetStartLevel(int);
	void SetStartJunks(int);
private:
	int windowWidth_;
	int windowHeight_;
	bool fullscreen_;
	int startLevel_;
	int startJunks_;
};

Options& GetOptions(); // Easier access to the singleton

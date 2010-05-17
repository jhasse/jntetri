#pragma once

#include "singleton.hpp"

#include <string>

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
	void SetLastLoginName(const std::string&);
	std::string GetLastLoginName() const;
	void SetLastHighscoreName(const std::string&);
	std::string GetLastHighscoreName() const;
private:
	int windowWidth_;
	int windowHeight_;
	bool fullscreen_;
	int startLevel_;
	int startJunks_;
	std::string lastLoginName_;
	std::string lastHighscoreName_;
};

Options& GetOptions(); // Easier access to the singleton

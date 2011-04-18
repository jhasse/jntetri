#include "options.hpp"
#include "paths.hpp"

#include <fstream>

Options::Options() : windowWidth_(1024), windowHeight_(768), fullscreen_(false), startLevel_(0), startJunks_(0)
{
	const std::string filename = GetPaths().Config() + "options.txt";
	std::ifstream fin(filename.c_str());
	if(fin)
	{
		fin >> windowWidth_ >> windowHeight_ >> fullscreen_ >> startLevel_ >> startJunks_;
		std::getline(fin, lastLoginName_); // Ignore new line after startJunks_
		std::getline(fin, lastLoginName_);
		std::getline(fin, lastHighscoreName_);
	}
#ifdef WIZ
	windowWidth_ = 320;
	windowHeight_ = 240;
#endif
}

int Options::GetWindowWidth() const
{
	return windowWidth_;
}

int Options::GetWindowHeight() const
{
	return windowHeight_;
}

Vector2d Options::GetWindowVector() const
{
	return Vector2d(windowWidth_, windowHeight_);
}

bool Options::GetFullscreen() const
{
	return fullscreen_;
}

void Options::SetFullscreen(bool fullscreen)
{
	fullscreen_ = fullscreen;
}

void Options::Save() const
{
	std::ofstream fout((GetPaths().Config() + "options.txt").c_str());
	fout << windowWidth_  << " "
	     << windowHeight_ << " "
	     << fullscreen_
	     << std::endl << startLevel_ << " " << startJunks_
	     << std::endl << lastLoginName_
	     << std::endl << lastHighscoreName_;
}

Options& GetOptions()
{
	return Options::Handle();
}

int Options::GetStartLevel() const
{
	return startLevel_;
}

int Options::GetStartJunks() const
{
	return startJunks_;
}

void Options::SetStartLevel(int l)
{
	startLevel_ = l;
}

void Options::SetStartJunks(int j)
{
	startJunks_ = j;
}

void Options::SetLastLoginName(const std::string& name)
{
	lastLoginName_ = name;
}

std::string Options::GetLastLoginName() const
{
	return lastLoginName_;
}

void Options::SetLastHighscoreName(const std::string& name)
{
	lastHighscoreName_ = name;
}

std::string Options::GetLastHighscoreName() const
{
	return lastHighscoreName_;
}

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
		std::getline(fin, lastLoginName_); // Ignore new line after start startJunks_
		std::getline(fin, lastLoginName_);
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
	     << std::endl << lastLoginName_;
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

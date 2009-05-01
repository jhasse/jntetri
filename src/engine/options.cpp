#include "options.hpp"
#include "paths.hpp"

#include <fstream>
#include <boost/filesystem.hpp>

Options::Options() : windowWidth_(1024), windowHeight_(768), fullscreen_(false), startLevel_(0), startJunks_(0)
{
	const std::string filename = GetPaths().Config() + "options.txt";
	if(boost::filesystem::exists(filename))
	{
		std::ifstream fin(filename.c_str());
		fin >> windowWidth_ >> windowHeight_ >> fullscreen_ >> startLevel_ >> startJunks_;
	}
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
	     << fullscreen_;
	fout << std::endl << startLevel_ << " " << startJunks_;
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

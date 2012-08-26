#include "../resizegraphics.hpp"
#include "../paths.hpp"
#include "../options.hpp"
#include "../screen.hpp"

#include <jngl.hpp>
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#ifdef _MSC_VER
typedef size_t ssize_t;
#endif
#include <wand/magick_wand.h>

void ScanPath(boost::filesystem::path path, std::deque<std::string>& filesToResize)
{
	boost::filesystem::directory_iterator end;
	for(boost::filesystem::directory_iterator it(path); it != end; ++it)
	{
		if(boost::filesystem::is_directory(it->status()))
		{
			ScanPath(it->path(), filesToResize);
		}
		else
		{
			std::string file = it->path().string();
			std::cout << file << std::endl;
			if(file.substr(file.size() - 4) == ".png")
			{
				filesToResize.push_back(it->path().string());
			}
		}
	}
}

ResizeGraphics::ResizeGraphics() : originalSize_(-1)
{
	MagickWandGenesis();
	boost::filesystem::path path(GetPaths().Data() + "gfx");
	boost::filesystem::directory_iterator end;
	for(boost::filesystem::directory_iterator it(path); it != end; ++it)
	{
		if(boost::filesystem::is_directory(it->status()))
		{
			std::string name = it->path().string(); // e.g. /gfx/x1200
			try
			{
				originalSize_ = boost::lexical_cast<int>(name.substr(name.rfind("x") + 1));
				std::cout << "Original screen height: " << originalSize_ << std::endl;
			}
			catch(boost::bad_lexical_cast&)
			{
				// Bad cast, this doesn't seem to be the right directory
			}
		}
	}


	GetScreen().SetFactor(double(GetOptions().Get<int>("windowHeight")) / originalSize_);
	GetPaths().SetOriginalGfx(GetPaths().Data() + "gfx/x" + boost::lexical_cast<std::string>(originalSize_) + "/");
	GetPaths().SetGraphics(GetPaths().Config() + "x" + boost::lexical_cast<std::string>(GetOptions().Get<int>("windowHeight")) + "/");
	ScanPath(GetPaths().Data() + "gfx/x" + boost::lexical_cast<std::string>(originalSize_), filesToResize_);
}

ResizeGraphics::~ResizeGraphics() {
	MagickWandTerminus();
}

bool ResizeGraphics::Finished(double& percentage)
{
	static int numberOfImages = filesToResize_.size();
	percentage = 100 - filesToResize_.size() * 100 / numberOfImages;

	// Don't do anything in the first frame in order to draw the loading screen for the first time
	static bool firstFrame = true;
	if(firstFrame) {
		firstFrame = false;
		return false;
	}

	if(GetOptions().Get<int>("windowHeight") == originalSize_)
	{
		GetPaths().SetGraphics(GetPaths().Data() + "gfx/x" + boost::lexical_cast<std::string>(originalSize_) + "/");
		return true;
	}
	if(filesToResize_.empty())
	{
		return true;
	}

	std::string basedir(GetPaths().Data() + "gfx/x" + boost::lexical_cast<std::string>(originalSize_));
	std::string relativeFilename(filesToResize_[0].substr(basedir.size() + 1));
	std::string newFilename = GetPaths().Graphics() + relativeFilename;
	std::string writeTimeFilename = newFilename.substr(0, newFilename.find_last_of(".")) + ".txt";

	std::time_t newWriteTime = boost::filesystem::last_write_time(filesToResize_[0]);
	std::time_t oldWriteTime;
	{
		std::ifstream fin(writeTimeFilename.c_str());
		fin >> oldWriteTime;
	}
	if(oldWriteTime != newWriteTime) // Image file has changed
	{
		const double factor = GetScreen().GetFactor();
		MagickWand* wand = NewMagickWand();
		MagickReadImage(wand, filesToResize_[0].c_str());
		int width = MagickGetImageWidth(wand);
		int height = MagickGetImageHeight(wand);
		MagickResizeImage(wand, int(width * factor), int(height * factor), LanczosFilter, 1);

		boost::filesystem::create_directory(boost::filesystem::path(newFilename).remove_leaf());
		MagickWriteImage(wand, newFilename.c_str());
		DestroyMagickWand(wand);

		std::ofstream fout(writeTimeFilename.c_str());
		fout << boost::filesystem::last_write_time(filesToResize_[0]);
	}

	jngl::load(newFilename);
	filesToResize_.pop_front();
	return false;
}

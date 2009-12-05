#include "../resizegraphics.hpp"
#include "../paths.hpp"
#include "../options.hpp"
#include "../screen.hpp"

#include <Magick++.h>
#include <jngl.hpp>
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

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
			if(file.substr(file.size() - 4) == ".png")
			{
				filesToResize.push_back(it->path().string());
			}
		}
	}
}

ResizeGraphics::ResizeGraphics() : originalSize_(0)
{
	boost::filesystem::path path(GetPaths().Data() + "gfx");
	boost::filesystem::directory_iterator end;
	for(boost::filesystem::directory_iterator it(path); it != end; ++it)
	{
		if(boost::filesystem::is_directory(it->status()))
		{
			std::string name = it->path().string(); // e.g. /gfx/x1200
			try
			{
				originalSize_ = boost::lexical_cast<int>(name.substr(name.rfind("/") + 2));
				std::cout << "Original screen height: " << originalSize_ << std::endl;
			}
			catch(...)
			{
				// Bad cast, this doesn't seem to be the right directory
			}
		}
	}

	GetPaths().SetGraphics(GetPaths().Config() + "/x" + boost::lexical_cast<std::string>(GetOptions().GetWindowHeight()) + "/");
	ScanPath(GetPaths().Data() + "gfx/x" + boost::lexical_cast<std::string>(originalSize_), filesToResize_);
}

bool ResizeGraphics::Finished()
{
	if(GetOptions().GetWindowHeight() == originalSize_)
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
		const double factor = static_cast<double>(GetOptions().GetWindowHeight()) / double(originalSize_);
		Magick::Image image(filesToResize_[0]);
		image.zoom(Magick::Geometry(int(image.columns() * factor), int(image.rows() * factor)));

		jngl::SetFontColor(0, 0, 0);
		GetScreen().PrintCentered(std::string("Loading ") + relativeFilename, 0, 0);

		boost::filesystem::create_directory(newFilename.substr(0, newFilename.find_last_of("/")));
		image.write(newFilename);

		std::ofstream fout(writeTimeFilename.c_str());
		fout << boost::filesystem::last_write_time(filesToResize_[0]);
	}

	filesToResize_.pop_front();
	return false;
}

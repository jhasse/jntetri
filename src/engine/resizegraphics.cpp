#include "resizegraphics.hpp"
#include "paths.hpp"
#include "options.hpp"
#include "screen.hpp"

#include <Magick++.h>
#include <jngl.hpp>
#include <iostream>
#include <fstream>

ResizeGraphics::ResizeGraphics()
{
	ScanPath(GetPaths().Data() + "gfx/x1200");
}

void ResizeGraphics::ScanPath(boost::filesystem::path path)
{
	boost::filesystem::directory_iterator end;
	for(boost::filesystem::directory_iterator it(path); it != end; ++it)
	{
		if(boost::filesystem::is_directory(it->status()))
		{
			ScanPath(it->path());
		}
		else
		{
			std::string file = it->path().string();
			if(file.substr(file.size() - 4) == ".png")
			{
				filesToResize_.push_back(it->path().string());
			}
		}
	}
}

bool ResizeGraphics::Finished()
{
	if(GetOptions().GetWindowHeight() == 1200)
	{
		return true;
	}
	if(filesToResize_.empty())
	{
		return true;
	}

	std::string basedir(GetPaths().Data() + "gfx/x1200");
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
		const double factor = static_cast<double>(GetOptions().GetWindowHeight()) / 1200.0;
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

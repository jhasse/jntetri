#pragma once

#include <deque>
#include <string>
#include <boost/filesystem.hpp>

class ResizeGraphics
{
public:
	ResizeGraphics();
	bool Finished();
	void ScanPath(boost::filesystem::path);
private:
	std::deque<std::string> filesToResize_;
	int originalSize_;
};

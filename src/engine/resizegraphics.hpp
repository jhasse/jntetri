#pragma once

#include <deque>
#include <string>
#include <boost/filesystem.hpp>

class ResizeGraphics {
public:
	ResizeGraphics();
	~ResizeGraphics();
	bool isFinished(float& percentage);
private:
	std::deque<std::string> filesToResize_;
	int originalSize_;
};

void ScanPath(boost::filesystem::path path, std::deque<std::string>& filesToResize);
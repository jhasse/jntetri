#pragma once

#include <deque>
#include <string>
#include <filesystem>

class ResizeGraphics
{
public:
	ResizeGraphics();
	~ResizeGraphics();
	bool isFinished(float& percentage);
private:
	std::deque<std::string> filesToResize_;
	int originalSize_;
};

void ScanPath(std::tr2::sys::path path, std::deque<std::string>& filesToResize);

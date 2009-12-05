#pragma once

#include <deque>
#include <string>

class ResizeGraphics
{
public:
	ResizeGraphics();
	bool Finished();
private:
	std::deque<std::string> filesToResize_;
	int originalSize_;
};

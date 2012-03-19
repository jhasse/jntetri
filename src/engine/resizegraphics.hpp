#pragma once

#include <deque>
#include <string>

class ResizeGraphics
{
public:
	ResizeGraphics();
	~ResizeGraphics();
	bool Finished(double& percentage);
private:
	std::deque<std::string> filesToResize_;
	int originalSize_;
};

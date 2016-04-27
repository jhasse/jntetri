#pragma once

#include <deque>

#ifdef __linux__
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#else
	#include <filesystem>
	namespace fs = std::tr2::sys;
#endif

class ResizeGraphics {
public:
	ResizeGraphics();
	~ResizeGraphics();
	bool isFinished(float& percentage);
private:
	std::deque<std::string> filesToResize_;
	int originalSize_;
};

void ScanPath(fs::path path, std::deque<std::string>& filesToResize);

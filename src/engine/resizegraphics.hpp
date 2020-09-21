#pragma once

#include <deque>

#ifdef __linux__
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#else
	#ifdef _MSC_VER
		#include <filesystem>
		namespace fs = std::filesystem;
	#else
		#include <boost/filesystem.hpp>
		namespace fs = boost::filesystem;
	#endif
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

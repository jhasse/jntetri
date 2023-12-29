#include "resizegraphics.hpp"
#include "paths.hpp"

#include <jngl.hpp>
#include <boost/lexical_cast.hpp>

void ScanPath(fs::path path, std::deque<std::string>& filesToResize) {
	fs::directory_iterator end;
	for (fs::directory_iterator it(path); it != end; ++it) {
		if (fs::is_directory(it->status())) {
			ScanPath(it->path(), filesToResize);
		} else {
			std::string file = it->path().string();
			const std::string extension = ".webp";
			if (file.substr(file.size() - extension.size()) == extension) {
				filesToResize.push_back(std::string(it->path().string()).erase(0, jngl::getPrefix().size()));
			}
		}
	}
}

ResizeGraphics::ResizeGraphics() {
	jngl::setScaleFactor(static_cast<double>(jngl::getWindowHeight()) / 1200.);
	ScanPath(jngl::getPrefix() + "gfx", filesToResize_);
}

ResizeGraphics::~ResizeGraphics() {
}

bool ResizeGraphics::isFinished(float& percentage) {
	if (filesToResize_.empty()) {
		return true;
	}

	static size_t numberOfImages = filesToResize_.size();
	percentage = float(100 - filesToResize_.size() * 100 / numberOfImages);

	// Don't do anything in the first frame in order to draw the loading screen for the first time
	static bool firstFrame = true;
	if (firstFrame) {
		firstFrame = false;
		return false;
	}

	std::string newFilename = filesToResize_[0];

	jngl::load(newFilename.substr(0, newFilename.size()-5));
	filesToResize_.pop_front();
	return false;
}

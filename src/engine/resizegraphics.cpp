#include "resizegraphics.hpp"
#include "paths.hpp"
#include "options.hpp"
#include "screen.hpp"

#include <jngl.hpp>
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

void ScanPath(boost::filesystem::path path, std::deque<std::string>& filesToResize) {
	boost::filesystem::directory_iterator end;
	for (boost::filesystem::directory_iterator it(path); it != end; ++it) {
		if (boost::filesystem::is_directory(it->status())) {
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

ResizeGraphics::ResizeGraphics() : originalSize_(-1) {
	boost::filesystem::path path(jngl::getPrefix() + getPaths().getData() + "gfx");
	boost::filesystem::directory_iterator end;
	for (boost::filesystem::directory_iterator it(path); it != end; ++it) {
		if (boost::filesystem::is_directory(it->status())) {
			std::string name = it->path().string(); // e.g. /gfx/x1200
			try {
				auto tmp = boost::lexical_cast<int>(name.substr(name.rfind("x") + 1));
				if (tmp > originalSize_) {
					originalSize_ = tmp;
					jngl::debug("Original screen height: "); jngl::debugLn(originalSize_);
				}
			} catch(boost::bad_lexical_cast&) {
				// Bad cast, this doesn't seem to be the right directory
			}
		}
	}

	const std::string origGfx = getPaths().getData() + "gfx/x" + boost::lexical_cast<std::string>(originalSize_) + "/";
	getPaths().setOriginalGfx(origGfx);
	jngl::setScaleFactor(jngl::Float(jngl::getWindowHeight())/jngl::Float(originalSize_));
	getPaths().setGraphics(origGfx);
	ScanPath(jngl::getPrefix() + getPaths().getData() + "gfx/x" + boost::lexical_cast<std::string>(originalSize_), filesToResize_);
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

	std::string basedir;
	if (jngl::getPrefix() == "") {
		basedir = getPaths().getData() + "gfx/x" + boost::lexical_cast<std::string>(originalSize_);
	} else {
		basedir = "gfx/x" + boost::lexical_cast<std::string>(originalSize_);
	}
	std::string relativeFilename(filesToResize_[0].substr(basedir.size() + 1));
	std::string newFilename = getPaths().getGraphics() + relativeFilename;

	jngl::load(newFilename.substr(0, newFilename.size()-5));
	filesToResize_.pop_front();
	return false;
}

#include "options.hpp"
#include "paths.hpp"

#include <iostream>
#include <jngl/all.hpp>
#include <fstream>

Options::Options() : filename_(getPaths().getConfig() + "options.json") {
	const int BORDER = 50;
	const int dpiFactor = std::max(std::min(
		(jngl::getDesktopWidth() - BORDER) / windowWidth,
		(jngl::getDesktopHeight() - BORDER) / windowHeight
	), 1);
	windowWidth *= dpiFactor;
	windowHeight *= dpiFactor;
	try {
		std::ifstream ifs(filename_.c_str());
		cereal::JSONInputArchive archive(ifs);
		archive >> cereal::make_nvp("options", *this);
	} catch(std::exception& e) {
		std::cout << "Error loading " << filename_ << ": " << e.what() << std::endl;
	}
}

void Options::Save() const {
	std::ofstream ofs(filename_.c_str());
	cereal::JSONOutputArchive archive(ofs);
	archive << cereal::make_nvp("options", *this);
}

Options& GetOptions() {
	return *Options::handle();
}

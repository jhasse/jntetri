#include "Options.hpp"
#include "paths.hpp"

#include <cereal/archives/json.hpp>
#include <fstream>
#include <iostream>
#include <jngl.hpp>

Options::Options() : filename("options.json") {
	const int BORDER = 50;
	const int dpiFactor = std::max(std::min(
		(jngl::getDesktopWidth() - BORDER) / windowWidth,
		(jngl::getDesktopHeight() - BORDER) / windowHeight
	), 1);
	windowWidth *= dpiFactor;
	windowHeight *= dpiFactor;
	try {
		std::istringstream ifs(jngl::readConfig(filename));
		cereal::JSONInputArchive archive(ifs);
		archive >> cereal::make_nvp("options", *this);
	} catch(std::exception& e) {
		std::cout << "Error loading " << filename << ": " << e.what() << std::endl;
	}
}

void Options::Save() const {
	std::ostringstream ofs;
	{
		cereal::JSONOutputArchive archive(ofs);
		archive << cereal::make_nvp("options", *this);
	}
	jngl::writeConfig(filename, ofs.str());
}

Options& getOptions() {
	return *Options::handle();
}

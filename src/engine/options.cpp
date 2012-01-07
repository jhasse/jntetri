#include "options.hpp"
#include "paths.hpp"

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <iostream>
#include <fstream>

Options::Options() : filename_(GetPaths().Config() + "options.xml") {
	try {
		std::ifstream ifs(filename_.c_str());
		boost::archive::xml_iarchive xml(ifs);
		xml >> boost::serialization::make_nvp("options", *this);
	} catch(std::exception& e) {
		std::cout << "Error loading " << filename_ << ": " << e.what() << std::endl;
	}
}

void Options::Save() const {
	std::ofstream ofs(filename_.c_str());
	boost::archive::xml_oarchive xml(ofs);
	xml << boost::serialization::make_nvp("options", *this);
}

Options& GetOptions() {
	return Options::Handle();
}

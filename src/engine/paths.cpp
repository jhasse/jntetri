#include "paths.hpp"
#include "options.hpp"
#include "../constants.hpp"

#if defined (__linux__)
	#include "linux/binreloc.h"
#elif defined (__APPLE__)
	#ifndef __IPHONE_OS_VERSION_MIN_REQUIRED
		#include <mach-o/dyld.h>
		#include <CoreServices/CoreServices.h>
	#endif
#else
	#include <windows.h>
	#include <shlobj.h>
#endif
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <jngl.hpp>

std::string Paths::getResolutionGraphics() const {
	return (boost::format("%1%gfx/%2%x%3%/") % getData() % jngl::getScreenWidth() % jngl::getScreenHeight()).str();
}

#ifndef __IPHONE_OS_VERSION_MIN_REQUIRED
Paths::Paths() {
#if defined(__linux__)
	BrInitError error;
	if (br_init(&error) == 0 && error != BR_INIT_ERROR_DISABLED) {
		std::cout << "Warning: BinReloc failed to initialize (error code " << error << ")\n"
		          << "Will fallback to hardcoded default path." << std::endl;
	}
	auto tmp = br_find_prefix("/usr");
	prefix = tmp;
	free(tmp);
	prefix += '/';
	std::stringstream path;
	path << getenv("HOME") << "/.config/" << programShortName << "/";
	configPath_ = path.str();
#elif defined(__APPLE__)
	uint32_t size = 0;
	if (_NSGetExecutablePath(NULL, &size) == 0) {
		throw std::runtime_error("Can't get executable path!");
	} else {
		std::vector<char> tmp(size);
		_NSGetExecutablePath(&tmp[0], &size);
		prefix.assign(tmp.begin(), tmp.end());
		boost::filesystem::path prefixPath(prefix);
		prefix = prefixPath.normalize().remove_leaf().parent_path().string() + "/";
	}

	FSRef ref;
	FSFindFolder(kUserDomain, kApplicationSupportFolderType, kCreateFolder, &ref);
	char path[PATH_MAX];
	FSRefMakePath(&ref, (UInt8*)&path, PATH_MAX);
	boost::filesystem::path applicationSupportFolder(path);
	applicationSupportFolder /= programDisplayName;
	configPath = applicationSupportFolder.string() + "/";
#else
	char filename[MAX_PATH];
	GetModuleFileName(NULL, filename, MAX_PATH);
	std::string tmp(filename);
	prefix = tmp.substr(0, tmp.find("\\bin") + 1);

	TCHAR szPath[MAX_PATH];
	if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath))) {
		throw std::runtime_error("Couldn't get %AppData% location!");
	}
	std::stringstream path;
	path << szPath << "/" << programDisplayName << "/";
	configPath = path.str();
#endif
	boost::filesystem::create_directory(configPath);
#else
Paths::Paths() : configPath(jngl::getConfigPath()) {
#endif
}

std::string Paths::getGraphics() {
	return graphics;
}

std::string Paths::getFonts() const {
	return getData() + "fonts/";
}

void Paths::setGraphics(const std::string& g) {
	graphics = g;
}

std::string Paths::getData() const {
#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED
	return "";
#else
	return prefix + "data/";
#endif
}

std::string Paths::getConfig() {
	return configPath;
}

std::string Paths::getPrefix() {
	return prefix;
}

std::string Paths::getOriginalGfx() const {
	return originalGfx;
}

void Paths::setOriginalGfx(const std::string& o) {
	originalGfx = o;
}

void Paths::setPrefix(const std::string& p) {
	prefix = p + "/";
}

Paths& getPaths() {
	return *Paths::handle();
}

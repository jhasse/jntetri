#include "paths.hpp"
#include "options.hpp"
#include "../constants.hpp"

#ifdef _WIN32
	#include <windows.h>
	#include <shlobj.h>
#endif
#ifdef __APPLE__
	#include <CoreServices/CoreServices.h>
#endif
#include <iostream>
#include <sstream>
#ifdef __linux__
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#else
	#ifdef _MSC_VER
		#include <filesystem>
		namespace fs = std::tr2::sys;
	#else
		#include <boost/filesystem.hpp>
		namespace fs = boost::filesystem;
	#endif
#endif
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <jngl.hpp>

std::string Paths::getResolutionGraphics() const {
	return (boost::format("%1%gfx/%2%x%3%/") % getData() % jngl::getScreenWidth() % jngl::getScreenHeight()).str();
}

#ifndef __IPHONE_OS_VERSION_MIN_REQUIRED
Paths::Paths() {
#if defined(__linux__)
	fs::current_path(fs::path(jngl::getBinaryPath()) / fs::path(".."));

	std::stringstream path;
	path << getenv("HOME") << "/.config/" << programDisplayName << "/";
	configPath = path.str();
#elif defined(__APPLE__)
	fs::current_path(fs::path(jngl::getBinaryPath()) / fs::path(".."));

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
	fs::create_directory(configPath);
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

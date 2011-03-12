#include "paths.hpp"
#include "options.hpp"
#include "../constants.hpp"

#ifdef __linux__
#include "linux/binreloc.h"
#else
#include <windows.h>
#include <shlobj.h>
#endif
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

Paths::Paths()
{
#ifdef __linux__
	BrInitError error;
	if(br_init(&error) == 0 && error != BR_INIT_ERROR_DISABLED)
	{
		std::cout << "Warning: BinReloc failed to initialize (error code " << error << ")\n"
		          << "Will fallback to hardcoded default path." << std::endl;
	}
	prefix_ = br_find_prefix("/usr");
	prefix_ += '/';
	std::stringstream path;
	path << getenv("HOME") << "/.config/" << programShortName << "/";
	configPath_ = path.str();
#else
	char filename[MAX_PATH];
	int newSize = GetModuleFileName(NULL, filename, MAX_PATH);
	assert(newSize != 0 && newSize < MAX_PATH);
	std::string prefix(filename);
	prefix_ = prefix.substr(0, prefix.find("\\bin") + 1);

	TCHAR szPath[MAX_PATH];
	if(!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath)))
	{
		throw std::runtime_error("Couldn't get %AppData% location!");
	}
	std::stringstream path;
	path << szPath << "/" << programShortName << "/";
	configPath_ = path.str();
#endif
	boost::filesystem::create_directory(configPath_);
}

std::string Paths::Graphics()
{
	return graphics_;
}

void Paths::SetGraphics(const std::string& graphics)
{
	graphics_ = graphics;
}

std::string Paths::Data()
{
	return prefix_ + "share/" + programShortName + "/";
}

std::string Paths::Config()
{
	return configPath_;
}

std::string Paths::Prefix()
{
	return prefix_;
}

Paths& GetPaths()
{
	return Paths::Handle();
}

std::string Paths::OriginalGfx() const
{
	return originalGfx_;
}

void Paths::SetOriginalGfx(const std::string& originalGfx)
{
	originalGfx_ = originalGfx;
}

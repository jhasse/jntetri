#pragma once

#include "singleton.hpp"

#include <cereal/archives/json.hpp>

class Options : public Singleton<Options> {
public:
	Options();
	void Save() const;

	int windowWidth = 1280;
	int windowHeight = 720;
	bool fullscreen = false;
	unsigned int startJunks = 0;
	unsigned int startLevel = 0;
	std::string lastHighscoreName;
	std::string lastLoginName;

private:
	std::string filename_;

	friend class cereal::access;
	template <class Archive> void serialize(Archive& ar) {
		ar & CEREAL_NVP(windowWidth);
		ar & CEREAL_NVP(windowHeight);
		ar & CEREAL_NVP(fullscreen);
		ar & CEREAL_NVP(startJunks);
		ar & CEREAL_NVP(startLevel);
		ar & CEREAL_NVP(lastHighscoreName);
		ar & CEREAL_NVP(lastLoginName);
	}
};

Options& GetOptions(); // Easier access to the singleton

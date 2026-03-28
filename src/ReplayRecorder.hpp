#pragma once

#include "ControlType.hpp"
#include "gametype.hpp"

#include <sstream>

class Field;

class ReplayRecorder {
public:
	ReplayRecorder(Field&, GameType);
	void step();

	/// \a key will result in a file named <key>.jtr in config directory
	void save(std::string_view key, std::string_view playerName, int64_t score) const;

private:
	void write(ControlType);

	std::ostringstream fout;
	Field& field_;
	unsigned char time_;
};

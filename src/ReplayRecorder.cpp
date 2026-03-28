#include "ReplayRecorder.hpp"

#include "Field.hpp"
#include "engine/Options.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <sstream>

ReplayRecorder::ReplayRecorder(Field& field, const GameType gameType)
: fout("testReplay.jtr"), field_(field), time_(0) {
	fout << field_.getRandomSeed() << ' ' << getOptions().startLevel << ' '
	     << getOptions().startJunks << ' ' << static_cast<int>(gameType) << '\n';
}

void ReplayRecorder::step() {
	field_.getControl().forEach([this](ControlType c) { write(c); });
	if (time_ == 255) {
		time_ = 0;
		write(ControlType::Null);
	} else {
		++time_;
	}
}

void ReplayRecorder::save(std::string_view key, std::string_view playerName, int64_t score) const {
	jngl::writeConfig(std::format("{}.jtr", key), fout.str());
	// TODO: Save in binary format with cereal?
	// std::ostringstream tmp;
	// {
	// 	cereal::BinaryOutputArchive archive(tmp);
	// 	archive << cereal::make_nvp("playerName", std::string(playerName))
	// 	        << cereal::make_nvp("score", score) << fout.str();
	// }
	// jngl::writeConfig(std::format("{}.jtr", key), tmp.str());
}

void ReplayRecorder::write(ControlType c) {
	fout << static_cast<int>(time_) << ' ' << static_cast<int>(c) << std::endl;
}

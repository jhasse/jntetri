#include "replayrecorder.hpp"
#include "field.hpp"
#include "engine/options.hpp"

#include <boost/bind.hpp>

ReplayRecorder::ReplayRecorder(Field& field, const GameType gameType)
: fout("testReplay.jtr"), field_(field), time_(0) {
	fout << field_.getRandomSeed() << " " << getOptions().startLevel << " "
	     << getOptions().startJunks << " " << static_cast<int>(gameType) << std::endl;
}

void ReplayRecorder::Step() {
	field_.getControl().forEach([this](ControlType c) { write(c); });
	if(time_ == 255) {
		time_ = 0;
		write(ControlType::Null);
	} else {
		++time_;
	}
}

void ReplayRecorder::write(ControlType c) {
	fout << static_cast<int>(time_) << " " << static_cast<int>(c) << std::endl;
}

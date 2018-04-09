#include "replayrecorder.hpp"
#include "field.hpp"
#include "engine/options.hpp"

#include <boost/bind.hpp>

ReplayRecorder::ReplayRecorder(Field& field) : fout("testReplay.jtr"), field_(field), time_(0) {
	fout << field_.getRandomSeed() << " " << GetOptions().startLevel << " "
	     << GetOptions().startJunks << std::endl;
}

void ReplayRecorder::Step() {
	field_.getControl().forEach(boost::bind(&ReplayRecorder::Write, this, _1));
	if(time_ == 255) {
		time_ = 0;
		Write(control::Null);
	} else {
		++time_;
	}
}

void ReplayRecorder::Write(control::ControlType c) {
	fout << static_cast<int>(time_) << " " << c << std::endl;
}

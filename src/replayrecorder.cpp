#include "replayrecorder.hpp"
#include "field.hpp"

#include <boost/bind.hpp>

ReplayRecorder::ReplayRecorder(Field& field) : fout("testReplay.jtr"), field_(field), time_(0) {
	fout << field_.GetRandom().GetSeed() << std::endl;
}

void ReplayRecorder::Step() {
	field_.GetControl().ForEach(boost::bind(&ReplayRecorder::Write, this, _1));
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

#include "replaycontrol.hpp"

#include <fstream>
#include <cassert>

ReplayControl::ReplayControl(std::ifstream& fin) : time_(0) {
	while(fin) {
		int time;
		int control;
		fin >> time;
		fin >> control;
		assert(time >= 0 && time <= 255);
		assert(control >= 0 && control < control::LastValue);
		data_.push(std::pair<unsigned char, control::ControlType>(time, static_cast<control::ControlType>(control)));
	}
}

void ReplayControl::Step() {
	bits_.reset();
	if(time_ == data_.front().first) {
		Set(data_.front().second);
		data_.pop();
	}

	if(time_ == 255) {
		time_ = 0;
		assert(data_.front().second == control::Null);
		data_.pop();
	} else {
		++time_;
	}
}

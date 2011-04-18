#include "replaycontrol.hpp"

#include <fstream>
#include <cassert>
#include <boost/lexical_cast.hpp>

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
	static int line = 2;
	bits_.reset();
	while(!data_.empty() && time_ == data_.front().first) {
		Set(data_.front().second);
		++line;
		data_.pop();
	}
	if(data_.empty()) {
		return;
	}

	if(time_ == 255) {
		time_ = 0;
		if(data_.front().second != control::Null) {
			throw std::runtime_error("Error in line " + boost::lexical_cast<std::string>(line));
		}
		++line;
		data_.pop();
	} else {
		++time_;
	}
}

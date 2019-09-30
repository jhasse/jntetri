#include "ReplayControl.hpp"

#include <fstream>
#include <cassert>

ReplayControl::ReplayControl(std::ifstream& fin) : time_(0) {
	while(fin) {
		int time;
		int control;
		fin >> time;
		fin >> control;
		assert(time >= 0 && time <= 255);
		assert(control >= 0 && control < static_cast<int>(ControlType::LastValue));
		data_.push(std::pair<unsigned char, ControlType>(time, static_cast<ControlType>(control)));
	}
}

bool ReplayControl::step(const std::function<void(ControlType)>& Set) {
	static int line = 2;
	while(!data_.empty() && time_ == data_.front().first) {
		Set(data_.front().second);
		++line;
		data_.pop();
	}
	if(data_.empty()) {
		return true;
	}

	if(time_ == 255) {
		time_ = 0;
		if (data_.front().second != ControlType::Null) {
			throw std::runtime_error("Error in line " + std::to_string(line));
		}
		++line;
		data_.pop();
	} else {
		++time_;
	}
	return true;
}

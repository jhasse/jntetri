#pragma once

#include "work.hpp"

#include <boost/shared_ptr.hpp>

class Fade : public Work {
public:
	Fade(Work*, int speed = 6);
	virtual void Step();
	virtual void Draw() const;
private:
	Work* work_;
	boost::shared_ptr<Work> oldWork_;
	double fadeCount_;
	int speed_;
};

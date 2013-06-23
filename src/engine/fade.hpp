#pragma once

#include "work.hpp"

#include <boost/shared_ptr.hpp>

namespace jngl {
	class Work;
}

class Fade : public Work {
public:
	Fade(Work*, int speed = 15);
	virtual void step();
	virtual void draw() const;
private:
	Work* work_;
	std::shared_ptr<jngl::Work> oldWork_;
	double fadeCount_;
	int speed_;
};

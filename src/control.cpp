#include "control.hpp"

#include <jngl.hpp>

using namespace control;

Control::~Control()
{
}

void Control::Set(ControlType e) {
	bits_.set(e);
}

bool Control::Check(ControlType e) {
	return bits_.test(e);
}

void Control::ForEach(const boost::function<void(ControlType)>& f) {
	for(size_t i = 0; i < bits_.size(); ++i) {
		if(bits_.test(i)) {
			f(static_cast<control::ControlType>(i));
		}
	}
}


void KeyboardControl::Step() {
	bits_.reset();
	if(jngl::KeyPressed(jngl::key::Space) || jngl::KeyPressed(jngl::key::Return)) {
		Set(Drop);
	}
	if(jngl::KeyDown(jngl::key::Down)) {
		Set(Down);
	}
	if(jngl::KeyPressed(jngl::key::Left)) {
		Set(Left);
	}
	if(jngl::KeyPressed(jngl::key::Right)) {
		Set(Right);
	}
	if(jngl::KeyPressed(jngl::key::Up)) {
		Set(Rotate);
	}
	if(jngl::KeyPressed(jngl::key::ControlR)) {
		Set(RotateCounter);
	}
}

void WizControl::Step()
{
	bits_.reset();
	if(jngl::KeyPressed(jngl::key::WizX)) {
		Set(Drop);
	}
	if(jngl::KeyDown(jngl::key::WizDown)) {
		Set(Down);
	}
	if(jngl::KeyPressed(jngl::key::WizLeft)) {
		Set(Left);
	}
	if(jngl::KeyPressed(jngl::key::WizRight)) {
		Set(Right);
	}
	if(jngl::KeyPressed(jngl::key::WizUp) || jngl::KeyPressed(jngl::key::WizB)) {
		Set(Rotate);
	}
	if(jngl::KeyPressed(jngl::key::WizA)) {
		Set(RotateCounter);
	}
}

void WizControlRotated::Step() {
	bits_.reset();
	if(jngl::KeyPressed(jngl::key::WizA)) {
		Set(Drop);
	}
	if(jngl::KeyDown(jngl::key::WizLeft))  {
		Set(Down);
	}
	if(jngl::KeyPressed(jngl::key::WizUp)) {
		Set(Left);
	}
	if(jngl::KeyPressed(jngl::key::WizDown)) {
		Set(Right);
	}
	if(jngl::KeyPressed(jngl::key::WizRight) || jngl::KeyPressed(jngl::key::WizX)) {
		Set(Rotate);
	}
	if(jngl::KeyPressed(jngl::key::WizY)) {
		Set(RotateCounter);
	}
}

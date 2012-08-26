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
	if(jngl::keyPressed(jngl::key::Space) || jngl::keyPressed(jngl::key::Return)) {
		Set(Drop);
	}
	if(jngl::keyDown(jngl::key::Down) || jngl::keyDown('k')) {
		Set(Down);
	}
	if(jngl::keyPressed(jngl::key::Left) || jngl::keyPressed('j')) {
		Set(Left);
	}
	if(jngl::keyPressed(jngl::key::Right) || jngl::keyPressed('l')) {
		Set(Right);
	}
	if(jngl::keyPressed(jngl::key::Up) || jngl::keyPressed('i')) {
		Set(Rotate);
	}
	if(jngl::keyPressed(jngl::key::ControlR)) {
		Set(RotateCounter);
	}
}

void WizControl::Step()
{
	bits_.reset();
	if(jngl::keyPressed(jngl::key::WizX)) {
		Set(Drop);
	}
	if(jngl::keyDown(jngl::key::WizDown)) {
		Set(Down);
	}
	if(jngl::keyPressed(jngl::key::WizLeft)) {
		Set(Left);
	}
	if(jngl::keyPressed(jngl::key::WizRight)) {
		Set(Right);
	}
	if(jngl::keyPressed(jngl::key::WizUp) || jngl::keyPressed(jngl::key::WizB)) {
		Set(Rotate);
	}
	if(jngl::keyPressed(jngl::key::WizA)) {
		Set(RotateCounter);
	}
}

void WizControlRotated::Step() {
	bits_.reset();
	if(jngl::keyPressed(jngl::key::WizA)) {
		Set(Drop);
	}
	if(jngl::keyDown(jngl::key::WizLeft))  {
		Set(Down);
	}
	if(jngl::keyPressed(jngl::key::WizUp)) {
		Set(Left);
	}
	if(jngl::keyPressed(jngl::key::WizDown)) {
		Set(Right);
	}
	if(jngl::keyPressed(jngl::key::WizRight) || jngl::keyPressed(jngl::key::WizX)) {
		Set(Rotate);
	}
	if(jngl::keyPressed(jngl::key::WizY)) {
		Set(RotateCounter);
	}
}

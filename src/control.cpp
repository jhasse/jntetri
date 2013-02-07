#include "control.hpp"

#include <jngl/input.hpp>

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

void GamepadControl::Step() {
	bits_.reset();
	if (jngl::getControllerState(0, jngl::controller::A)) {
		Set(Drop);
	}
	if (jngl::getControllerState(0, jngl::controller::RightTrigger)) {
		Set(Down);
	}
	if (jngl::getControllerState(0, jngl::controller::DpadLeft)) {
		Set(Left);
	}
	if (jngl::getControllerState(0, jngl::controller::DpadRight)) {
		Set(Right);
	}
	if (jngl::getControllerState(0, jngl::controller::B)) {
		Set(Rotate);
	}
	if (jngl::getControllerState(0, jngl::controller::X)) {
		Set(RotateCounter);
	}
}
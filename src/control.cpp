#include "control.hpp"

#include <jngl/Controller.hpp>
#include <jngl/input.hpp>

using namespace control;

ControlBase::~ControlBase() {
}

Control::Control(std::initializer_list<std::shared_ptr<ControlBase>> l) : controls(l) {
}

Control::~Control() {
}

bool Control::Check(ControlType e) {
	return bits_.test(e);
}

void Control::forEach(const boost::function<void(ControlType)>& f) {
	for (size_t i = 0; i < bits_.size(); ++i) {
		if (bits_.test(i)) {
			f(static_cast<control::ControlType>(i));
		}
	}
}

void Control::step() {
	bits_.reset();
	for (auto& control : controls) {
		control->step([&](control::ControlType e) {
			bits_.set(e);
		});
	}
}

void KeyboardControl::step(std::function<void(control::ControlType)> Set) {
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
	if(jngl::keyPressed(jngl::key::ControlR) || jngl::keyPressed(jngl::key::AltR)) {
		Set(RotateCounter);
	}
}

void GamepadControl::step(std::function<void(control::ControlType)> Set) {
	if (!controller) { return; }
	if (controller->pressed(jngl::controller::A)) {
		Set(Drop);
	}
	if (controller->down(jngl::controller::RightTrigger)) {
		Set(Down);
	}
	if (controller->pressed(jngl::controller::DpadLeft)) {
		Set(Left);
	}
	if (controller->pressed(jngl::controller::DpadRight)) {
		Set(Right);
	}
	if (controller->pressed(jngl::controller::X)) {
		Set(Rotate);
	}
	if (controller->pressed(jngl::controller::B)) {
		Set(RotateCounter);
	}
}

GamepadControl::GamepadControl(const size_t number) {
	const auto controllers = jngl::getConnectedControllers();
	if (controllers.size() > number) {
		controller = controllers[number];
	}
}

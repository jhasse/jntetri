#include "control.hpp"

#include <jngl/Controller.hpp>
#include <jngl/input.hpp>

ControlBase::~ControlBase() {
}

Control::Control(std::initializer_list<std::shared_ptr<ControlBase>> l) : controls(l) {
}

Control::~Control() = default;

bool Control::Check(ControlType e) {
	return bits_.test(static_cast<size_t>(e));
}

void Control::forEach(const std::function<void(ControlType)>& f) {
	for (size_t i = 0; i < bits_.size(); ++i) {
		if (bits_.test(i)) {
			f(static_cast<ControlType>(i));
		}
	}
}

void Control::step() {
	bits_.reset();
	for (auto& control : controls) {
		control->step([&](ControlType e) {
			bits_.set(static_cast<size_t>(e));
		});
	}
}

void KeyboardControl::step(const std::function<void(ControlType)>& Set) {
	if(jngl::keyPressed(jngl::key::Space) || jngl::keyPressed(jngl::key::Return)) {
		Set(ControlType::Drop);
	}
	if(jngl::keyDown(jngl::key::Down) || jngl::keyDown('k')) {
		Set(ControlType::Down);
	}
	if(jngl::keyPressed(jngl::key::Left) || jngl::keyPressed('j')) {
		Set(ControlType::Left);
	}
	if(jngl::keyPressed(jngl::key::Right) || jngl::keyPressed('l')) {
		Set(ControlType::Right);
	}
	if(jngl::keyPressed(jngl::key::Up) || jngl::keyPressed('i')) {
		Set(ControlType::Rotate);
	}
	if(jngl::keyPressed(jngl::key::ControlR) || jngl::keyPressed(jngl::key::AltR)) {
		Set(ControlType::RotateCounter);
	}
}

void GamepadControl::step(const std::function<void(ControlType)>& Set) {
	if (!controller) { return; }
	if (controller->pressed(jngl::controller::A)) {
		Set(ControlType::Drop);
	}
	if (controller->down(jngl::controller::RightTrigger)) {
		Set(ControlType::Down);
	}
	if (controller->pressed(jngl::controller::DpadLeft)) {
		Set(ControlType::Left);
	}
	if (controller->pressed(jngl::controller::DpadRight)) {
		Set(ControlType::Right);
	}
	if (controller->pressed(jngl::controller::X)) {
		Set(ControlType::Rotate);
	}
	if (controller->pressed(jngl::controller::B)) {
		Set(ControlType::RotateCounter);
	}
}

GamepadControl::GamepadControl(const size_t number) {
	const auto controllers = jngl::getConnectedControllers();
	if (controllers.size() > number) {
		controller = controllers[number];
	}
}

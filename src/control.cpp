#include "control.hpp"

#include <jngl/Controller.hpp>
#include <jngl/input.hpp>

ControlBase::~ControlBase() {
}

bool ControlBase::step(const std::function<void(ControlType)>& set) {
	if (junkToAdd > 0) {
		--junkToAdd;
		set(ControlType::AddJunk);
	}
	return true;
}

void ControlBase::addJunk() {
	++junkToAdd;
}

bool ControlBase::desync() const {
	return false;
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

void Control::addJunk() {
	for (const auto& control : controls) {
		control->addJunk();
	}
}

bool Control::desync() const {
	for (const auto& control : controls) {
		if (control->desync()) {
			return true;
		}
	}
	return false;
}

bool Control::step() {
	bits_.reset();
	bool success = false;
	for (auto& control : controls) {
		if (control->step([&](ControlType e) { bits_.set(static_cast<size_t>(e)); })) {
			success = true;
		}
	}
	return success;
}

bool KeyboardControl::step(const std::function<void(ControlType)>& Set) {
	if (jngl::keyPressed(jngl::key::Space) || jngl::keyPressed(jngl::key::Return)) {
		Set(ControlType::Drop);
	}
	if (jngl::keyDown(jngl::key::Down) || jngl::keyDown('k')) {
		Set(ControlType::Down);
	}
	if (jngl::keyPressed(jngl::key::Left) || jngl::keyPressed('j')) {
		Set(ControlType::Left);
	}
	if (jngl::keyPressed(jngl::key::Right) || jngl::keyPressed('l')) {
		Set(ControlType::Right);
	}
	if (jngl::keyPressed(jngl::key::Up) || jngl::keyPressed('i')) {
		Set(ControlType::Rotate);
	}
	if (jngl::keyPressed(jngl::key::ControlR) || jngl::keyPressed(jngl::key::AltR)) {
		Set(ControlType::RotateCounter);
	}
	return ControlBase::step(Set);
}

bool GamepadControl::step(const std::function<void(ControlType)>& Set) {
	if (!controller) {
		return false;
	}
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

	// Bastian's Nintendo Steuerung (modern)
	// if (controller->pressed(jngl::controller::DpadUp)) {
	// 	Set(ControlType::Drop);
	// }
	// if (controller->down(jngl::controller::RightTrigger)) {
	// 	Set(ControlType::Down);
	// }
	// if (controller->pressed(jngl::controller::DpadLeft)) {
	// 	Set(ControlType::Left);
	// }
	// if (controller->pressed(jngl::controller::DpadRight)) {
	// 	Set(ControlType::Right);
	// }
	// if (controller->down(jngl::controller::DpadDown)) {
	// 	Set(ControlType::Down);
	// }
	// if (controller->pressed(jngl::controller::A)) {
	// 	Set(ControlType::RotateCounter);
	// }
	// if (controller->pressed(jngl::controller::B)) {
	// 	Set(ControlType::Rotate);
	// }
	return ControlBase::step(Set);
}

GamepadControl::GamepadControl(const size_t number) {
	const auto controllers = jngl::getConnectedControllers();
	if (controllers.size() > number) {
		controller = controllers[number];
	}
}

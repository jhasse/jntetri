#pragma once

#include "ControlType.hpp"

#include <bitset>
#include <functional>
#include <memory>
#include <vector>

namespace jngl {
class Controller;
} // namespace jngl

class ControlBase {
public:
	virtual ~ControlBase();

	/// Returns false when no new commands are coming in (e.g. network issues).
	virtual bool step(const std::function<void(ControlType)>&);

	/// step should set ControlType::AddJunk in the next frame
	virtual void addJunk();

	/// Network issues?
	virtual bool desync() const;

protected:
	int junkToAdd = 0;
};

class Control {
public:
	Control(std::initializer_list<std::shared_ptr<ControlBase>>);
	virtual ~Control();
	bool Check(ControlType);

	/// Returns false when no new commands are coming in (e.g. network issues).
	virtual bool step();

	void forEach(const std::function<void(ControlType)>&);

	/// Calls ControlBase::addJunk. Only really relevant when using NetworkControl
	void addJunk();

	bool desync() const;

protected:
	std::bitset<static_cast<size_t>(ControlType::LastValue)> bits_;
	std::vector<std::shared_ptr<ControlBase>> controls;
};

class KeyboardControl : public ControlBase {
public:
	bool step(const std::function<void(ControlType)>&) override;
};

class GamepadControl : public ControlBase {
public:
	explicit GamepadControl(size_t number);
	bool step(const std::function<void(ControlType)>&) override;

private:
	std::shared_ptr<jngl::Controller> controller;
};

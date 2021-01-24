#pragma once

#include <bitset>
#include <functional>
#include <memory>
#include <vector>

namespace jngl {
class Controller;
}

enum class ControlType {
	Drop,
	Left,
	Right,
	Down,
	Rotate,
	RotateCounter,
	Null,     // dummy package used in network games and replays
	AddJunk,  // only for multiplayer
	LastValue // must always be the last (see bitset in Control class)
};

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
	GamepadControl(size_t number);
	bool step(const std::function<void(ControlType)>&) override;

private:
	std::shared_ptr<jngl::Controller> controller;
};

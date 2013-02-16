#pragma once

#include <bitset>
#include <boost/function.hpp>

namespace control {
	enum ControlType {
		Drop,
		Left,
		Right,
		Down,
		Rotate,
		RotateCounter,
		Null, // dummy package used in network games
		LastValue // must always be the last (see bitset in Control class)
	};
};

class ControlBase {
public:
	virtual ~ControlBase();
	virtual void step(std::function<void(control::ControlType)>) = 0;
protected:
};

class Control {
public:
	Control(std::initializer_list<std::shared_ptr<ControlBase>>);
	virtual ~Control();
	bool Check(control::ControlType);
	void Step();
	void ForEach(const boost::function<void(control::ControlType)>&);
protected:
	std::bitset<control::LastValue> bits_;
	std::vector<std::shared_ptr<ControlBase>> controls;
};

class KeyboardControl : public ControlBase {
public:
	void step(std::function<void(control::ControlType)>) override;
};

class GamepadControl : public ControlBase {
public:
	GamepadControl(int number);
	void step(std::function<void(control::ControlType)>) override;
private:
	int number;
};
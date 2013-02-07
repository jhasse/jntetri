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

class Control {
public:
	virtual ~Control();
	bool Check(control::ControlType);
	virtual void Step() = 0;
	void ForEach(const boost::function<void(control::ControlType)>&);
protected:
	void Set(control::ControlType);
	std::bitset<control::LastValue> bits_;
};

class KeyboardControl : public Control {
public:
	void Step();
};

class GamepadControl : public Control {
public:
	void Step() override;
};
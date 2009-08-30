#pragma once

class Control {
public:
	virtual ~Control();
	virtual bool Drop() = 0;
	virtual bool Left() = 0;
	virtual bool Right() = 0;
	virtual bool Down() = 0;
	virtual bool Rotate() = 0;
	virtual bool RotateCounter() = 0;
};

class KeyboardControl : public Control {
public:
	bool Drop();
	bool Left();
	bool Right();
	bool Down();
	bool Rotate();
	bool RotateCounter();
};

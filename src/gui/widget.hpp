#pragma once

class Widget {
public:
	Widget();
	virtual ~Widget();
	virtual void Step() = 0;
	virtual void Draw() const = 0;
	void SetSensitive(bool);
protected:
	bool sensitive_;
};

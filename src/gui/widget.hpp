#pragma once

class Widget {
public:
	virtual ~Widget();
	virtual void Step() = 0;
	virtual void Draw() const = 0;
};

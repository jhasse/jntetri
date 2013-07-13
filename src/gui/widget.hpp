#pragma once

#include "../engine/work.hpp"

class Widget {
public:
	Widget();
	virtual ~Widget();
	virtual void step() = 0;
	virtual void draw() const = 0;
	bool GetSensitive() const;
	void SetSensitive(bool);
	virtual void SetFocus(bool);
	virtual void OnFocusChanged();
	virtual void OnAdd(Work&);
protected:
	bool sensitive_;
	bool focus_;
};

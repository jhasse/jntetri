#pragma once

#include "../engine/work.hpp"

class Widget {
public:
	Widget();
	virtual ~Widget();
	virtual void Step() = 0;
	virtual void Draw() const = 0;
	bool GetSensitive() const;
	void SetSensitive(bool);
	virtual void SetFocus(bool);
	virtual void OnFocusChanged();
	virtual void OnAdd(Work&);
protected:
	bool sensitive_;
	bool focus_;
};

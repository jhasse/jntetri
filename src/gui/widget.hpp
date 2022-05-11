#pragma once

#include <jngl/Drawable.hpp>

class Work;

class Widget : public jngl::Drawable {
public:
	bool getSensitive() const;
	void setSensitive(bool);
	bool getFocus() const;
	void setFocus(bool);
	virtual void onFocusChanged();
	virtual void onAdd(Work&);

protected:
	bool sensitive = true;
	bool focus = false;
};

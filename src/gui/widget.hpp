#pragma once

class Work;

class Widget {
public:
	virtual ~Widget();
	virtual void step() = 0;
	virtual void draw() const = 0;
	bool getSensitive() const;
	void setSensitive(bool);
	void setFocus(bool);
	virtual void onFocusChanged();
	virtual void onAdd(Work&);

protected:
	bool sensitive = true;
	bool focus = false;
};

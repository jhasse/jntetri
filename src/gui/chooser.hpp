#pragma once

#include "Button.hpp"
#include "widget.hpp"

#include <list>

class Chooser : public Widget {
public:
	Chooser(int x, int y);
	void step() override;
	void draw() const override;
	int GetValue() const;
	void AddValue(int);
	void Previous();
	void Next();
private:
	typedef std::list<int> ArrayType;
	ArrayType values_;
	ArrayType::const_iterator activeValue_;
	Button previous_;
	Button next_;
	std::string sprite_;
};

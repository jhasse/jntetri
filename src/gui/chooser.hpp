#pragma once

#include "button.hpp"

#include <list>

class Chooser {
public:
	Chooser(int x, int y);
	void Step();
	void Draw() const;
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
	int x_;
	int y_;
};

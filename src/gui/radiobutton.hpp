#pragma once

#include "widget.hpp"

#include <string>

class RadioButton : public Widget {
public:
	RadioButton(const std::string& label);
	RadioButton* getPrevious() const;
	void setPrevious(RadioButton*);
	RadioButton* getNext() const;
	void setNext(RadioButton*);
	void setChecked(bool);
	void step() override;
	void draw() const override;
private:
	bool checked = false;
	RadioButton* previous = nullptr;
	RadioButton* next = nullptr;
	std::string label;
};

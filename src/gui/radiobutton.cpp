#include "radiobutton.hpp"

RadioButton::RadioButton(const std::string& label) : label(label) {
}

void RadioButton::setNext(RadioButton* n) {
	next = n;
	if (next) {
		next->setPrevious(this);
	}
}

void RadioButton::setPrevious(RadioButton* p) {
	previous = p;
	if (previous) {
		previous->setNext(this);
	}
}

RadioButton* RadioButton::getNext() const {
	return next;
}

RadioButton* RadioButton::getPrevious() const {
	return previous;
}

void RadioButton::setChecked(bool c) {
	checked = c;
	if (checked) {
		for (auto it = getPrevious(); it; it = getPrevious()) {
			it->setChecked(false);
		}
		for (auto it = getNext(); it; it = getNext()) {
			it->setChecked(false);
		}
	}
}

void RadioButton::step() {
}

void RadioButton::draw() const {
}

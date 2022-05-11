#include "widget.hpp"

bool Widget::getSensitive() const {
	return sensitive;
}

void Widget::setSensitive(const bool sensitive) {
	this->sensitive = sensitive;
}

bool Widget::getFocus() const {
	return focus;
}

void Widget::setFocus(const bool focus) {
	bool old = this->focus;
	this->focus = focus;
	if (focus != old) {
		onFocusChanged();
	}
}

void Widget::onFocusChanged() {
}

void Widget::onAdd(Work&) {
}

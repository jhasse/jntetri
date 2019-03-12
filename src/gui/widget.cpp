#include "widget.hpp"

Widget::Widget() : sensitive(true), focus_(false) {
}

Widget::~Widget() {
}

bool Widget::GetSensitive() const {
	return sensitive;
}

void Widget::SetSensitive(const bool sensitive) {
	this->sensitive = sensitive;
}

void Widget::SetFocus(bool focus) {
	bool old = focus_;
	focus_ = focus;
	if (focus_ != old) {
		OnFocusChanged();
	}
}

void Widget::OnFocusChanged() {
}

void Widget::onAdd(Work&) {
}

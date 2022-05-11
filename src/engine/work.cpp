#include "work.hpp"
#include "procedure.hpp"
#include "../gui/widget.hpp"

#include <algorithm>
#include <jngl.hpp>

Work::~Work() {
}

void Work::FocusPrevious() {
	focusedWidget_->setFocus(false);
	if (focusedWidget_ == widgets_.front()) {
		focusedWidget_ = widgets_.back();
	} else {
		auto it = std::find(widgets_.begin(), widgets_.end(), focusedWidget_);
		--it;
		focusedWidget_ = *it;
	}
}

void Work::FocusNext() {
	focusedWidget_->setFocus(false);
	if (focusedWidget_ == widgets_.back()) {
		focusedWidget_ = widgets_.front();
	} else {
		auto it = std::find(widgets_.begin(), widgets_.end(), focusedWidget_);
		++it;
		focusedWidget_ = *it;
	}
}

void Work::StepFocus() {
	if (jngl::keyPressed(jngl::key::Down) || jngl::keyPressed(jngl::key::Tab)) {
		FocusNext();
	}
	if (jngl::keyPressed(jngl::key::Up)) {
		FocusPrevious();
		while (!focusedWidget_->getSensitive()) {
			FocusPrevious();
		}
	}
	focusedWidget_->setFocus(true);
}

void Work::StepWidgets() {
	if (widgets_.empty()) {
		return;
	}
	jngl::setCursor(jngl::Cursor::ARROW);
	for (auto w : widgets_) {
		bool focus = w->getFocus();
		w->step();
		if (!focus && w->getFocus()) { // the widget requests focus
			focusedWidget_->setFocus(false);
			focusedWidget_ = w;
		} else if (focus && !w->getFocus()) { // the widget wants to lose focus
			FocusNext();
		}
	}
	StepFocus();
}

void Work::DrawWidgets() const {
	for (auto& w : widgets_) {
		w->draw();
	}
}

void Work::addWidget(std::shared_ptr<Widget> widget) {
	widgets_.push_back(widget);
	if (!focusedWidget_) {
		focusedWidget_ = widget;
	}
	widget->onAdd(*this);
	StepFocus();
}

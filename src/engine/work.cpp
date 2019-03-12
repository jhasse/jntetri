#include "work.hpp"
#include "procedure.hpp"
#include "../gui/widget.hpp"

#include <algorithm>
#include <jngl/input.hpp>

Work::~Work() {
}

void Work::FocusPrevious() {
	focusedWidget_->SetFocus(false);
	if (focusedWidget_ == widgets_.front()) {
		focusedWidget_ = widgets_.back();
	} else {
		auto it = std::find(widgets_.begin(), widgets_.end(), focusedWidget_);
		--it;
		focusedWidget_ = *it;
	}
}

void Work::FocusNext() {
	focusedWidget_->SetFocus(false);
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
	if (!focusedWidget_->GetSensitive()) {
		FocusNext();
	}
	if (jngl::keyPressed(jngl::key::Up)) {
		FocusPrevious();
		while (!focusedWidget_->GetSensitive()) {
			FocusPrevious();
		}
	}
	focusedWidget_->SetFocus(true);
}

void Work::StepWidgets() {
	if (widgets_.empty()) {
		return;
	}
	for (auto w : widgets_) {
		w->step();
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

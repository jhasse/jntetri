#include "buttonbox.hpp"
#include "../engine/options.hpp"
#include "../engine/screen.hpp"
#include "../engine/procedure.hpp"

const int ButtonBox::spacing_ = 30;

ButtonBox::ButtonBox(const int xCenter, const int yCenter)
: xCenter_(xCenter), yCenter_(yCenter) {
	SetSensitive(false);
}

ButtonBox::ButtonBox()
: xCenter_(0), yCenter_(0) {
	SetSensitive(false);
}

void ButtonBox::add(const std::string& text, std::function<void()> function, const char shortcut) {
	buttons_.push_back(std::make_shared<Button>(text, function, shortcut));

	auto end = buttons_.end();
	int yPosButton = yCenter_ - (buttons_.size() * (buttons_[0]->GetHeight() + spacing_) - spacing_) / 2 + buttons_[0]->GetHeight() / 2;
	for (auto it = buttons_.begin(); it != end; ++it) {
		(*it)->CenterAt(xCenter_, yPosButton);
		yPosButton += buttons_[0]->GetHeight() + spacing_;
	}
}

void ButtonBox::draw() const {
}

void ButtonBox::step() {
}

int ButtonBox::GetMouseover() const
{
	for(std::size_t i = 0; i < buttons_.size(); ++i)
	{
		if(buttons_[i]->Mouseover())
		{
			return static_cast<int>(i);
		}
	}
	return -1;
}

void ButtonBox::OnAdd(Work& work) {
	for (auto& button : buttons_) {
		work.addWidget(button);
	}
}

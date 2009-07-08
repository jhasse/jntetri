#include "buttonbox.hpp"
#include "../engine/options.hpp"
#include "../engine/screen.hpp"

const int ButtonBox::spacing_ = 30;

ButtonBox::ButtonBox(const int xCenter, const int yCenter)
	: xCenter_(xCenter), yCenter_(yCenter)
{
}

ButtonBox::ButtonBox()
	: xCenter_(0), yCenter_(GetScreen().GetHeight() / 2)
{
}

void ButtonBox::Add(const std::string& text, boost::function<void()> function, const char shortcut)
{
	buttons_.push_back(Button(text, function, shortcut));

	std::vector<Button>::iterator end = buttons_.end();
	int yPosButton = yCenter_ - (buttons_.size() * (buttons_[0].GetHeight() + spacing_) - spacing_) / 2 + buttons_[0].GetHeight() / 2;
	for(std::vector<Button>::iterator it = buttons_.begin(); it != end; ++it)
	{
		it->CenterAt(xCenter_, yPosButton);
		yPosButton += buttons_[0].GetHeight() + spacing_;
	}
}

void ButtonBox::Draw() const
{
	std::vector<Button>::const_iterator end = buttons_.end();
	for(std::vector<Button>::const_iterator it = buttons_.begin(); it != end; ++it)
	{
		it->Draw();
	}
}

void ButtonBox::Step()
{
	std::vector<Button>::iterator end = buttons_.end();
	for(std::vector<Button>::iterator it = buttons_.begin(); it != end; ++it)
	{
		it->Step();
	}
}

int ButtonBox::GetMouseover() const
{
	for(std::size_t i = 0; i < buttons_.size(); ++i)
	{
		if(buttons_[i].Mouseover())
		{
			return i;
		}
	}
	return -1;
}

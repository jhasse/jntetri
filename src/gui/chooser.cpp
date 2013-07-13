#include "chooser.hpp"
#include "../engine/screen.hpp"
#include "../engine/paths.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <jngl/all.hpp>

Chooser::Chooser(int x, int y)
	: previous_(""),
	  next_(""), sprite_("chooser"), x_(x), y_(y)
{
	previous_.Connect(boost::bind(&Chooser::Previous, this));
	previous_.SetSprites("chooser_left", "chooser_left_over", "chooser_left_over");
	previous_.CenterAt(x - jngl::getWidth(GetPaths().Graphics() + sprite_) / 2 - previous_.GetWidth() / 2, y);
	next_.Connect(boost::bind(&Chooser::Next, this));
	next_.SetSprites("chooser_right", "chooser_right_over", "chooser_right_over");
	next_.CenterAt(x + jngl::getWidth(GetPaths().Graphics() + sprite_) / 2 + next_.GetWidth() / 2, y);
}

void Chooser::AddValue(int v)
{
	values_.push_back(v);
	activeValue_ = values_.begin();
}

void Chooser::step() {
	previous_.SetFocus(focus_);
	next_.SetFocus(focus_);
	previous_.step();
	next_.step();
	if(focus_)
	{
		if(jngl::keyPressed(jngl::key::Left) || jngl::keyPressed(jngl::key::WizLeft))
		{
			Previous();
		}
		if(jngl::keyPressed(jngl::key::Right) || jngl::keyPressed(jngl::key::WizRight))
		{
			Next();
		}
	}
}

void Chooser::draw() const {
	GetScreen().DrawCentered(sprite_, x_, y_);
	jngl::setFontSize(70);
	jngl::setFontColor(255, 255, 255);
	GetScreen().PrintCentered(boost::lexical_cast<std::string>(*activeValue_), x_, y_);
	previous_.draw();
	next_.draw();
}

int Chooser::GetValue() const
{
	return *activeValue_;
}

void Chooser::Next()
{
	++activeValue_;
	if(activeValue_ == values_.end())
	{
		activeValue_ = values_.begin();
	}
}

void Chooser::Previous()
{
	if(activeValue_ == values_.begin())
	{
		activeValue_ = values_.end();
	}
	--activeValue_;
}

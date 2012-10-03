#include "chooser.hpp"
#include "../engine/screen.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <jngl/all.hpp>

Chooser::Chooser(int x, int y)
	: previous_(""),
	  next_(""), sprite_("chooser.png"), x_(x), y_(y)
{
	previous_.Connect(boost::bind(&Chooser::Previous, this));
	previous_.SetSprites("chooser_left.png", "chooser_left_over.png", "chooser_left_over.png");
	previous_.CenterAt(x - GetScreen().GetWidth(sprite_) / 2 - previous_.GetWidth() / 2, y);
	next_.Connect(boost::bind(&Chooser::Next, this));
	next_.SetSprites("chooser_right.png", "chooser_right_over.png", "chooser_right_over.png");
	next_.CenterAt(x + GetScreen().GetWidth(sprite_) / 2 + next_.GetWidth() / 2, y);
}

void Chooser::AddValue(int v)
{
	values_.push_back(v);
	activeValue_ = values_.begin();
}

void Chooser::Step()
{
	previous_.SetFocus(focus_);
	next_.SetFocus(focus_);
	previous_.Step();
	next_.Step();
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

void Chooser::Draw() const
{
	GetScreen().DrawCentered(sprite_, x_, y_);
	GetScreen().SetFontSize(70);
	jngl::setFontColor(255, 255, 255);
	GetScreen().PrintCentered(boost::lexical_cast<std::string>(*activeValue_), x_, y_);
	previous_.Draw();
	next_.Draw();
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

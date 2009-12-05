#include "chooser.hpp"
#include "../engine/screen.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <jngl.hpp>

Chooser::Chooser(int x, int y)
	: previous_("", boost::bind(&Chooser::Previous, this)),
	  next_("", boost::bind(&Chooser::Next, this)), sprite_("chooser.png"), x_(x), y_(y)
{
	previous_.SetSprites("chooser_left.png", "chooser_left_over.png", "chooser_left_over.png");
	previous_.CenterAt(x - GetScreen().GetWidth(sprite_) / 2 - previous_.GetWidth() / 2, y);
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
		if(jngl::KeyPressed(jngl::key::Left) || jngl::KeyPressed(jngl::key::WizLeft))
		{
			Previous();
		}
		if(jngl::KeyPressed(jngl::key::Right) || jngl::KeyPressed(jngl::key::WizRight))
		{
			Next();
		}
	}
}

void Chooser::Draw() const
{
	GetScreen().DrawCentered(sprite_, x_, y_);
	GetScreen().SetFontSize(70);
	jngl::SetFontColor(255, 255, 255);
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

#include "input.hpp"
#include "../engine/screen.hpp"

#include <jngl.hpp>

Input::Input(int x, int y) : x_(x), y_(y), password_(false), displayCursor_(0), letter_('A')
{
	unicodeChars_.push_back("ä");
	unicodeChars_.push_back("ö");
	unicodeChars_.push_back("ü");
	unicodeChars_.push_back("ß");
	unicodeChars_.push_back("§");
}

Input::~Input()
{
}

void Input::SetPassword(bool password)
{
	password_ = password;
}

void Input::OnFocusChanged()
{
	displayCursor_ = 50;
}

void Input::Step()
{
	if(!sensitive_)
	{
		return;
	}
	if(focus_)
	{
		--displayCursor_;
		if(displayCursor_ < -50)
		{
			displayCursor_ = 50;
		}
		if(jngl::KeyPressed(jngl::key::Up))
		{
			--letter_;
		}
		if(jngl::KeyPressed(jngl::key::Down))
		{
			++letter_;
		}
		if(letter_ == 'A' - 1)
		{
			letter_ = 'Z';
		}
		if(letter_ == 'Z' + 1)
		{
			letter_ = 'A';
		}
		if(jngl::KeyPressed(jngl::key::Right))
		{
			text_ += letter_;
		}

		if((jngl::KeyPressed(jngl::key::Left) || jngl::KeyPressed(jngl::key::WizLeft)) && !text_.empty())
		{
			displayCursor_ = 50;
			std::string::iterator it = text_.end();
			--it;
			while(*it & 0x80 && !(*it & 0x40)) // Unicode character?
			{
				--it;
			}
			text_.erase(it, text_.end());
		}
	}
}

void Input::Draw() const
{
	std::string temp;
	if(password_)
	{
		temp = text_;
		int size = text_.size();
		text_.clear();
		for(int i = 0; i < size; ++i)
		{
			text_ += "●";
		}
	}
	if(sensitive_)
	{
		jngl::SetFontColor(0, 0, 0);
	}
	else
	{
		jngl::SetFontColor(150, 150, 150);
	}
	GetScreen().Print(text_, x_, y_);
	if(focus_)
	{
		jngl::SetFontColor(0, 0, 0, displayCursor_ + 150);
		const int x = x_ + GetScreen().GetTextWidth(text_);
		char temp[2] = { letter_, 0 };
		GetScreen().Print(temp, x, y_);
	}
	if(password_)
	{
		text_ = temp;
	}
}

std::string Input::GetText() const
{
	return text_;
}

void Input::SetText(const std::string& text)
{
	text_ = text;
}

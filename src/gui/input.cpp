#include "input.hpp"
#include "../engine/screen.hpp"

#include <jngl.hpp>

Input* Input::focus_ = 0;

Input::Input(int x, int y) : x_(x), y_(y), password_(false)
{
	unicodeChars_.push_back("ä");
	unicodeChars_.push_back("ö");
	unicodeChars_.push_back("ü");
	unicodeChars_.push_back("ß");
	unicodeChars_.push_back("§");
	Focus();
}

Input::~Input()
{
	if(focus_ == this)
	{
		focus_ = 0;
	}
}

void Input::Focus()
{
	focus_ = this;
}

void Input::SetPassword(bool password)
{
	password_ = password;
}
#include <iostream>
void Input::Step()
{
	if(!sensitive_)
	{
		return;
	}
	if(focus_ == this)
	{
		for(char c = ' '; c < '~' + 1; ++c)
		{
			if(jngl::KeyPressed(c))
			{
				text_ += c;
			}
		}
		std::vector<std::string>::iterator end = unicodeChars_.end();
		for(std::vector<std::string>::iterator it = unicodeChars_.begin(); it != end; ++it)
		{
			if(jngl::KeyPressed(*it))
			{
				text_ += *it;
			}
		}
		if(jngl::KeyPressed(jngl::key::BackSpace) && !text_.empty())
		{
			std::string::iterator it = text_.end();
			--it;
			while(*it & 0x80 && !(*it & 0x40)) // Unicode character?
			{
				--it;
			}
			text_.erase(it, text_.end());
		}
	}
	else if(jngl::KeyPressed(jngl::key::Tab))
	{
		focus_ = this;
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
	if(focus_ != this || static_cast<int>(jngl::Time() * 2) % 2)
	{
		GetScreen().Print(text_, x_, y_);
	}
	else
	{
		GetScreen().Print(text_ + "|", x_, y_);
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

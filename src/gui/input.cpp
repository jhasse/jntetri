#include "input.hpp"
#include "../engine/screen.hpp"

#include <jngl.hpp>

Input::Input(int x, int y) : x_(x), y_(y)
{
	unicodeChars_.push_back("ä");
	unicodeChars_.push_back("ö");
	unicodeChars_.push_back("ü");
	unicodeChars_.push_back("ß");
	unicodeChars_.push_back("§");
}

void Input::Step()
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

void Input::Draw() const
{
	if(static_cast<int>(jngl::Time() * 2) % 2)
	{
		GetScreen().Print(text_, x_, y_);
	}
	else
	{
		GetScreen().Print(text_ + "|", x_, y_);
	}
}

std::string Input::GetText() const
{
	return text_;
}

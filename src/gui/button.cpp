#include "button.hpp"
#include "../engine/paths.hpp"
#include "../engine/screen.hpp"

#include <jngl.hpp>
#include <string>
#include <boost/function.hpp>

const int Button::fontSize_ = 70;

Button::Button(const std::string& text, boost::function<void()> callback, char shortcut)
	: text_(text), xPos_(0), yPos_(0), mouseoverAlpha_(0),
	  shortcut_(shortcut), callback_(callback), clicked_(false)
{
	SetSprites("button.png", "button_over.png", "button_clicked.png");
}

void Button::SetText(const std::string& text)
{
	text_ = text;
}

void Button::SetSprites(const std::string& normal, const std::string& mouseOver, const std::string& clicked)
{
	texture_ = normal;
	textureMouseOver_ = mouseOver;
	textureClicked_ = clicked;
	width_ = GetScreen().GetWidth(texture_);
	height_ = GetScreen().GetHeight(texture_);
}

void Button::CenterAt(const int xCenter, const int yCenter)
{
	xPos_ = xCenter;
	yPos_ = yCenter;
	GetScreen().SetFontSize(fontSize_);
}

void Button::Draw() const
{
	GetScreen().DrawCentered(texture_, xPos_, yPos_);
	jngl::SetSpriteColor(255, 255, 255, mouseoverAlpha_);
	GetScreen().DrawCentered(textureMouseOver_, xPos_, yPos_);
	jngl::SetSpriteColor(255, 255, 255, 255);
    if(clicked_)
    {
    	GetScreen().DrawCentered(textureClicked_, xPos_, yPos_);
    }
    if(focus_)
    {
    	jngl::SetColor(0, 0, 0, 90);
    	GetScreen().DrawRect(xPos_ - width_ / 2, yPos_ - height_ / 2, width_, height_);
    }
    jngl::SetFontColor(255, 255, 255);
    GetScreen().SetFontSize(fontSize_);
    if(!clicked_)
    {
		GetScreen().PrintCentered(text_, xPos_, yPos_);
    }
    else
    {
    	GetScreen().PrintCentered(text_, xPos_ + 5, yPos_ + 5);
    }
}

void Button::Step()
{
	if(!jngl::MouseDown())
	{
		clicked_ = false;
	}
	const int alphaSpeed = 10;
	if(focus_)
	{
		if(jngl::KeyPressed(jngl::key::Space) || jngl::KeyPressed(jngl::key::Return))
		{
			clicked_ = true;
			callback_();
		}
	}
	if(Mouseover())
	{
		if(mouseoverAlpha_ < 255)
		{
			mouseoverAlpha_ += alphaSpeed;
		}
		if(jngl::MousePressed())
		{
			clicked_ = true;
			callback_();
		}
	}
	else if(mouseoverAlpha_ > 0)
	{
		mouseoverAlpha_ -= alphaSpeed;
	}
	if(mouseoverAlpha_ > 255)
	{
		mouseoverAlpha_ = 255;
	}
	if(mouseoverAlpha_ < 0)
	{
		mouseoverAlpha_ = 0;
	}
}

bool Button::Mouseover() const
{
    return (xPos_ - width_ / 2 <= GetScreen().GetMouseX() && GetScreen().GetMouseX() < (xPos_ + width_ / 2)
            && yPos_ - height_ / 2 <= GetScreen().GetMouseY() && GetScreen().GetMouseY() < (yPos_ + height_ / 2));
}

int Button::GetX() const
{
    return xPos_;
}

int Button::GetY() const
{
    return yPos_;
}

int Button::GetHeight()
{
    return height_;
}

int Button::GetWidth()
{
    return width_;
}

char Button::ShortCutClicked()
{
    return jngl::KeyDown(shortcut_);
}

#include "button.hpp"
#include "../engine/paths.hpp"
#include "../engine/screen.hpp"

#include <jngl.hpp>
#include <string>
#include <boost/function.hpp>

const int Button::fontSize_ = 70;

Button::Button(const std::string& text, boost::function<void()> callback, char shortcut)
	: text_(text), xPos_(0), yPos_(0), textPosition_(0), mouseoverAlpha_(0),
	  shortcut_(shortcut), callback_(callback), clicked_(false)
{
	SetSprites("button.png", "button_over.png", "button_clicked.png");
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
	xCenter_ = xCenter;
	yCenter_ = yCenter;
	xPos_ = xCenter - width_ / 2;
	yPos_ = yCenter - height_ / 2;
	GetScreen().SetFontSize(fontSize_);
	textPosition_ = static_cast<int>(xPos_ + width_ / 2 - GetScreen().GetTextWidth(text_) / 2);
}

void Button::Draw() const
{
	GetScreen().DrawCentered(texture_, xCenter_, yCenter_);
	jngl::SetSpriteColor(255, 255, 255, mouseoverAlpha_);
	GetScreen().DrawCentered(textureMouseOver_, xCenter_, yCenter_);
	jngl::SetSpriteColor(255, 255, 255, 255);
    if(clicked_)
    {
    	GetScreen().DrawCentered(textureClicked_, xCenter_, yCenter_);
    }
    jngl::SetFontColor(255, 255, 255);
    GetScreen().SetFontSize(fontSize_);
    if(!clicked_)
    {
		GetScreen().Print(text_, textPosition_, yPos_ + 60);
    }
    else
    {
    	GetScreen().Print(text_, textPosition_ + 10, yPos_ + 70);
    }
}

void Button::Step()
{
	if(!jngl::MouseDown())
	{
		clicked_ = false;
	}
	const int alphaSpeed = 10;
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
    return (xPos_ <= GetScreen().GetMouseX() && GetScreen().GetMouseX() < (xPos_ + width_)
            && yPos_ <= GetScreen().GetMouseY() && GetScreen().GetMouseY() < (yPos_ + height_));
}

int Button::GetX()
{
    return xPos_;
}

int Button::GetY()
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

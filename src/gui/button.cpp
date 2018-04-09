#include "button.hpp"
#include "../engine/paths.hpp"
#include "../engine/screen.hpp"

#include <jngl/all.hpp>
#include <string>
#include <boost/function.hpp>

const int Button::fontSize_ = 70;

Button::Button(const std::string& text)
	: text_(text), xPos_(0), yPos_(0), mouseoverAlpha_(0), clicked_(false)
{
	SetSprites("button", "button_over", "button_clicked");
}

Button::Button(const std::string& text, boost::function<void()> callback)
	: text_(text), xPos_(0), yPos_(0), mouseoverAlpha_(0),
	  callback_(callback), clicked_(false)
{
	SetSprites("button", "button_over", "button_clicked");
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
	width_ = jngl::getWidth(getPaths().getGraphics() + texture_);
	height_ = jngl::getHeight(getPaths().getGraphics() + texture_);
}

void Button::CenterAt(const int xCenter, const int yCenter)
{
	xPos_ = xCenter;
	yPos_ = yCenter;
	jngl::setFontSize(fontSize_);
}

void Button::draw() const {
	int alpha = mouseoverAlpha_;
	if(clicked_)
	{
		alpha -= 100;
	}
	GetScreen().DrawCenteredScaled(texture_, xPos_, yPos_, 1.0f + (alpha / 6000.0f));
    if(focus_)
    {
		GetScreen().DrawCentered(textureMouseOver_, xPos_, yPos_);
    }
	jngl::setSpriteColor(255, 255, 255, alpha);
	jngl::pushMatrix();
	GetScreen().DrawCenteredScaled(textureMouseOver_, xPos_, yPos_, 1.0f + (alpha / 6000.0f));
	jngl::popMatrix();
	jngl::setSpriteColor(255, 255, 255, 255);
    if(clicked_)
    {
    	GetScreen().DrawCenteredScaled(textureClicked_, xPos_, yPos_, 1.0f + (alpha / 6000.0f));
    }
    jngl::setFontColor(255, 255, 255);
    jngl::setFontSize(fontSize_);
    if(!clicked_)
    {
		GetScreen().PrintCentered(text_, xPos_, yPos_);
    }
    else
    {
    	GetScreen().PrintCentered(text_, xPos_ + 5, yPos_ + 5);
    }
}

void Button::step() {
	if(!jngl::mouseDown())
	{
		clicked_ = false;
	}
	const int alphaSpeed = 20;
	if(focus_)
	{
		if(jngl::keyPressed(jngl::key::Space) || jngl::keyPressed(jngl::key::Return) || jngl::keyPressed(jngl::key::WizB))
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
		if(jngl::mousePressed())
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

bool Button::Mouseover() const {
	const auto mousePos = jngl::getMousePos();
	return (xPos_ - width_ / 2 <= mousePos.x && mousePos.x < (xPos_ + width_ / 2) &&
	        yPos_ - height_ / 2 <= mousePos.y && mousePos.y < (yPos_ + height_ / 2));
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

void Button::Connect(boost::function<void()> callback)
{
	callback_ = callback;
}

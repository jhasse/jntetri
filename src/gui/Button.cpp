#include "Button.hpp"

#include "../engine/paths.hpp"
#include "../engine/screen.hpp"

#include <jngl/all.hpp>
#include <string>

const int Button::fontSize_ = 70;

Button::Button(const std::string& text) : text_(text), mouseoverAlpha_(0), clicked_(false) {
	SetSprites("button", "button_over", "button_clicked");
}

Button::Button(const std::string& text, std::function<void()> callback)
: text_(text), mouseoverAlpha_(0), callback_(callback), clicked_(false) {
	SetSprites("button", "button_over", "button_clicked");
}

void Button::SetText(const std::string& text) {
	text_ = text;
}

void Button::SetSprites(const std::string& normal, const std::string& mouseOver,
                        const std::string& clicked) {
	texture_ = normal;
	textureMouseOver_ = mouseOver;
	textureClicked_ = clicked;
	width = jngl::getWidth(getPaths().getGraphics() + texture_) * jngl::getScaleFactor();
	height = jngl::getHeight(getPaths().getGraphics() + texture_) * jngl::getScaleFactor();
}

void Button::draw() const {
	int alpha = mouseoverAlpha_;
	if (clicked_) {
		alpha -= 100;
	}
	GetScreen().DrawCenteredScaled(texture_, getCenter().x, getCenter().y,
	                               1.0f + (alpha / 6000.0f));
	if (focus) {
		GetScreen().DrawCentered(textureMouseOver_, getCenter().x, getCenter().y);
	}
	jngl::setSpriteColor(255, 255, 255, alpha);
	jngl::pushMatrix();
	GetScreen().DrawCenteredScaled(textureMouseOver_, getCenter().x, getCenter().y,
	                               1.0f + (alpha / 6000.0f));
	jngl::popMatrix();
	jngl::setSpriteColor(255, 255, 255, 255);
	if (clicked_) {
		GetScreen().DrawCenteredScaled(textureClicked_, getCenter().x, getCenter().y,
		                               1.0f + (alpha / 6000.0f));
	}
	jngl::setFontColor(255, 255, 255);
	jngl::setFontSize(fontSize_);
	if (!clicked_) {
		GetScreen().printCentered(text_, getCenter());
	} else {
		GetScreen().printCentered(text_, { getCenter().x + 5, getCenter().y + 5 });
	}
}

void Button::step() {
	if (!jngl::mouseDown()) {
		clicked_ = false;
	}
	const int alphaSpeed = 20;
	if (focus) {
		if (jngl::keyPressed(jngl::key::Space) || jngl::keyPressed(jngl::key::Return)) {
			clicked_ = true;
			callback_();
		}
	}
	if (contains(jngl::getMousePos())) {
		if (mouseoverAlpha_ < 255) {
			mouseoverAlpha_ += alphaSpeed;
		}
		if (jngl::mousePressed()) {
			clicked_ = true;
			callback_();
		}
	} else if (mouseoverAlpha_ > 0) {
		mouseoverAlpha_ -= alphaSpeed;
	}
	if (mouseoverAlpha_ > 255) {
		mouseoverAlpha_ = 255;
	}
	if (mouseoverAlpha_ < 0) {
		mouseoverAlpha_ = 0;
	}
}

void Button::Connect(std::function<void()> callback) {
	callback_ = callback;
}

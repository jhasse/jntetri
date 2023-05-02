#include "Button.hpp"

#include "../engine/paths.hpp"
#include "../engine/screen.hpp"
#include "jngl/matrix.hpp"

#include <jngl.hpp>
#include <string>

const int Button::fontSize_ = 70;

Button::Button(const std::string& text, std::function<void()> callback, const std::string& normal,
               const std::string& mouseOver, const std::string& clicked)
: text_(text), mouseoverAlpha_(0), callback_(callback), clicked_(false),
  sprite(getPaths().getGraphics() + normal), spriteMouseOver(getPaths().getGraphics() + mouseOver),
  spriteClicked(getPaths().getGraphics() + clicked) {
	width = sprite.getWidth() * jngl::getScaleFactor();
	height = sprite.getHeight() * jngl::getScaleFactor();
}

void Button::SetText(const std::string& text) {
	text_ = text;
}

void Button::draw() const {
	int alpha = mouseoverAlpha_;
	if (clicked_) {
		alpha -= 100;
	}
	auto mv = jngl::modelview().translate(getCenter());
	sprite.draw(mv.scale(1.0f + (alpha / 6000.0f)));
	if (focus) {
		spriteMouseOver.draw(mv);
	}
	jngl::setSpriteColor(255, 255, 255, alpha);
	jngl::pushMatrix();
	spriteMouseOver.draw(mv.scale(1.0f + (alpha / 6000.0f)));
	jngl::popMatrix();
	jngl::setSpriteColor(255, 255, 255, 255);
	if (clicked_) {
		spriteClicked.draw(mv.scale(1.0f + (alpha / 6000.0f)));
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
	if (sensitive && contains(jngl::getMousePos())) {
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

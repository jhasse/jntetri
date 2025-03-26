#include "Button.hpp"

#include "../engine/screen.hpp"
#include "jngl/matrix.hpp"

#include <jngl.hpp>
#include <string>
#include <utility>

const int Button::fontSize = 70;

Button::Button(std::string text, std::function<void()> callback, const std::string& normal,
               const std::string& mouseOver, const std::string& clicked)
: text_(std::move(text)), mouseoverAlpha(0), callback(std::move(callback)), clicked(false),
  sprite("gfx/" + normal), spriteMouseOver("gfx/" + mouseOver), spriteClicked("gfx/" + clicked) {
	setWidth(sprite.getWidth());
	setHeight(sprite.getHeight());
}

void Button::SetText(const std::string& text) {
	text_ = text;
}

void Button::draw() const {
	int alpha = mouseoverAlpha;
	if (clicked) {
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
	if (clicked) {
		spriteClicked.draw(mv.scale(1.0f + (alpha / 6000.0f)));
	}
	jngl::setFontColor(255, 255, 255);
	jngl::setFontSize(fontSize);
	if (!clicked) {
		GetScreen().printCentered(text_, getCenter());
	} else {
		GetScreen().printCentered(text_, { getCenter().x + 5, getCenter().y + 5 });
	}
}

void Button::step() {
	if (!jngl::mouseDown()) {
		clicked = false;
	}
	const int alphaSpeed = 20;
	if (focus) {
		if (jngl::keyPressed(jngl::key::Space) || jngl::keyPressed(jngl::key::Return)) {
			clicked = true;
			callback();
		}
	}
	if (sensitive && contains(jngl::getMousePos())) {
		if (mouseoverAlpha < 255) {
			mouseoverAlpha += alphaSpeed;
		}
		if (jngl::mousePressed()) {
			clicked = true;
			callback();
		}
	} else if (mouseoverAlpha > 0) {
		mouseoverAlpha -= alphaSpeed;
	}
	if (mouseoverAlpha > 255) {
		mouseoverAlpha = 255;
	}
	if (mouseoverAlpha < 0) {
		mouseoverAlpha = 0;
	}
}

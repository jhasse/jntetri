#pragma once

#include "widget.hpp"

#include <functional>
#include <jngl/Sprite.hpp>
#include <string>

class Button : public Widget {
public:
	Button(const std::string& text, std::function<void()> callback,
	       const std::string& normal = "button", const std::string& mouseOver = "button_over",
	       const std::string& clicked = "button_clicked");
	void SetText(const std::string&);
	void draw() const override;
	void step() override;

private:
	std::string text_;
	int mouseoverAlpha;
	std::function<void()> callback;
	bool clicked;
	jngl::Sprite sprite;
	jngl::Sprite spriteMouseOver;
	jngl::Sprite spriteClicked;
	const static int fontSize;
};

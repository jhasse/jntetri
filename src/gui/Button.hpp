#pragma once

#include "widget.hpp"

#include <functional>
#include <jngl/sprite.hpp>
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
	int mouseoverAlpha_;
	std::function<void()> callback_;
	bool clicked_;
	jngl::Sprite sprite;
	jngl::Sprite spriteMouseOver;
	jngl::Sprite spriteClicked;
	const static int fontSize_;
};

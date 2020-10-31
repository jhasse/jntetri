#pragma once

#include "widget.hpp"

#include <functional>
#include <string>

class Button : public Widget {
public:
	Button(const std::string& text);
	Button(const std::string& text, std::function<void()> callback);
	void SetSprites(const std::string& normal, const std::string& mouseOver,
	                const std::string& clicked);
	void SetText(const std::string&);
	void draw() const override;
	void step() override;
	void Connect(std::function<void()> callback);

private:
	std::string text_;
	int mouseoverAlpha_;
	std::function<void()> callback_;
	bool clicked_;
	std::string texture_;
	std::string textureMouseOver_;
	std::string textureClicked_;
	const static int fontSize_;
};

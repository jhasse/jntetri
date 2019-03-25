#pragma once

#include "widget.hpp"

#include <functional>
#include <string>

class Button : public Widget {
public:
	Button(const std::string& text);
	Button(const std::string& text, std::function<void()> callback);
    void SetSprites(const std::string& normal, const std::string& mouseOver, const std::string& clicked);
    void SetText(const std::string&);
    void draw() const override;
    void step() override;
    bool Mouseover() const;
    int GetHeight();
    int GetWidth();
    void CenterAt(int xCenter, int yCenter);
	void Connect(std::function<void()> callback);
private:
    std::string text_;
    int xPos_, yPos_, mousepos_, mouseoverAlpha_;
	std::function<void()> callback_;
    bool clicked_;
    int width_, height_;
	std::string texture_;
	std::string textureMouseOver_;
	std::string textureClicked_;
	const static int fontSize_;
};
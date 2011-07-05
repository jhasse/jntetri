#pragma once

#include "widget.hpp"

#include <boost/function.hpp>
#include <string>

class Button : public Widget
{
public:
	Button(const std::string& text);
    Button(const std::string& text, boost::function<void()> callback, char shortcut = ' ');
    void SetSprites(const std::string& normal, const std::string& mouseOver, const std::string& clicked);
    void SetText(const std::string&);
    void Draw() const;
    void Step();
    bool Mouseover() const;
    int GetX() const;
    int GetY() const;
    int GetHeight();
    int GetWidth();
    int GetMousePostiton();
    char ShortCutClicked();
    void CenterAt(int xCenter, int yCenter);
	void Connect(boost::function<void()> callback);
private:
    std::string text_;
    int xPos_, yPos_, mousepos_, mouseoverAlpha_;
    char shortcut_;
    boost::function<void()> callback_;
    bool clicked_;
    int width_, height_;
	std::string texture_;
	std::string textureMouseOver_;
	std::string textureClicked_;
	const static int fontSize_;
};

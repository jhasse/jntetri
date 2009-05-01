#pragma once

#include <boost/function.hpp>

#include <string>

class Button
{
public:
    Button(const std::string& text, boost::function<void()> callback, char shortcut = ' ');
    void SetSprites(const std::string& normal, const std::string& mouseOver, const std::string& clicked);
    void Draw() const;
    void Step();
    bool Mouseover() const;
    int GetX();
    int GetY();
    int GetHeight();
    int GetWidth();
    int GetMousePostiton();
    char ShortCutClicked();
    void SetPosition(const int xPos, const int yPos);
    void CenterAt(int xCenter, int yCenter);
private:
    std::string text_;
    int xPos_, yPos_, mousepos_, textPosition_, mouseoverAlpha_;
    char shortcut_;
    boost::function<void()> callback_;
    bool clicked_;
    int width_, height_;
	std::string texture_;
	std::string textureMouseOver_;
	std::string textureClicked_;
	const static int fontSize_;
};

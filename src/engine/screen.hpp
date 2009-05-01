#pragma once

#include "singleton.hpp"

#include <string>
#include <boost/noncopyable.hpp>

class Screen : public Singleton<Screen>
{
public:
	Screen();
	void BeginDraw();
	void Draw(const std::string& filename, double x, double y);
	void DrawCentered(const std::string& filename, double x, double y);
	void DrawCenteredScaled(const std::string& filename, double x, double y, double factor);
	void DrawLine(double xstart, double ystart, double xend, double yend);
	void DrawScaled(const std::string& filename, double x, double y, double factor);
	int GetMouseX() const;
	int GetMouseY() const;
	void SetFontSize(int size);
	void DrawRect(int x, int y, int width, int height);
	void Print(const std::string& text, double x, double y);
	void PrintCentered(const std::string& text, double x, double y);
	int GetWidth(const std::string& filename) const;
	int GetHeight(const std::string& filename) const;
	void Translate(double x, double y);
	int GetWidth() const;
	int GetHeight() const;
	double GetTextWidth(const std::string&) const;
private:
	double factor_;
	double xCenter_;
};

Screen& GetScreen();

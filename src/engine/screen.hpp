#pragma once

#include "singleton.hpp"
#include "vector2d.hpp"

#include <string>
#include <boost/noncopyable.hpp>

class Screen : public Singleton<Screen>
{
public:
	Screen();
	void BeginDraw();
	void Draw(const std::string& filename, double x, double y);
	void DrawCentered(const std::string& filename, double x, double y);
	void DrawCentered(const std::string& filename, const Vector2d& position);
	void DrawScaled(const std::string& filename, double x, double y, float xfactor, float yfactor);
	void DrawCenteredScaled(const std::string& filename, double x, double y, float factor);
	void DrawCenteredScaled(const std::string& filename, double x, double y, float xfactor, float yfactor);
	void DrawLine(double xstart, double ystart, double xend, double yend);
	void DrawLine(const Vector2d& start, const Vector2d& end);
	void DrawCircle(double x, double y, double radius);
	void DrawCircle(const Vector2d& position, double radius);
	void DrawScaled(const std::string& filename, double x, double y, float factor);
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
	Vector2d GetMousePos() const;
	Vector2d GetAbsoluteMousePos() const;
	void MoveCamera(const Vector2d& position);
	Vector2d GetCameraPosition() const;
	void CenterCameraPosition(const Vector2d&);
	Vector2d GetCameraSpeed() const;
	void StepCamera();
	void TranslateCamera();
	double GetFactor() const;
	void SetFactor(double factor);
private:
	double factor_;
	double xCenter_;
	double yCenter_;
	Vector2d cameraPosition_;
	Vector2d targetCameraPosition_;
};

Screen& GetScreen();

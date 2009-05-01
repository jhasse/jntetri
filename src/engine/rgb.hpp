#pragma once

class RGB {
public:
	RGB(unsigned char red, unsigned char green, unsigned char blue);
	unsigned char GetRed() const;
	unsigned char GetGreen() const;
	unsigned char GetBlue() const;
private:
	unsigned char red_;
	unsigned char green_;
	unsigned char blue_;
};

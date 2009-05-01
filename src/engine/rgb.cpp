#include "rgb.hpp"

#include <jngl.hpp>

RGB::RGB(unsigned char red, unsigned char green, unsigned char blue) : red_(red), green_(green), blue_(blue)
{
}

unsigned char RGB::GetRed() const
{
	return red_;
}

unsigned char RGB::GetGreen() const
{
	return green_;
}

unsigned char RGB::GetBlue() const
{
	return blue_;
}

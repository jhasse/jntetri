#include "explosion.hpp"
#include "engine/screen.hpp"

#include <jngl.hpp>

Explosion::Explosion(const Block& block, const int numberOfLines) : block_(block), countdown_(255), numberOfLines_(numberOfLines)
{
}

void Explosion::Step()
{
	countdown_ -= 5;
}

bool Explosion::Finished() const
{
	return countdown_ < 0;
}

void Explosion::Draw() const
{
	jngl::SetSpriteColor(255, 255, 255, countdown_);
	std::string filename;
	switch(numberOfLines_)
	{
		case 4:
			filename = "explosion4.png";
		break;
		default:
			filename = "explosion.png";
	}
	GetScreen().DrawCenteredScaled(filename,
	                               block_.GetX() * block_.GetSize(),
	                               -(block_.GetY() - double(255 - countdown_) / 510 * numberOfLines_) * block_.GetSize(),
	                               double(510 - countdown_) / 510);
	jngl::SetSpriteColor(255, 255, 255);
}

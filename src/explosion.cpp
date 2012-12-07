#include "explosion.hpp"
#include "engine/screen.hpp"

#include <jngl/sprite.hpp>

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
	jngl::setSpriteAlpha(countdown_);
	std::string filename;
	switch(numberOfLines_)
	{
		case 4:
			filename = "explosion4";
		break;
		default:
			filename = "explosion";
	}
	GetScreen().DrawCenteredScaled(filename,
	                               block_.getX() * block_.getSize(),
	                               -(block_.getY() - double(255 - countdown_) / 510 * numberOfLines_) * block_.getSize(),
	                               float(510 - countdown_) / 510);
	jngl::setSpriteAlpha(255);
}

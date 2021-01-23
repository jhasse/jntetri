#include "explosion.hpp"

#include "engine/paths.hpp"
#include "engine/screen.hpp"

#include <jngl.hpp>

Explosion::Explosion(const Block& block, const int numberOfLines)
: block_(block), countdown_(255), numberOfLines_(numberOfLines),
  sprite(getPaths().getGraphics() + (numberOfLines == 4 ? "explosion4" : "explosion")) {
}

void Explosion::Step() {
	countdown_ -= 5;
}

bool Explosion::isFinished() const {
	return countdown_ < 0;
}

void Explosion::Draw() const {
	jngl::setSpriteAlpha(countdown_);
	sprite.draw(
	    jngl::modelview()
	        .translate({ double(block_.getX() * block_.getSize()),
	                     -(block_.getY() - double(255 - countdown_) / 510 * numberOfLines_) *
	                         block_.getSize() })
	        .scale(float(510 - countdown_) / 510));
	jngl::setSpriteAlpha(255);
}

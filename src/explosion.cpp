#include "explosion.hpp"

#include <jngl.hpp>

Explosion::Explosion(const Block& block, const int numberOfLines)
: block_(block), countdown_(255), numberOfLines_(numberOfLines),
  sprite(std::string("gfx/") + (numberOfLines == 4 ? "explosion4" : "explosion")) {
}

void Explosion::Step() {
	countdown_ -= 5;
}

bool Explosion::isFinished() const {
	return countdown_ < 0;
}

void Explosion::draw(jngl::Mat3 modelview) const {
	sprite.draw(
	    modelview
	        .translate({ double(block_.getX() * block_.getSize()),
	                     -(block_.getY() - double(255 - countdown_) / 510 * numberOfLines_) *
	                         block_.getSize() })
	        .scale(float(510 - countdown_) / 510),
	    jngl::Alpha::u8(countdown_));
}

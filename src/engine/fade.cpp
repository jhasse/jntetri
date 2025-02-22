#include "fade.hpp"

#include <jngl.hpp>

Fade::Fade(std::shared_ptr<Work> work, int speed)
: work_(std::move(work)), oldWork_(jngl::getWork()), fadeCount_(0), speed_(speed) {
}

void Fade::step()
{
#ifdef NDEBUG
	const int maxAlpha = 255;
#endif
	fadeCount_ += speed_;
#ifdef NDEBUG
	if (fadeCount_ >= 2 * maxAlpha) // Finished?
#endif
	{
		jngl::setWork(work_);
	}
}

void Fade::draw() const
{
	const int maxAlpha = 255;
	if(fadeCount_ < maxAlpha)
	{
		if(!dynamic_cast<Fade*>(oldWork_.get()))
		{
			oldWork_->draw();
		}
	}
	else
	{
		work_->draw();
	}
	const int alpha = static_cast<int>(fadeCount_ > maxAlpha ? 2 * maxAlpha - fadeCount_ : fadeCount_);
	jngl::drawRect(jngl::modelview().translate(-jngl::getScreenSize() / 2), jngl::getScreenSize(),
	               jngl::Rgba::u8(0, 0, 0, alpha));
}

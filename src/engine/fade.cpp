#include "fade.hpp"
#include "procedure.hpp"
#include "options.hpp"
#include "screen.hpp"

#include <jngl/all.hpp>

Fade::Fade(Work* work, int speed) : work_(work), oldWork_(jngl::getWork()), fadeCount_(0), speed_(speed)
{
}

void Fade::step()
{
	const int maxAlpha = 255;
	fadeCount_ += speed_;
	if(fadeCount_ >= 2 * maxAlpha) // Finished?
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
	jngl::setColor(0, 0, 0, alpha);
	GetScreen().DrawRect(-GetScreen().GetWidth() / 2, -GetScreen().GetHeight() / 2, GetScreen().GetWidth(), GetScreen().GetHeight());
	jngl::setColor(255, 255, 255, 255);
}

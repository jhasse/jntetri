#include "fade.hpp"
#include "procedure.hpp"
#include "options.hpp"
#include "screen.hpp"

#include <jngl.hpp>

Fade::Fade(Work* work, int speed) : work_(work), oldWork_(GetProcedure().GetWork()), fadeCount_(0), speed_(speed)
{
}

void Fade::Step()
{
	const int maxAlpha = 255;
	fadeCount_ += speed_;
	if(fadeCount_ >= 2 * maxAlpha) // Finished?
	{
		GetProcedure().SetWork(work_);
	}
}

void Fade::Draw() const
{
	const int maxAlpha = 255;
	if(fadeCount_ < maxAlpha)
	{
		if(!dynamic_cast<Fade*>(oldWork_.get()))
		{
			oldWork_->Draw();
		}
	}
	else
	{
		work_->Draw();
	}
	const int alpha = static_cast<int>(fadeCount_ > maxAlpha ? 2 * maxAlpha - fadeCount_ : fadeCount_);
	jngl::SetColor(255, 255, 255, alpha);
	GetScreen().DrawRect(-GetScreen().GetWidth() / 2, 0, GetScreen().GetWidth(), GetScreen().GetHeight());
	jngl::SetColor(255, 255, 255, 255);
}

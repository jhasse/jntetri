#include "intro.hpp"
#include "engine/procedure.hpp"
#include "engine/options.hpp"
#include "menu.hpp"
#include "engine/fade.hpp"
#include "engine/screen.hpp"

#include <jngl.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

Intro::Intro() : blink_(0), finished_(false)
{
}

Intro::~Intro()
{
}

void Intro::Draw() const
{
	GetScreen().SetFontSize(250);
	jngl::SetFontColor(0, 0, 0);
	GetScreen().PrintCentered("JNTETRI", 0, 350);
	GetScreen().SetFontSize(60);
	jngl::SetFontColor(0, 0, 0, blink_ > 255 ? 510 - blink_ : blink_);

	jngl::PushMatrix();
	GetScreen().Translate(0, 750);
	if(resizeGraphics_.Finished())
	{
		finished_ = true;
		GetScreen().PrintCentered("Press any key to continue", 0, 0);
	}
	jngl::PopMatrix();

	jngl::SetFontColor(100, 100, 100, 255);
	GetScreen().SetFontSize(35);
	GetScreen().PrintCentered("v0.8.2", 0, 960);
	GetScreen().PrintCentered("http://watteimdocht.de/jntetri", 0, 1030);
	GetScreen().PrintCentered("Copyright 2009 Jan Niklas Hasse", 0, 1100);
}

void Intro::Step()
{
	blink_ += 4;
	if(blink_ > 2 * 255)
	{
		blink_ = 0;
	}
	if(jngl::MousePressed() && finished_)
	{
		GetProcedure().SetWork(new Fade(new Menu));
	}
}

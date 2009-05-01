#include "procedure.hpp"
#include "options.hpp"
#include "screen.hpp"
#include "../constants.hpp"

#include <jngl.hpp>
#include <stdexcept>
#include <iostream>

Procedure::Procedure() : oldTime_(jngl::Time()), needDraw_(true), changeWork_(false), running_(true)
{
}

void Procedure::SetWork(boost::shared_ptr<Work> work)
{
	if(!currentWork_)
	{
		currentWork_ = work;
	}
	else
	{
		changeWork_ = true;
		newWork_ = work;
	}
}

void Procedure::SetWork(Work* work)
{
    return SetWork(boost::shared_ptr<Work>(work));
}

void Procedure::MainLoop()
{
	while (running_)
	{
		if(jngl::Time() - oldTime_ > 0.5) // Is half a second missing?
		{
			oldTime_ += 0.5; // Let's slowdown
		}
		if(jngl::Time() - oldTime_ > timePerStep_)
		{
			// This stuff needs to be done 100 times per second
			oldTime_ += timePerStep_;
			currentWork_->Step();
			needDraw_ = true;
			if(jngl::KeyPressed(jngl::key::Escape) || !jngl::Running())
			{
				jngl::Continue(); // Don't let JNGL send the quit event again
				currentWork_->QuitEvent();
			}
			if(jngl::KeyPressed(jngl::key::F11))
			{
				GetOptions().SetFullscreen(!GetOptions().GetFullscreen());
				ShowWindow();
			}
		}
		else
		{
			if (needDraw_)
			{
				needDraw_ = false;
				// This needs to be done when "needDraw" is true
				jngl::BeginDraw();
				GetScreen().BeginDraw();
				currentWork_->Draw();
				fps_ = jngl::FPS();
				jngl::EndDraw();
			}
			else
			{
				// Nothing to do? Okay let's sleep.
				jngl::Sleep(1);
			}
		}
		if(changeWork_)
		{
			changeWork_ = false;
			currentWork_ = newWork_;
		}
	}
}

double Procedure::FPS()
{
	return fps_;
}

boost::shared_ptr<Work> Procedure::GetWork() const
{
	return currentWork_;
}

Procedure& GetProcedure()
{
	return Procedure::Handle();
}

void Procedure::Quit()
{
    running_ = false;
}

bool Procedure::ShowWindow()
{
	const Options& options = GetOptions();
	try
	{
		jngl::HideWindow();
		jngl::ShowWindow(programShortName,
						 options.GetWindowWidth(),
						 options.GetWindowHeight(),
						 options.GetFullscreen());
	}
	catch(std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
		return false;
	}
	return true;
}

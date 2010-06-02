#include "procedure.hpp"
#include "options.hpp"
#include "screen.hpp"
#include "../constants.hpp"

#include <jngl.hpp>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

Procedure::Procedure() : oldTime_(jngl::Time()), needDraw_(true), fps_(0), fpsTemp_(0), changeWork_(false), running_(true), showFps_(false)
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
	while(running_)
	{
		if(jngl::Time() - oldTime_ > 0.5) // Is half a second missing?
		{
			oldTime_ += 0.5; // Let's slowdown
		}
		if(jngl::Time() - oldTime_ > timePerStep_)
		{
			// This stuff needs to be done 100 times per second
			oldTime_ += timePerStep_;
			jngl::UpdateKeyStates();
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
			if(jngl::KeyPressed(jngl::key::F1) || jngl::KeyPressed(jngl::key::WizL))
			{
				showFps_ = !showFps_;
			}
		}
		else
		{
			if(needDraw_ || showFps_)
			{
				needDraw_ = false;
				// This needs to be done when "needDraw" is true
				jngl::BeginDraw();
				GetScreen().BeginDraw();
				currentWork_->Draw();

				jngl::Reset();
				fpsTemp_ += jngl::FPS();
				++fpsCounter_;
				if(fpsCounter_ == 10)
				{
					fps_ = fpsTemp_ / fpsCounter_;
					fpsText_ = std::string("FPS: ") + boost::lexical_cast<std::string>(int(fps_));
					fpsCounter_ = 0;
					fpsTemp_ = 0;
				}
				if(showFps_)
				{
					jngl::SetColor(100, 255, 100);
					jngl::SetFontColor(0, 0, 0);
					jngl::SetFontSize(10);
					jngl::DrawRect(0, 0, jngl::GetTextWidth(fpsText_) + 5, 15);
					jngl::Print(fpsText_, 2, 2);
				}
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
		jngl::ShowWindow(programDisplayName + " " + programVersion,
						 options.GetWindowWidth(),
						 options.GetWindowHeight(),
						 options.GetFullscreen());
	}
	catch(std::runtime_error& err)
	{
		jngl::ErrorMessage(err.what());
		return false;
	}
	return true;
}

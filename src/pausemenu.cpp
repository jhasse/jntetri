#include "pausemenu.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "menu.hpp"
#include "engine/fade.hpp"

#include <jngl.hpp>
#include <boost/bind.hpp>

PauseMenu::PauseMenu(boost::shared_ptr<jngl::Work> work)
	: work_(boost::dynamic_pointer_cast<Game>(work)), buttonBox_(new ButtonBox)
{
	assert(work_);
	work_->SetRotateScreen(false); // Don't rotate the screen so that the buttons work correctly
	buttonBox_->Add("Resume", boost::bind(&PauseMenu::Continue, this));
	buttonBox_->Add("Menu", boost::bind(&PauseMenu::QuitToMenu, this));
	buttonBox_->Add("Quit", jngl::quit);
	AddWidget(buttonBox_);
}

void PauseMenu::step()
{
    if(jngl::keyPressed(jngl::key::Escape))
    {
		jngl::setWork(work_);
    }
    StepWidgets();
    work_->StepToRotateScreen();
}

void PauseMenu::Continue()
{
	jngl::setWork(work_);
}

void PauseMenu::draw() const
{
	work_->draw();
	DrawWidgets();
}

void PauseMenu::QuitToMenu() const
{
	jngl::setWork(new Fade(new Menu));
}

void PauseMenu::onQuitEvent()
{
	// Do Nothing
}

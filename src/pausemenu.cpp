#include "pausemenu.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "menu.hpp"
#include "engine/fade.hpp"

#include <jngl.hpp>
#include <boost/bind.hpp>

PauseMenu::PauseMenu(boost::shared_ptr<Work> work)
	: work_(boost::dynamic_pointer_cast<Game>(work)), buttonBox_(new ButtonBox)
{
	assert(work_);
	work_->SetRotateScreen(false); // Don't rotate the screen so that the buttons work correctly
	buttonBox_->Add("Resume", boost::bind(&PauseMenu::Continue, this));
	buttonBox_->Add("Menu", boost::bind(&PauseMenu::QuitToMenu, this));
	buttonBox_->Add("Quit", boost::bind(&Procedure::Quit, &GetProcedure()));
	AddWidget(buttonBox_);
}

void PauseMenu::Step()
{
    if(jngl::KeyPressed(jngl::key::Escape))
    {
        GetProcedure().SetWork(work_);
    }
    StepWidgets();
    work_->StepToRotateScreen();
}

void PauseMenu::Continue()
{
	GetProcedure().SetWork(work_);
}

void PauseMenu::Draw() const
{
	work_->Draw();
	DrawWidgets();
}

void PauseMenu::QuitToMenu() const
{
	GetProcedure().SetWork(new Fade(new Menu));
}

void PauseMenu::QuitEvent()
{
	// Do Nothing
}

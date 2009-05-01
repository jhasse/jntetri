#include "pausemenu.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "menu.hpp"
#include "engine/fade.hpp"

#include <jngl.hpp>
#include <boost/bind.hpp>

PauseMenu::PauseMenu(boost::shared_ptr<Work> work)
	: work_(work)
{
	buttonBox_.Add("Resume", boost::bind(&PauseMenu::Continue, this));
	buttonBox_.Add("Menu", boost::bind(&PauseMenu::QuitToMenu, this));
	buttonBox_.Add("Quit", boost::bind(&Procedure::Quit, &GetProcedure()));
}

void PauseMenu::Step()
{
    if(jngl::KeyPressed(jngl::key::Escape))
    {
        GetProcedure().SetWork(work_);
    }
    buttonBox_.Step();
}

void PauseMenu::Continue()
{
	GetProcedure().SetWork(work_);
}

void PauseMenu::Draw() const
{
	work_->Draw();
	buttonBox_.Draw();
}

void PauseMenu::QuitToMenu() const
{
	GetProcedure().SetWork(new Fade(new Menu));
}

void PauseMenu::QuitEvent()
{
	// Do Nothing
}

#include "multiplayermenu.hpp"
#include "engine/screen.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "menu.hpp"

#include <jngl.hpp>
#include <boost/bind.hpp>

MultiplayerMenu::MultiplayerMenu() : back_("Back", boost::bind(&MultiplayerMenu::OnBack, this), 'b')
{
	back_.CenterAt(0, 880);
}

void MultiplayerMenu::Step()
{
	back_.Step();
}

void MultiplayerMenu::Draw() const
{
	jngl::SetFontColor(0, 0, 0);
	GetScreen().PrintCentered("Sorry not implemented yet.", 0, 600);
	back_.Draw();
}

void MultiplayerMenu::OnBack() const
{
	GetProcedure().SetWork(new Fade(new Menu));
}

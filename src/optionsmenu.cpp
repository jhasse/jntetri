#include "optionsmenu.hpp"
#include "menu.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "engine/screen.hpp"
#include "engine/options.hpp"

#include <boost/bind.hpp>
#include <jngl.hpp>

OptionsMenu::OptionsMenu() : startLevel_(0, 300), startJunks_(0, 700)
{
	for(int i = 0; i < 10; ++i)
	{
		startLevel_.AddValue(i);
	}
	for(int i = 0; i < GetOptions().GetStartLevel(); ++i)
	{
		startLevel_.Next();
	}
	for(int i = 0; i < 6; ++i)
	{
		startJunks_.AddValue(i * 2);
	}
	for(int i = 0; i < GetOptions().GetStartJunks() / 2; ++i)
	{
		startJunks_.Next();
	}
	buttons_.push_back(Button("Back", boost::bind(&OptionsMenu::OnBack, this)));
	buttons_.back().CenterAt(0, 1000);
}

void OptionsMenu::Step()
{
	std::vector<Button>::iterator end = buttons_.end();
	for(std::vector<Button>::iterator it = buttons_.begin(); it != end; ++it)
	{
		it->Step();
	}
	startLevel_.Step();
	startJunks_.Step();
}

void OptionsMenu::Draw() const
{
	std::vector<Button>::const_iterator end = buttons_.end();
	for(std::vector<Button>::const_iterator it = buttons_.begin(); it != end; ++it)
	{
		it->Draw();
	}
	startLevel_.Draw();
	startJunks_.Draw();
	jngl::SetFontColor(0, 0, 0);
	GetScreen().PrintCentered("Start Level", 0, 100);
	GetScreen().PrintCentered("Start Junks", 0, 500);
}

void OptionsMenu::OnBack() const
{
	GetOptions().SetStartLevel(startLevel_.GetValue());
	GetOptions().SetStartJunks(startJunks_.GetValue());
	GetOptions().Save();
	GetProcedure().SetWork(new Fade(new Menu));
}

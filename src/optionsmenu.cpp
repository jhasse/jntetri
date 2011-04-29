#include "optionsmenu.hpp"
#include "menu.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "engine/screen.hpp"
#include "engine/options.hpp"

#include <boost/bind.hpp>
#include <jngl.hpp>

OptionsMenu::OptionsMenu() : startLevel_(new Chooser(0, 300)), startJunks_(new Chooser(0, 700)), back_(new Button("Back", boost::bind(&OptionsMenu::OnBack, this)))
{
	for(int i = 0; i < 10; ++i)
	{
		startLevel_->AddValue(i);
	}
	for(int i = 0; i < GetOptions().Get<int>("startLevel"); ++i)
	{
		startLevel_->Next();
	}
	for(int i = 0; i < 6; ++i)
	{
		startJunks_->AddValue(i * 2);
	}
	for(int i = 0; i < GetOptions().Get<int>("startJunks") / 2; ++i)
	{
		startJunks_->Next();
	}
	back_->CenterAt(0, 1000);
	AddWidget(startLevel_);
	AddWidget(startJunks_);
	AddWidget(back_);
}

void OptionsMenu::Step()
{
	StepWidgets();
}

void OptionsMenu::Draw() const
{
	DrawWidgets();
	jngl::SetFontColor(0, 0, 0);
	GetScreen().PrintCentered("Start Level", 0, 100);
	GetScreen().PrintCentered("Start Junks", 0, 500);
}

void OptionsMenu::OnBack() const
{
	GetOptions().Set("startLevel", startLevel_->GetValue());
	GetOptions().Set("startJunks", startJunks_->GetValue());
	GetOptions().Save();
	GetProcedure().SetWork(new Fade(new Menu));
}

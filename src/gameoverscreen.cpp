#include "gameoverscreen.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "engine/fade.hpp"
#include "menu.hpp"

#include <jngl.hpp>

GameOverScreen::GameOverScreen()
	: game_(boost::shared_dynamic_cast<Game>(GetProcedure().GetWork())), blink_(0), highscore_(game_->GetType()), input_(new Input(-160, 800))
{
	assert(game_);
	data_.score = game_->GetField().GetScore();
	data_.time = game_->GetTime();
	AddWidget(input_);
}

bool GameOverScreen::IsHighscore() const
{
	return highscore_.IsHighscore(data_) && (game_->GetType() == NORMAL || game_->GetField().GetLines() >= 50);
}

void GameOverScreen::Step()
{
	game_->GetField().Step(); // Show GameOver animation
	game_->StepToRotateScreen();
	if(game_->GameOverAnimationFinished())
	{
		if(IsHighscore())
		{
			StepWidgets();
			if(jngl::KeyPressed(jngl::key::Return) || jngl::KeyPressed(jngl::key::WizB))
			{
				data_.name = input_->GetText();
				highscore_.Add(data_);
				highscore_.Save();
				Menu* menu = new Menu;
				menu->BlinkHighscore(data_);
				GetProcedure().SetWork(new Fade(menu));
			}
		}
		else
		{
			blink_ += 4;
			if(blink_ > 2 * 255)
			{
				blink_ = 0;
			}
			if(jngl::MousePressed() || jngl::KeyPressed(jngl::key::Any))
			{
				GetProcedure().SetWork(new Fade(new Menu));
			}
		}
	}
}

void GameOverScreen::QuitEvent()
{
	while(!game_->GameOverAnimationFinished())
	{
		game_->GetField().Step();
	}
	if(!IsHighscore())
	{
		GetProcedure().Quit();
	}
}

void GameOverScreen::Draw() const
{
	game_->Draw();
	GetScreen().SetFontSize(80);
	jngl::SetFontColor(0, 0, 0);
	GetScreen().PrintCentered("GAMEOVER", 0, 500);
	if(game_->GameOverAnimationFinished())
	{
		GetScreen().SetFontSize(50);
		if(highscore_.IsHighscore(data_) && (game_->GetType() == NORMAL || game_->GetField().GetLines() >= 50))
		{
			jngl::SetFontColor(0, 0, 0);
			GetScreen().PrintCentered("You entered the top!", 0, 650);
			GetScreen().PrintCentered("Enter your name:", 0, 730);
			DrawWidgets();
		}
		else
		{
			jngl::SetFontColor(0, 0, 0, blink_ > 255 ? 510 - blink_ : blink_);
			GetScreen().PrintCentered("Press any key", 0, 700);
		}
	}
}

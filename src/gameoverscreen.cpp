#include "gameoverscreen.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "engine/fade.hpp"
#include "menu.hpp"

#include <jngl.hpp>

GameOverScreen::GameOverScreen()
	: game_(boost::shared_dynamic_cast<Game>(GetProcedure().GetWork())), blink_(0), highscore_(game_->GetType()), input_(-160, 800)
{
	assert(game_);
	data_.score = game_->GetField().GetScore();
	data_.time = game_->GetTime();
}

void GameOverScreen::Step()
{
	game_->GetField().Step(); // Show GameOver animation
	if(game_->GameOverAnimationFinished())
	{
		if(highscore_.IsHighscore(data_) && (game_->GetType() == NORMAL || game_->GetField().GetLines() >= 50))
		{
			input_.Step();
			if(jngl::KeyPressed(jngl::key::Return))
			{
				data_.name = input_.GetText();
				highscore_.Add(data_);
				highscore_.Save();
				GetProcedure().SetWork(new Fade(new Menu));
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
			input_.Draw();
		}
		else
		{
			jngl::SetFontColor(0, 0, 0, blink_ > 255 ? 510 - blink_ : blink_);
			GetScreen().PrintCentered("Press any key", 0, 700);
		}
	}
}

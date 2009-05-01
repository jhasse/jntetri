#include "game.hpp"
#include "engine/screen.hpp"
#include "engine/procedure.hpp"
#include "gameoverscreen.hpp"
#include "pausemenu.hpp"

#include <jngl.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iomanip>

Game::Game(GameType type) : type_(type), nextPosition_(field_.GetNextPosition()), oldNextPosition_(nextPosition_), startTime_(jngl::Time()), pauseTime_(0)
{
}

void Game::Step()
{
	if(field_.GameOver())
	{
		pauseTime_ = jngl::Time();
		GetProcedure().SetWork(new GameOverScreen);
	}
	else
	{
		field_.SetPause(false);
		field_.Step();
		if(type_ == FIFTYLINES && field_.GetLines() >= 50)
		{
			field_.SetGameOver(true);
		}
	}
	nextPosition_ = field_.GetNextPosition();
	oldNextPosition_ = (nextPosition_ - oldNextPosition_) * 0.01 + oldNextPosition_;
	if(pauseTime_ > 0.0001 && !field_.GameOver())
	{
		startTime_ += jngl::Time() - pauseTime_;
		pauseTime_ = 0;
	}
	if(jngl::KeyPressed('p'))
	{
		QuitEvent(); // Pause
	}
}

void Game::QuitEvent()
{
	static double lastPauseTime = 0;
	if(jngl::Time() - lastPauseTime > 1) // Don't allow pausing the game more then one time per second
	{
		lastPauseTime = pauseTime_ = jngl::Time();
		field_.SetPause(true);
		GetProcedure().SetWork(new PauseMenu(GetProcedure().GetWork()));
	}
}

void Game::DrawTime(const int x, const int y) const
{
	GetScreen().Print("Time: ", x, y);
	double time = GetTime();
	int minutes = int(time / 60);
	int seconds = int(time - minutes * 60);
	std::stringstream sstream;
	sstream.fill('0');
	sstream << minutes << ":" << std::setw(2) << seconds;
	GetScreen().Print(sstream.str(), 450, y + 100);
}

void Game::Draw() const
{
	field_.Draw();
	jngl::SetFontColor(0, 0, 0);
	GetScreen().SetFontSize(60);
	jngl::PushMatrix();
	GetScreen().Translate(-600, oldNextPosition_);
	GetScreen().Print("Next:", -100, -75);
	field_.DrawNextTetromino();
	jngl::PopMatrix();
	if(type_ == FIFTYLINES)
	{
		DrawTime(450, 100);
	}
	else
	{
		GetScreen().Print("Score: ", 450, 100);
		GetScreen().Print(boost::lexical_cast<std::string>(field_.GetScore()), 450, 200);
		DrawTime(450, 820);
	}
	GetScreen().Print("Level: ", 450, 340);
	GetScreen().Print(boost::lexical_cast<std::string>(field_.GetLevel()), 450, 440);
	GetScreen().Print("Lines: ", 450, 580);
	GetScreen().Print(boost::lexical_cast<std::string>(field_.GetLines()), 450, 680);
	if(jngl::KeyPressed('f'))
	{
		GetScreen().Print(std::string("FPS: ") + boost::lexical_cast<std::string>(jngl::FPS()), 450, 1060);
	}
}

Field& Game::GetField()
{
	return field_;
}

double Game::GetTime() const
{
	if(pauseTime_ > 0)
	{
		return pauseTime_ - startTime_;
	}
	else
	{
		return jngl::Time() - startTime_;
	}
}

bool Game::GameOverAnimationFinished() const
{
	return field_.GameOverAnimationFinished();
}

GameType Game::GetType() const
{
	return type_;
}

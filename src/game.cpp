#include "game.hpp"
#include "engine/screen.hpp"
#include "engine/procedure.hpp"
#include "gameoverscreen.hpp"
#include "pausemenu.hpp"
#include "control.hpp"

#include <jngl.hpp>
#include <sstream>
#include <iomanip>
#include <boost/lexical_cast.hpp>

Game::Game(GameType type, int seed)
	: field_(seed), type_(type), nextPosition_(field_.GetNextPosition()),
	  oldNextPosition_(nextPosition_), startTime_(jngl::getTime()), pauseTime_(0),
	  rotateScreen_(false), rotateDegree_(0), replayRecorder_(field_)
{
	jngl::setMouseVisible(false);
}

Game::~Game()
{
	jngl::setMouseVisible(true);
}

void Game::SetRotateScreen(bool rotateScreen)
{
	rotateScreen_ = rotateScreen;
#ifdef WIZ
	if(rotateScreen_)
	{
		field_.SetControl(new WizControlRotated);
	}
	else
	{
		field_.SetControl(new WizControl);
	}
#endif
}

void Game::step()
{
	StepToRotateScreen();
	if(jngl::keyPressed(jngl::key::WizR))
	{
		SetRotateScreen(!rotateScreen_);
	}
	if(field_.GameOver())
	{
		pauseTime_ = jngl::getTime();
		jngl::setWork(new GameOverScreen(this));
	}
	else
	{
		field_.SetPause(false);
		field_.Step();
		replayRecorder_.Step();
		if(type_ == FIFTYLINES && field_.GetLines() >= 50)
		{
			field_.SetGameOver(true);
		}
	}
	nextPosition_ = field_.GetNextPosition();
	oldNextPosition_ = (nextPosition_ - oldNextPosition_) * 0.01 + oldNextPosition_;
	if(pauseTime_ > 0.0001 && !field_.GameOver())
	{
		startTime_ += jngl::getTime() - pauseTime_;
		pauseTime_ = 0;
	}
	if(jngl::keyPressed('p') || jngl::keyPressed(jngl::key::WizMenu))
	{
		onQuitEvent(); // Pause
	}
}

void Game::onQuitEvent()
{
	static double lastPauseTime = 0;
	if(jngl::getTime() - lastPauseTime > 1) // Don't allow pausing the game more then one time per second
	{
		lastPauseTime = pauseTime_ = jngl::getTime();
		field_.SetPause(true);
		jngl::setWork(new PauseMenu(jngl::getWork()));
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

void Game::StepToRotateScreen()
{
	if(rotateScreen_ && rotateDegree_ < 90)
	{
		rotateDegree_ += (90 - rotateDegree_) * 0.05;
	}
	else
	{
		rotateDegree_ *= 0.95;
	}
}

void Game::draw() const
{
	jngl::pushMatrix();
	jngl::rotate(rotateDegree_);
	jngl::scale(1 + rotateDegree_ / 270);
	GetScreen().Translate(0, -static_cast<double>(GetScreen().GetHeight()) / 2);

	field_.Draw();
	if(!rotateScreen_)
	{
		jngl::setFontColor(0, 0, 0);
		GetScreen().SetFontSize(60);
		jngl::pushMatrix();
		GetScreen().Translate(-600, oldNextPosition_);
		GetScreen().Print("Next:", -100, -75);
		field_.DrawNextTetromino();
		jngl::popMatrix();
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
	}
	jngl::popMatrix();
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
		return jngl::getTime() - startTime_;
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

bool Game::gameOver() const {
	return field_.GameOver();
}
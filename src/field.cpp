#include "field.hpp"
#include "engine/screen.hpp"
#include "engine/random.hpp"
#include "engine/options.hpp"

#include <jngl.hpp>
#include <map>
#include <set>

const int Field::width_ = 11;
const int Field::height_ = 19;

Field::Field()
	: blockSize_(60), counter_(0), gameOver_(false), score_(0),
	  level_(GetOptions().GetStartLevel()), lines_(0), maxY_(0),
	  pause_(false)
{
	NewTetromino();
	NewTetromino();
	score_ = 0;
	for(int i = 0; i < GetOptions().GetStartJunks(); ++i)
	{
		int leaveOut = Random::Handle()(width_);
		for(int x = 0; x < width_; ++x)
		{
			if(x != leaveOut)
			{
				RGB color(Random::Handle()(255), Random::Handle()(255), Random::Handle()(255));
				AddBlock(Block(x, i, color));
			}
		}
	}
}

void Field::ResetCounter()
{
	counter_ = 200 / (level_ + 1);
}


void Field::Step()
{
	--counter_;
	if(gameOver_ && counter_ <= 0 && !blocks_.empty())
	{
		counter_ = 5;
		std::vector<Block>::iterator randomBlock = blocks_.begin() + Random::Handle()(blocks_.size());
		explosions_.push_back(Explosion(*randomBlock, 1));
		blocks_.erase(randomBlock);
	}
	if(!gameOver_)
	{
		if(counter_ <= 0)
		{
			ResetCounter();
			if(!tetromino_->MoveDown())
			{
				if(delay_)
				{
					NewTetromino();
					delay_ = false;
				}
				else
				{
					counter_ = 30; // Wait 0.3 seconds so that it's possible to move blocks below others
					delay_ = true;
				}
			}
			else
			{
				delay_ = false;
			}
		}
		tetromino_->Step();
		if(jngl::KeyPressed(jngl::key::Space) || jngl::KeyPressed(jngl::key::Return))
		{
			while(tetromino_->MoveDown())
			{
			}
			NewTetromino();
		}
		if(jngl::KeyDown(jngl::key::Down) && counter_ > 7)
		{
			counter_ = 7;
		}
	}

	std::vector<Block>::iterator end = blocks_.end();
	for(std::vector<Block>::iterator it = blocks_.begin(); it != end; ++it)
	{
		it->Step();
	}
	CheckLines();

	if((level_ + 1) * 10 <= lines_)
	{
		++level_;
	}

	std::vector<Explosion>::iterator end2 = explosions_.end();
	for(std::vector<Explosion>::iterator it = explosions_.begin(); it != end2; ++it)
	{
		it->Step();
		if(it->Finished())
		{
			it = explosions_.erase(it);
		}
	}
}

void Field::CheckLines()
{
	std::set<int> linesToRemove; // We need to sort the lines so that they are removed from top to bottom
	std::map<int, int> blocksInLine;
	std::vector<Block>::const_iterator end = blocks_.end();
	for(std::vector<Block>::const_iterator it = blocks_.begin(); it != end; ++it)
	{
		if(++blocksInLine[it->GetY()] == width_)
		{
			linesToRemove.insert(it->GetY());
		}
	}
	score_ += linesToRemove.size() * linesToRemove.size() * (level_ + 1) * (level_ + 1);

	// Remove lines from top to bottom so that explosions will be placed correctly
	std::set<int>::reverse_iterator rend = linesToRemove.rend();
	for(std::set<int>::reverse_iterator it = linesToRemove.rbegin(); it != rend; ++it)
	{
		RemoveLine(*it, linesToRemove.size());
	}
}

void Field::NewTetromino()
{
	ResetCounter();
	score_ += level_;
	if(tetromino_)
	{
		tetromino_->AttachToField();
		CheckLines();
	}
	tetromino_ = nextTetromino_;
	nextTetromino_.reset(new Tetromino(Random::Handle()(7), *this));
	if(tetromino_ && tetromino_->Collided())
	{
		gameOver_ = true;
		tetromino_->AttachToField();
		counter_ = 10;
	}
}

bool Field::GameOver() const
{
	return gameOver_;
}

void Field::Translate(const double x, const double y) const
{
	GetScreen().Translate(-double(width_ * blockSize_) / 2.0 + (x + 0.5) * blockSize_, height_ * blockSize_ - y * blockSize_);
}

int Field::GetNextPosition() const
{
	int nextPosition = 900 - (maxY_ - 4) * blockSize_;
	if(nextPosition > 900)
	{
		return 900;
	}
	if(nextPosition < 175)
	{
		return 175;
	}
	return nextPosition;
}

void Field::DrawNextTetromino() const
{
	nextTetromino_->Draw();
}

void Field::Draw() const
{
	GetScreen().DrawCentered("field.png", 0, 600);
	if(!pause_)
	{
		jngl::PushMatrix();
		Translate(0, 0);
		std::vector<Block>::const_iterator end = blocks_.end();
		for(std::vector<Block>::const_iterator it = blocks_.begin(); it != end; ++it)
		{
			it->Draw();
		}
		std::vector<Explosion>::const_iterator end2 = explosions_.end();
		for(std::vector<Explosion>::const_iterator it = explosions_.begin(); it != end2; ++it)
		{
			it->Draw();
		}
		jngl::PopMatrix();
		if(!gameOver_)
		{
			tetromino_->Draw();
		}
	}
}

bool Field::CheckCollision(const int x, const int y) const
{
	if(x < 0 || x >= width_ || y < 0)
	{
		return true;
	}
	std::vector<Block>::const_iterator end = blocks_.end();
	for(std::vector<Block>::const_iterator it = blocks_.begin(); it != end; ++it)
	{
		if(x == it->GetX() && y == it->GetY())
		{
			return true;
		}
	}
	return false;
}

void Field::AddBlock(const Block& block)
{
	blocks_.push_back(block);
	if(blocks_.back().GetY() > maxY_)
	{
		maxY_ = blocks_.back().GetY();
	}

	// Let's look if we need to copy the current animation
	std::vector<Block>::const_iterator end = blocks_.end();
	for(std::vector<Block>::const_iterator it = blocks_.begin(); it != end; ++it)
	{
		if(it->GetAnimation() > 0.0001)
		{
			blocks_.back().SetAnimation(it->GetAnimation());
		}
	}
}

int Field::GetBlockSize() const
{
	return blockSize_;
}

void Field::RemoveLine(const int y, const int numberOfLines)
{
	std::vector<Block>::iterator end = blocks_.end();
	for(std::vector<Block>::iterator it = blocks_.begin(); it != end; ++it)
	{
		if(it->GetY() == y)
		{
			explosions_.push_back(Explosion(*it, numberOfLines));
			blocks_.erase(it);
			return RemoveLine(y, numberOfLines);
		}
	}
	for(std::vector<Block>::iterator it = blocks_.begin(); it != end; ++it)
	{
		if(it->GetY() > y)
		{
			it->SetY(it->GetY() - 1);
			it->SetAnimation(it->GetAnimation() + 1);
		}
	}
	++lines_;
	--maxY_;
}

int Field::GetScore() const
{
	return score_;
}

int Field::GetLines() const
{
	return lines_;
}

int Field::GetLevel() const
{
	return level_;
}

void Field::SetPause(bool pause)
{
	pause_ = pause;
}

bool Field::GameOverAnimationFinished() const
{
	return blocks_.empty();
}

void Field::SetGameOver(bool gameOver)
{
	gameOver_ = gameOver;
}

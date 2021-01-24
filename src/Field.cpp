#include "Field.hpp"

#include "engine/screen.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <jngl/all.hpp>

const int Field::width_ = 11;
const int Field::height_ = 19;

Field::Field(int seed, const int level)
: blockSize_(60), counter_(0), gameOver_(false), score_(0), level_(level),
  lines_(0), maxY_(0), pause_(false),
  control_(new Control{ std::make_shared<KeyboardControl>(), std::make_shared<GamepadControl>(0) }),
  randomSeed(seed), linesCleared(0) {
	random.seed(seed);
	NewTetromino();
	NewTetromino();
	score_ = 0;
	std::uniform_int_distribution<int> dist(0, width_ - 1);
	std::mt19937 colorRandom;
	std::uniform_int_distribution<int> colorDist(0, 255);
	for (unsigned int i = 0; i < getOptions().startJunks; ++i) {
		int leaveOut = dist(random);
		for (int x = 0; x < width_; ++x) {
			if (x != leaveOut) {
				jngl::Color color(colorDist(colorRandom), colorDist(colorRandom),
				                  colorDist(colorRandom));
				AddBlock(Block(x, i, color));
			}
		}
	}
}

void Field::addJunk(int nr) {
	for (int i = 0; i < nr; ++i) {
		control_->addJunk();
	}
}

double Field::getSecondsPlayed() const {
	return secondsPlayed;
}

void Field::ResetCounter() {
	counter_ = static_cast<int>(200 / (level_ * 1.5 + 1));
}

void Field::step() {
	if (pause_) {
		if (checkPause && !checkPause()) {
			SetPause(false);
		}
		return;
	}
	if (gameOver_) {
		--counter_;
		if (counter_ <= 0 && !blocks_.empty()) {
			counter_ = 5;
			std::vector<Block>::iterator randomBlock =
			    blocks_.begin() + std::uniform_int_distribution<int>(0, blocks_.size() - 1)(random);
			explosions_.push_back(Explosion(*randomBlock, 1));
			blocks_.erase(randomBlock);
		}
	} else {
		if (checkPause && checkPause()) { // is our opponent having network issues?
			SetPause(true);
		} else {
			if (control_->step()) {
				--counter_;
				stepsWithoutPackage = 0;
				secondsPlayed += 1. / double(jngl::getStepsPerSecond());
				linesCleared = 0;
				if (counter_ <= 0) {
					ResetCounter();
					if (!tetromino_->MoveDown()) {
						if (delay_) {
							NewTetromino();
							delay_ = false;
						} else {
							counter_ = 30; // Wait 0.3 seconds so that it's possible to move blocks
										// below others
							delay_ = true;
						}
					} else {
						delay_ = false;
					}
				}
				tetromino_->Step();
				if (control_->Check(ControlType::Drop)) {
					tetromino_->drop();
					NewTetromino();
				}
				if (control_->Check(ControlType::Down)) {
					if (downKeyReleased_ && counter_ > 7) {
						counter_ = 7;
					}
				} else {
					downKeyReleased_ = true;
				}
				if (control_->Check(ControlType::AddJunk)) {
					for (auto& block : blocks_) {
						block.setY(block.getY() + 1);
						block.setAnimationY(block.getAnimationY() - 1);
					}
					if (tetromino_) {
						tetromino_->moveUp(1);
					}
					std::uniform_int_distribution<int> dist(0, width_ - 1);
					std::mt19937 colorRandom;
					std::uniform_int_distribution<int> colorDist(0, 255);
					int leaveOut = dist(random);
					for (int x = 0; x < width_; ++x) {
						if (x != leaveOut) {
							jngl::Color color(colorDist(colorRandom), colorDist(colorRandom),
							                  colorDist(colorRandom));
							Block block(x, 0, color);
							block.setAnimationY(-1);
							AddBlock(block);
						}
					}
				}
			} else {
				++stepsWithoutPackage;
			}
		}
	}

	for (auto& b : blocks_) {
		b.step();
	}

	if ((level_ + 1) * 10 <= lines_) {
		++level_;
	}

	for (auto& e : explosions_) {
		e.Step();
	}
	explosions_.erase(std::remove_if(explosions_.begin(), explosions_.end(),
	                                 [](const Explosion& e) { return e.isFinished(); }),
	                  explosions_.end());
}

void Field::CheckLines() {
	std::set<int>
	    linesToRemove; // We need to sort the lines so that they are removed from top to bottom
	std::map<int, int> blocksInLine;
	for (const auto& block : blocks_) {
		if (++blocksInLine[block.getY()] == width_) {
			linesToRemove.insert(block.getY());
		}
	}
	linesCleared = linesToRemove.size();
	score_ += linesCleared * linesCleared * (level_ + 1) * (level_ + 1);

	// Remove lines from top to bottom so that explosions will be placed correctly
	auto rend = linesToRemove.rend();
	for (auto it = linesToRemove.rbegin(); it != rend; ++it) {
		removeLine(*it, static_cast<int>(linesToRemove.size()));
	}
}

void Field::NewTetromino() {
	downKeyReleased_ = false;

	ResetCounter();
	score_ += level_;
	if (tetromino_) {
		tetromino_->AttachToField();
		CheckLines();
	}
	tetromino_ = nextTetromino_;
	nextTetromino_.reset(new Tetromino(std::uniform_int_distribution<int>(0, 6)(random), *this));

	if (tetromino_) {
		const int xPositions[] = { 5, 6, 4, 7, 3, 8, 2, 9, 1, 10, 0 };
		for (int i = 0; i < 11; ++i) {
			tetromino_->SetX(xPositions[i]);
			if (!tetromino_->Collided()) {
				return;
			}
		}
		tetromino_->SetX(xPositions[0]);
		gameOver_ = true;
		nextTetromino_ = tetromino_;
		counter_ = 10;
	}
}

bool Field::GameOver() const {
	return gameOver_;
}

jngl::Vec2 Field::indexToPixel(const double x, const double y) const {
	return { -double(width_ * blockSize_) / 2.0 + (x + 0.5) * blockSize_,
	         height_ * blockSize_ - y * blockSize_ };
}

double Field::getBottomY() const {
	return height_ * blockSize_;
}

int Field::GetNextPosition() const {
	int nextPosition = 900 - (maxY_ - 4) * blockSize_;
	if (nextPosition > 900) {
		return 900;
	}
	if (nextPosition < 175) {
		return 175;
	}
	return nextPosition;
}

void Field::drawNextTetromino() const {
	jngl::print("Next:", -100, -75);
	nextTetromino_->Draw();
}

void Field::draw() const {
	GetScreen().DrawCentered("field", 0, 600);
	if (stepsWithoutPackage > 0) {
		jngl::setFontColor(0x000000_rgb);
		jngl::print(std::string((stepsWithoutPackage / 20) % 4, '.'), { -60, 500 });
		return;
	}
	if (!pause_) {
		if (!gameOver_) {
			tetromino_->drawShadow();
		}
		jngl::pushMatrix();
		jngl::translate(indexToPixel(0, 0));

		std::vector<Block>::const_iterator end = blocks_.end();
		for (std::vector<Block>::const_iterator it = blocks_.begin(); it != end; ++it) {
			it->setSpriteColor();
			it->draw();
		}
		jngl::setSpriteColor(255, 255, 255);

		std::vector<Explosion>::const_iterator end2 = explosions_.end();
		for (std::vector<Explosion>::const_iterator it = explosions_.begin(); it != end2; ++it) {
			it->Draw();
		}
		jngl::popMatrix();
		if (!gameOver_) {
			tetromino_->Draw();
		}
	}
}

Block* Field::getBlock(int x, int y) {
	auto end = blocks_.end();
	for (auto it = blocks_.begin(); it != end; ++it) {
		if (x == it->getX() && y == it->getY()) {
			return &(*it);
		}
	}
	return nullptr;
}

bool Field::checkCollision(const int x, const int y) const {
	if (x < 0 || x >= width_ || y < 0) {
		return true;
	}
	auto end = blocks_.end();
	for (auto it = blocks_.begin(); it != end; ++it) {
		if (x == it->getX() && y == it->getY()) {
			return true;
		}
	}
	return false;
}

void Field::AddBlock(const Block& block) {
	blocks_.push_back(block);
	if (blocks_.back().getY() > maxY_) {
		maxY_ = blocks_.back().getY();
	}

	// Copy the current animation
	for (const auto& block : blocks_) {
		if (block.getAnimationY() > 0.0001 || block.getAnimationY() < -0.0001) {
			blocks_.back().setAnimationY(block.getAnimationY());
		}
	}
}

int Field::getBlockSize() const {
	return blockSize_;
}

void Field::removeLine(const int y, const int numberOfLines) {
	auto end = blocks_.end();
	for (auto it = blocks_.begin(); it != end; ++it) {
		if (it->getY() == y) {
			explosions_.push_back(Explosion(*it, numberOfLines));
			blocks_.erase(it);
			return removeLine(y, numberOfLines);
		}
	}
	for (auto it = blocks_.begin(); it != end; ++it) {
		if (it->getY() > y) {
			it->setY(it->getY() - 1);
			it->setAnimationY(it->getAnimationY() + 1);
		}
	}
	++lines_;
	--maxY_;
}

int Field::GetScore() const {
	return score_;
}

int Field::GetLines() const {
	return lines_;
}

int Field::GetLevel() const {
	return level_;
}

void Field::SetPause(bool pause) {
	pause_ = pause;
	jngl::setMouseVisible(pause);
}

bool Field::getPause() const {
	return pause_ || control_->desync();
}

void Field::setCheckPause(std::function<bool()> checkPause) {
	this->checkPause = std::move(checkPause);
}

bool Field::GameOverAnimationFinished() const {
	return blocks_.empty();
}

void Field::setGameOver(bool gameOver) {
	gameOver_ = gameOver;
}

void Field::setControl(Control* control) {
	control_.reset(control);
}

Control& Field::getControl() const {
	return *control_;
}

std::mt19937& Field::getRandom() {
	return random;
}

int Field::getRandomSeed() const {
	return randomSeed;
}

int Field::getLinesCleared() const {
	return linesCleared;
}

#include "Field.hpp"

#include <algorithm>
#include <jngl.hpp>
#include <map>
#include <set>

const int Field::width_ = 11;
const int Field::height_ = 19;

Field::Field(int seed, const int level, const uint32_t startJunks)
: blockSize_(60), counter_(0), gameOver_(false), score_(0), level_(level), lines(0), maxY(0),
  pause_(false),
  control_(new Control{ std::make_shared<KeyboardControl>(), std::make_shared<GamepadControl>(0) }),
  randomSeed(seed), linesCleared(0) {
	random.seed(seed);
	NewTetromino();
	NewTetromino();
	score_ = 0;
	std::mt19937 colorRandom;
	std::uniform_int_distribution<int> colorDist(0, 255);
	for (unsigned int i = 0; i < startJunks; ++i) {
		int leaveOut = random() % (width_ - 1);
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
	if (desyncInfo) {
		desyncInfo->step();
		if (!checkDesync()) {
			desyncInfo = std::nullopt;
		}
		return;
	}
	if (pause_) {
		return;
	}
	if (checkDesync && checkDesync()) { // is our opponent having network issues?
		desyncInfo = DesyncInfo(onUserQuitCallback);
	} else {
		if (!control_->step()) {
			++stepsWithoutPackage;
			return;
		}
	}
	if (gameOver_) {
		--counter_;
		if (counter_ <= 0 && !blocks_.empty()) {
			counter_ = 5;
			std::vector<Block>::iterator randomBlock =
			    blocks_.size() == 1 ? blocks_.begin()
			                        : (blocks_.begin() + (random() % (blocks_.size() - 1)));
			explosions_.push_back(Explosion(*randomBlock, 1));
			blocks_.erase(randomBlock);
		}
	} else {
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
			std::mt19937 colorRandom;
			std::uniform_int_distribution<int> colorDist(0, 255);
			int leaveOut = random() % (width_ - 1);
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
	}

	for (auto& b : blocks_) {
		b.step();
	}

	if ((level_ + 1) * 10 <= lines) {
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
	nextTetromino_.reset(new Tetromino(random() % 7, *this));

	if (tetromino_) {
		const int xPositions[] = { 5, 6, 4, 7, 3, 8, 2, 9, 1, 10, 0 };
		for (int xPosition : xPositions) {
			tetromino_->SetX(xPosition);
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
		     height_ * blockSize_ - y * blockSize_ - 600 };
}

double Field::getBottomY() const {
	return height_ * blockSize_;
}

int Field::GetNextPosition() const {
	int nextPosition = 300 - (maxY - 4) * blockSize_;
	if (nextPosition > 300) {
		return 300;
	}
	if (nextPosition < -425) {
		return -425;
	}
	return nextPosition;
}

void Field::drawNextTetromino() const {
	jngl::print("Next:", -100, -75);
	jngl::pushMatrix();
	jngl::translate(0, 600);
	nextTetromino_->draw(false);
	jngl::popMatrix();
}

void Field::draw() const {
	bg.draw();
	if (stepsWithoutPackage > 480) {
		jngl::setFontColor(0x000000_rgb);
		jngl::print(std::string((stepsWithoutPackage / 20) % 4, '.'), { -60, -100 });
		return;
	}
	if (desyncInfo && desyncInfo->getStepsAlive() > 0.5 * jngl::getStepsPerSecond()) {
		desyncInfo->draw();
	} else if (!pause_) {
		if (!gameOver_) {
			tetromino_->drawShadow();
		}
		auto mv = jngl::modelview().translate(indexToPixel(0, 0));

		auto end = blocks_.end();
		for (auto it = blocks_.begin(); it != end; ++it) {
			it->setSpriteColor();
			it->draw(mv);
		}
		jngl::setSpriteColor(255, 255, 255);

		auto end2 = explosions_.end();
		for (auto it = explosions_.begin(); it != end2; ++it) {
			it->draw(mv);
		}
		if (!gameOver_) {
			tetromino_->draw(true);
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
	if (blocks_.back().getY() > maxY) {
		maxY = blocks_.back().getY();
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
	++lines;
	--maxY;
}

int Field::GetScore() const {
	return score_;
}

int Field::GetLines() const {
	return lines;
}

int Field::GetLevel() const {
	return level_;
}

void Field::SetPause(bool pause) {
	pause_ = pause;
	jngl::setMouseVisible(pause);
}

bool Field::desync() const {
	return control_->desync();
}

void Field::setCheckDesync(std::function<bool()> checkDesync) {
	this->checkDesync = std::move(checkDesync);
}

void Field::onUserQuit(std::function<void()> onUserQuitCallback) {
	this->onUserQuitCallback = std::move(onUserQuitCallback);
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

unsigned int Field::getRandom() {
	return random();
}

int Field::getRandomSeed() const {
	return randomSeed;
}

int Field::getLinesCleared() const {
	return linesCleared;
}

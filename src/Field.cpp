#include "Field.hpp"

#include <algorithm>
#include <jngl.hpp>
#include <map>
#include <set>

const int Field::width = 11;
const int Field::height = 19;

Field::Field(int seed, const int level, const uint32_t startJunks)
: blockSize(60), counter(0), gameOver(false), score(0), level(level), lines(0), maxY(0),
  pause(false),
  control(new Control{ std::make_shared<KeyboardControl>(), std::make_shared<GamepadControl>(0) }),
  randomSeed(seed), linesCleared(0) {
	random.seed(seed);
	NewTetromino();
	NewTetromino();
	score = 0;
	std::mt19937 colorRandom;
	std::uniform_int_distribution<int> colorDist(0, 255);
	for (unsigned int i = 0; i < startJunks; ++i) {
		int leaveOut = random() % (width - 1);
		for (int x = 0; x < width; ++x) {
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
		control->addJunk();
	}
}

double Field::getSecondsPlayed() const {
	return secondsPlayed;
}

void Field::ResetCounter() {
	counter = static_cast<int>(200 / (level * 1.5 + 1));
}

void Field::step() {
	if (desyncInfo) {
		desyncInfo->step();
		if (!checkDesync()) {
			desyncInfo = std::nullopt;
		}
		return;
	}
	if (pause) {
		return;
	}
	if (checkDesync && checkDesync()) { // is our opponent having network issues?
		desyncInfo = DesyncInfo(onUserQuitCallback);
	} else {
		if (!control->step()) {
			++stepsWithoutPackage;
			return;
		}
	}
	if (gameOver) {
		--counter;
		if (counter <= 0 && !blocks.empty()) {
			counter = 5;
			std::vector<Block>::iterator randomBlock =
			    blocks.size() == 1 ? blocks.begin()
			                       : (blocks.begin() + (random() % (blocks.size() - 1)));
			explosions.push_back(Explosion(*randomBlock, 1));
			blocks.erase(randomBlock);
		}
	} else {
		--counter;
		stepsWithoutPackage = 0;
		secondsPlayed += 1. / double(jngl::getStepsPerSecond());
		linesCleared = 0;
		if (counter <= 0) {
			ResetCounter();
			if (!tetromino->MoveDown()) {
				if (delayBeforeNewTetrominoActive) {
					NewTetromino();
					delayBeforeNewTetrominoActive = false;
				} else {
					counter = 30; // Wait 0.3 seconds so that it's possible to move blocks
					              // below others
					delayBeforeNewTetrominoActive = true;
				}
			} else {
				delayBeforeNewTetrominoActive = false;
			}
		}
		tetromino->Step();
		if (control->Check(ControlType::Drop)) {
			tetromino->drop();
			NewTetromino();
		}
		if (control->Check(ControlType::Down)) {
			if (downKeyReleased && counter > 7) {
				counter = 7;
			}
		} else {
			downKeyReleased = true;
		}
		if (control->Check(ControlType::AddJunk)) {
			for (auto& block : blocks) {
				block.setY(block.getY() + 1);
				block.setAnimationY(block.getAnimationY() - 1);
			}
			if (tetromino) {
				tetromino->moveUp(1);
			}
			std::mt19937 colorRandom;
			std::uniform_int_distribution<int> colorDist(0, 255);
			int leaveOut = random() % (width - 1);
			for (int x = 0; x < width; ++x) {
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

	for (auto& b : blocks) {
		b.step();
	}

	if ((level + 1) * 10 <= lines) {
		++level;
	}

	for (auto& e : explosions) {
		e.Step();
	}
	explosions.erase(std::remove_if(explosions.begin(), explosions.end(),
	                                [](const Explosion& e) { return e.isFinished(); }),
	                 explosions.end());
}

void Field::CheckLines() {
	std::set<int>
	    linesToRemove; // We need to sort the lines so that they are removed from top to bottom
	std::map<int, int> blocksInLine;
	for (const auto& block : blocks) {
		if (++blocksInLine[block.getY()] == width) {
			linesToRemove.insert(block.getY());
		}
	}
	linesCleared = linesToRemove.size();
	score += linesCleared * linesCleared * (level + 1) * (level + 1);

	// Remove lines from top to bottom so that explosions will be placed correctly
	auto rend = linesToRemove.rend();
	for (auto it = linesToRemove.rbegin(); it != rend; ++it) {
		removeLine(*it, static_cast<int>(linesToRemove.size()));
	}
}

void Field::NewTetromino() {
	downKeyReleased = false;

	ResetCounter();
	score += level;
	if (tetromino) {
		tetromino->AttachToField();
		CheckLines();
	}
	tetromino = nextTetromino;
	nextTetromino.reset(new Tetromino(random() % 7, *this));

	if (tetromino) {
		const int xPositions[] = { 5, 6, 4, 7, 3, 8, 2, 9, 1, 10, 0 };
		for (int xPosition : xPositions) {
			tetromino->SetX(xPosition);
			if (!tetromino->Collided()) {
				return;
			}
		}
		tetromino->SetX(xPositions[0]);
		gameOver = true;
		nextTetromino = tetromino;
		counter = 10;
	}
}

bool Field::GameOver() const {
	return gameOver;
}

jngl::Vec2 Field::indexToPixel(const double x, const double y) const {
	return { -double(width * blockSize) / 2.0 + (x + 0.5) * blockSize,
		     height * blockSize - y * blockSize - 600 };
}

double Field::getBottomY() const {
	return height * blockSize;
}

int Field::GetNextPosition() const {
	int nextPosition = 300 - (maxY - 4) * blockSize;
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
	nextTetromino->draw(false);
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
	} else if (!pause) {
		if (!gameOver) {
			tetromino->drawShadow();
		}
		auto mv = jngl::modelview().translate(indexToPixel(0, 0));

		auto end = blocks.end();
		for (auto it = blocks.begin(); it != end; ++it) {
			it->setSpriteColor();
			it->draw(mv);
		}
		jngl::setSpriteColor(255, 255, 255);

		auto end2 = explosions.end();
		for (auto it = explosions.begin(); it != end2; ++it) {
			it->draw(mv);
		}
		if (!gameOver) {
			tetromino->draw(true);
		}
	}
}

Block* Field::getBlock(int x, int y) {
	auto end = blocks.end();
	for (auto it = blocks.begin(); it != end; ++it) {
		if (x == it->getX() && y == it->getY()) {
			return &(*it);
		}
	}
	return nullptr;
}

bool Field::checkCollision(const int x, const int y) const {
	if (x < 0 || x >= width || y < 0) {
		return true;
	}
	auto end = blocks.end();
	for (auto it = blocks.begin(); it != end; ++it) {
		if (x == it->getX() && y == it->getY()) {
			return true;
		}
	}
	return false;
}

void Field::AddBlock(const Block& block) {
	blocks.push_back(block);
	if (blocks.back().getY() > maxY) {
		maxY = blocks.back().getY();
	}

	// Copy the current animation
	for (const auto& block : blocks) {
		if (block.getAnimationY() > 0.0001 || block.getAnimationY() < -0.0001) {
			blocks.back().setAnimationY(block.getAnimationY());
		}
	}
}

int Field::getBlockSize() const {
	return blockSize;
}

void Field::removeLine(const int y, const int numberOfLines) {
	auto end = blocks.end();
	for (auto it = blocks.begin(); it != end; ++it) {
		if (it->getY() == y) {
			explosions.push_back(Explosion(*it, numberOfLines));
			blocks.erase(it);
			return removeLine(y, numberOfLines);
		}
	}
	for (auto it = blocks.begin(); it != end; ++it) {
		if (it->getY() > y) {
			it->setY(it->getY() - 1);
			it->setAnimationY(it->getAnimationY() + 1);
		}
	}
	++lines;
	--maxY;
}

int Field::GetScore() const {
	return score;
}

int Field::GetLines() const {
	return lines;
}

int Field::GetLevel() const {
	return level;
}

void Field::SetPause(bool pause) {
	this->pause = pause;
	jngl::setMouseVisible(pause);
}

bool Field::desync() const {
	return control->desync();
}

void Field::setCheckDesync(std::function<bool()> checkDesync) {
	this->checkDesync = std::move(checkDesync);
}

void Field::onUserQuit(std::function<void()> onUserQuitCallback) {
	this->onUserQuitCallback = std::move(onUserQuitCallback);
}

bool Field::GameOverAnimationFinished() const {
	return blocks.empty();
}

void Field::setGameOver(bool gameOver) {
	this->gameOver = gameOver;
}

void Field::setControl(Control* control) {
	this->control.reset(control);
}

Control& Field::getControl() const {
	return *control;
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

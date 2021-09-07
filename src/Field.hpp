#pragma once

#include "block.hpp"
#include "engine/options.hpp"
#include "tetromino.hpp"
#include "explosion.hpp"
#include "control.hpp"
#include "shadow.hpp"

#include <vector>
#include <random>
#include <ctime>
#include <boost/noncopyable.hpp>
#include <memory>

class Field : boost::noncopyable {
public:
	Field(int seed = static_cast<int>(std::time(0)), int level = getOptions().startLevel);
	void step();
	void draw() const;
	void DrawBlock(int x, int y) const;
	void AddBlock(const Block&);
	[[nodiscard]] jngl::Vec2 indexToPixel(double x, double y) const;

	/// y coordinate in dp of the bottom
	double getBottomY() const;

	int getBlockSize() const;
	Block* getBlock(int x, int y);
	bool checkCollision(int x, int y) const;
	void NewTetromino();
	void removeLine(int y, int numberOfLines);
	void setGameOver(bool);
	bool GameOver() const;
	int GetScore() const;
	int GetLines() const;
	int GetLevel() const;
	int GetNextPosition() const;
	void drawNextTetromino() const;
	void SetPause(bool pause);

	/// returns true when there are network issues
	bool desync() const;

	/// the function will be called to check if there's a network desync and the game should be
	/// paused
	void setCheckDesync(std::function<bool()>);

	bool GameOverAnimationFinished() const;
	void setControl(Control*);
	Control& getControl() const;
	unsigned int getRandom();
	int getRandomSeed() const;
	int getLinesCleared() const;
	void addJunk(int nr);
	double getSecondsPlayed() const;

private:
	void CheckLines();
	void ResetCounter();
	std::vector<Block> blocks_;
	std::vector<Explosion> explosions_;
	std::shared_ptr<Tetromino> nextTetromino_;
	std::shared_ptr<Tetromino> tetromino_;
	const static int width_;
	const static int height_;
	const int blockSize_;
	int counter_;
	bool gameOver_;
	int score_;
	int level_;
	int lines_;
	int maxY_;
	bool pause_;

	/// >0 when there are network issues i.e. control_->step() hasn't returned any data
	int stepsWithoutPackage = 0;

	bool delay_;
	std::shared_ptr<Control> control_;
	bool downKeyReleased_;
	std::mt19937 random;
	int randomSeed;
	int linesCleared;
	double secondsPlayed = 0;

	/// If set, we force-pause the game when it returns true
	std::function<bool()> checkDesync;
};

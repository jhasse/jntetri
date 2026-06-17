#pragma once

#include "Block.hpp"
#include "DesyncInfo.hpp"
#include "control.hpp"
#include "explosion.hpp"
#include "tetromino.hpp"

#include <memory>
#include <optional>
#include <random>
#include <vector>

class Field {
public:
	Field(int seed, int level, uint32_t startJunks);
	Field(const Field&) = delete;
	Field(Field&&) = delete;
	Field& operator=(const Field&) = delete;
	Field& operator=(Field&&) = delete;
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

	void onUserQuit(std::function<void()>);

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
	std::vector<Block> blocks;
	std::vector<Explosion> explosions;
	std::shared_ptr<Tetromino> nextTetromino;
	std::shared_ptr<Tetromino> tetromino;
	const static int width;
	const static int height;
	const int blockSize;
	int counter;
	bool gameOver;
	int score;
	int level;
	int lines;
	int maxY;
	bool pause;

	/// >0 when there are network issues i.e. control->step() hasn't returned any data
	int stepsWithoutPackage = 0;

	std::shared_ptr<Control> control;
	bool delayBeforeNewTetrominoActive = false;
	bool downKeyReleased = true;
	std::mt19937 random;
	int randomSeed;
	int linesCleared;
	double secondsPlayed = 0;

	/// If set, we force-pause the game when it returns true
	std::function<bool()> checkDesync;

	std::optional<DesyncInfo> desyncInfo;
	std::function<void()> onUserQuitCallback;

	jngl::Sprite bg{ "gfx/field" };
};

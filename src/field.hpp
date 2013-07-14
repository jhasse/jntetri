#pragma once

#include "block.hpp"
#include "tetromino.hpp"
#include "explosion.hpp"
#include "control.hpp"
#include "engine/random.hpp"
#include "shadow.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

class Field : boost::noncopyable {
public:
	Field(int seed = static_cast<int>(time(0)));
	void step();
	void draw() const;
	void DrawBlock(int x, int y) const;
	void AddBlock(const Block&);
	void Translate(double x, double y) const;
	int getBlockSize() const;
	Block* getBlock(int x, int y);
	bool CheckCollision(int x, int y) const;
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
	bool GameOverAnimationFinished() const;
	void setControl(Control*);
	Control& getControl() const;
	Random& GetRandom();
	void addShadow(int x, int y);
	void clearShadows();
	int getLinesCleared() const;
	void addJunk(int nr);
private:
	void CheckLines();
	void ResetCounter();
	std::vector<Block> blocks_;
	std::vector<Explosion> explosions_;
	std::vector<Shadow> shadows;
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
	bool delay_;
	std::shared_ptr<Control> control_;
	bool downKeyReleased_;
	Random random_;
	int linesCleared;
};

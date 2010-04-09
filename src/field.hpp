#pragma once

#include "block.hpp"
#include "tetromino.hpp"
#include "explosion.hpp"
#include "control.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>

class Field {
public:
	Field();
	void Step();
	void Draw() const;
	void DrawBlock(int x, int y) const;
	void AddBlock(const Block&);
	void Translate(double x, double y) const;
	int GetBlockSize() const;
	bool CheckCollision(int x, int y) const;
	void NewTetromino();
	void RemoveLine(int y, int numberOfLines);
	void SetGameOver(bool);
	bool GameOver() const;
	int GetScore() const;
	int GetLines() const;
	int GetLevel() const;
	int GetNextPosition() const;
	void DrawNextTetromino() const;
	void SetPause(bool pause);
	bool GameOverAnimationFinished() const;
	void SetControl(Control*);
	Control& GetControl() const;
private:
	void CheckLines();
	void ResetCounter();
	std::vector<Block> blocks_;
	std::vector<Explosion> explosions_;
	boost::shared_ptr<Tetromino> nextTetromino_;
	boost::shared_ptr<Tetromino> tetromino_;
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
	boost::shared_ptr<Control> control_;
	bool downKeyReleased_;
};

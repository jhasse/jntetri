#pragma once

#include "engine/work.hpp"
#include "field.hpp"
#include "block.hpp"
#include "gametype.hpp"
#include "replayrecorder.hpp"

class Game : public Work {
public:
	Game(GameType, int seed);
	virtual ~Game();
	virtual void step();
	virtual void draw() const;
	void onQuitEvent();
	Field& GetField();
	double GetTime() const;
	bool GameOverAnimationFinished() const;
	GameType GetType() const;
	void DrawTime(int x, int y) const;
	void StepToRotateScreen();
	void SetRotateScreen(bool);
	bool gameOver() const;
protected:
	Field field_;
	GameType type_;
	double nextPosition_;
	double oldNextPosition_;
	double startTime_;
	double pauseTime_;
	bool rotateScreen_;
	double rotateDegree_;
	ReplayRecorder replayRecorder_;
};

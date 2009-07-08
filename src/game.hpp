#pragma once

#include "engine/work.hpp"
#include "field.hpp"
#include "block.hpp"
#include "gametype.hpp"

class Game : public Work {
public:
	Game(GameType);
	virtual ~Game();
	void Step();
	void Draw() const;
	void QuitEvent();
	Field& GetField();
	double GetTime() const;
	bool GameOverAnimationFinished() const;
	GameType GetType() const;
	void DrawTime(int x, int y) const;
	void StepToRotateScreen();
	void SetRotateScreen(bool);
private:
	Field field_;
	GameType type_;
	double nextPosition_;
	double oldNextPosition_;
	double startTime_;
	double pauseTime_;
	bool rotateScreen_;
	double rotateDegree_;
};

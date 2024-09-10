#pragma once

#include "engine/work.hpp"
#include "Field.hpp"
#include "gametype.hpp"

class ReplayRecorder;

class Game : public Work, public std::enable_shared_from_this<Game> {
public:
	Game(GameType, int seed, bool replay);
	virtual ~Game();
	virtual void step();
	virtual void draw() const;
	void onQuitEvent();
	Field& GetField();
	double GetTime() const;
	bool GameOverAnimationFinished() const;
	GameType getType() const;
	void DrawTime(int x, int y) const;
	void StepToRotateScreen();
	void SetRotateScreen(bool);
	bool gameOver() const;
	bool isReplay() const;

protected:
	void onLoad() override;

	Field field_;
	GameType type_;
	double nextPosition_;
	double oldNextPosition_;
	double pauseTime_ = 0;
	bool rotateScreen_;
	double rotateDegree_;

	/// If we're owned by a ReplayPlayer, this is nullptr
	std::unique_ptr<ReplayRecorder> replayRecorder;
};

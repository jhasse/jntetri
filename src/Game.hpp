#pragma once

#include "engine/work.hpp"
#include "Field.hpp"
#include "gametype.hpp"

class ReplayRecorder;

class Game : public Work, public std::enable_shared_from_this<Game> {
public:
	Game(GameType, int seed, bool replay);
	~Game() override;
	void step() override;
	void draw() const override;
	Field& GetField();
	double GetTime() const;
	bool GameOverAnimationFinished() const;
	GameType getType() const;
	void DrawTime(int x, int y) const;
	void StepToRotateScreen();
	void setRotateScreen(bool);
	bool gameOver() const;
	bool isReplay() const;

protected:
	void onQuitEvent() override;
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

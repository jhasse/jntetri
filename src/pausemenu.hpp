#pragma once

#include "engine/work.hpp"

class ButtonBox;
class Game;

class PauseMenu : public Work {
public:
	PauseMenu(std::shared_ptr<Game>);
	virtual void step();
	virtual void draw() const;
	void Continue();
	void QuitToMenu() const;
	void onQuitEvent();

private:
	std::shared_ptr<jngl::Work> work;
	std::shared_ptr<ButtonBox> buttonBox_;
	std::shared_ptr<Game> game;
};

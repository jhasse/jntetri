#pragma once

#include "engine/work.hpp"

class ButtonBox;
class Game;

class PauseMenu : public Work {
public:
	explicit PauseMenu(std::shared_ptr<Game>);
	void Continue();
	void QuitToMenu() const;

private:
	void step() override;
	void draw() const override;
	void onQuitEvent() override;

	std::shared_ptr<jngl::Work> work;
	std::shared_ptr<ButtonBox> buttonBox;
	std::shared_ptr<Game> game;
};

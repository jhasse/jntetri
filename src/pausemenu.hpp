#pragma once

#include "game.hpp"
#include "gui/buttonbox.hpp"

class PauseMenu : public Work {
public:
    PauseMenu(std::shared_ptr<jngl::Work>);
    virtual void step();
    virtual void draw() const;
	void Continue();
	void QuitToMenu() const;
	void onQuitEvent();
private:
    std::shared_ptr<Game> work_;
    std::shared_ptr<ButtonBox> buttonBox_;
};

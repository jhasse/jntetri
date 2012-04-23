#pragma once

#include "game.hpp"
#include "gui/buttonbox.hpp"

#include <boost/shared_ptr.hpp>

class PauseMenu : public Work {
public:
    PauseMenu(boost::shared_ptr<jngl::Work>);
    virtual void step();
    virtual void draw() const;
	void Continue();
	void QuitToMenu() const;
	void onQuitEvent();
private:
    boost::shared_ptr<Game> work_;
    boost::shared_ptr<ButtonBox> buttonBox_;
};

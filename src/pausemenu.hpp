#pragma once

#include "engine/work.hpp"
#include "gui/buttonbox.hpp"

#include <boost/shared_ptr.hpp>

class PauseMenu : public Work {
public:
    PauseMenu(boost::shared_ptr<Work>);
    virtual void Step();
    virtual void Draw() const;
	void Continue();
	void QuitToMenu() const;
	void QuitEvent();
private:
    boost::shared_ptr<Work> work_;
    ButtonBox buttonBox_;
};

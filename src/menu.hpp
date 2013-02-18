#pragma once

#include "engine/work.hpp"
#include "gui/buttonbox.hpp"
#include "highscore.hpp"

#include <vector>

class Menu : public Work {
public:
    Menu();
    virtual void step();
    virtual void draw() const;
    void BlinkHighscore(Data);

    void Normal() const;
    void FiftyLines() const;
    void Multiplayer() const;
    void OptionsMenuCallback() const;
    void QuitGame() const;
private:
    std::shared_ptr<ButtonBox> buttonBox_;
    Highscore normalHighscore_;
    Highscore fiftyLinesHighscore_;
};

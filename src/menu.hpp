#pragma once

#include "engine/work.hpp"
#include "gui/buttonbox.hpp"
#include "highscore.hpp"

#include <vector>

class Menu : public Work {
public:
    Menu();
    virtual void Step();
    virtual void Draw() const;
    void BlinkHighscore(Data);

    void Normal() const;
    void FiftyLines() const;
    void Multiplayer() const;
    void OptionsMenuCallback() const;
    void QuitGame() const;
private:
    ButtonBox buttonBox_;
    Highscore normalHighscore_;
    Highscore fiftyLinesHighscore_;
};

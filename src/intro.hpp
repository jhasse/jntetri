#pragma once

#include "engine/work.hpp"
#include "engine/resizegraphics.hpp"

#include <deque>
#include <string>

class Intro : public Work {
public:
    Intro();
    ~Intro();
    virtual void step();
    virtual void draw() const;
private:
    int blink_;
    mutable bool finished_;
    mutable ResizeGraphics resizeGraphics_;
};

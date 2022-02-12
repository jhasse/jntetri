#pragma once

#include <jngl/job.hpp>
#include <jngl/text.hpp>

class DebugOverlay : public jngl::Job {
public:
	DebugOverlay();
	void step() override;
	void draw() const override;
private:
	bool visible = false;
	mutable jngl::Text fps;
	mutable double lastDraw = 0;
};

#include "Intro.hpp"
#include "engine/debugoverlay.hpp"
#include "constants.hpp"

#include <jngl.hpp>
#include <jngl/init.hpp>

jngl::AppParameters jnglInit() {
	jngl::AppParameters params;
	params.displayName = programDisplayName;
	params.screenSize = { 1280, 720 };
	params.start = []() {
		jngl::setAntiAliasing(true);
		jngl::setIcon("icon");
		jngl::setFont("Inter-ExtraLight.ttf");
		jngl::setFontSize(jngl::getScaleFactor() * 13);
		jngl::addJob(std::make_shared<DebugOverlay>());
		return std::make_shared<Intro>();
	};
	return params;
}

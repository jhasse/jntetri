#include "engine/procedure.hpp"
#include "engine/options.hpp"
#include "Intro.hpp"
#include "engine/debugoverlay.hpp"
#include "constants.hpp"

#include <spdlog/spdlog.h>
#include <jngl.hpp>
#include <jngl/init.hpp>

jngl::AppParameters jnglInit() {
	jngl::AppParameters params;
#ifndef NDEBUG
	spdlog::set_level(spdlog::level::trace);
#endif
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

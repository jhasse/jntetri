#include "engine/procedure.hpp"
#include "engine/options.hpp"
#include "Intro.hpp"
#include "engine/debugoverlay.hpp"
#include "constants.hpp"

#include <spdlog/spdlog.h>
#include <jngl/all.hpp>
#include <jngl/init.hpp>

std::function<std::shared_ptr<jngl::Work>()> jnglInit(jngl::AppParameters& params) {
#ifndef NDEBUG
	spdlog::set_level(spdlog::level::debug);
#endif
	params.displayName = programDisplayName;
	return []() {
		jngl::setAntiAliasing(true);
		jngl::setIcon("share/icons/jntetri.png");
		jngl::setFontSize(jngl::getScaleFactor() * 13);
		jngl::addJob(std::make_shared<DebugOverlay>());
		return std::make_shared<Intro>();
	};
}

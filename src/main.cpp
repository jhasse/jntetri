#include "engine/procedure.hpp"
#include "engine/options.hpp"
#include "Intro.hpp"
#include "engine/debugoverlay.hpp"

#include <spdlog/spdlog.h>
#include <jngl/all.hpp>

JNGL_MAIN_BEGIN {
#ifdef NDEBUG
	try {
#else
	spdlog::set_level(spdlog::level::debug);
#endif
		jngl::App app("JNTETRI");
		if (!GetProcedure().ShowWindow() && getOptions().fullscreen) {
			// Try again without fullscreen
			getOptions().fullscreen = false;
			GetProcedure().ShowWindow();
		}
		jngl::setAntiAliasing(true);
		jngl::setIcon("share/icons/jntetri.png");
		jngl::setWork(std::make_shared<Intro>());
		jngl::setFontSize(jngl::getScaleFactor() * 13);
		jngl::addJob(std::make_shared<DebugOverlay>());
		app.mainLoop();
		getOptions().Save();
#ifdef NDEBUG
	} catch(std::exception& e) {
		jngl::hideWindow();
		jngl::errorMessage(e.what());
	}
#endif
} JNGL_MAIN_END

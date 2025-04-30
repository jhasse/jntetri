#include "Intro.hpp"
#include "ReplayPlayer.hpp"
#include "constants.hpp"
#include "engine/debugoverlay.hpp"

#include <jngl.hpp>
#include <jngl/init.hpp>

jngl::AppParameters jnglInit() {
	jngl::AppParameters params;
	params.displayName = programDisplayName;
	params.screenSize = { 1200. / 9. * 16., 1200 };
	params.start = []() -> std::shared_ptr<jngl::Work> {
		jngl::setAntiAliasing(true);
		jngl::setIcon("icon");
		jngl::setFont("Inter-ExtraLight.ttf");
		jngl::addJob(std::make_shared<DebugOverlay>());
		auto args = jngl::getArgs();
		if (!args.empty()) {
			if (args[0] == "--replay") {
				return std::make_shared<ReplayPlayer>(args[1]);
			}
		}
		return std::make_shared<Intro>();
	};
	return params;
}

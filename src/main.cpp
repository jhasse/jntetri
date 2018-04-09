#include "engine/procedure.hpp"
#include "engine/options.hpp"
#include "engine/paths.hpp"
#include "intro.hpp"
#include "engine/debugoverlay.hpp"

#include <jngl/all.hpp>

int main(int argc, char** argv) {
#ifdef NDEBUG
	try {
#endif
		if (argc == 2) {
			getPaths().setPrefix(argv[1]);
		}
		if (!GetProcedure().ShowWindow() && GetOptions().fullscreen) {
			// Try again without fullscreen
			GetOptions().fullscreen = false;
			GetProcedure().ShowWindow();
		}
		jngl::setAntiAliasing(true);
		jngl::setIcon(getPaths().getPrefix() + "share/icons/jntetri.png");
		jngl::setWork(new Intro);
		jngl::addJob(std::make_shared<DebugOverlay>());
		jngl::mainLoop();
		GetOptions().Save();
#ifdef NDEBUG
	} catch(std::exception& e) {
		jngl::hideWindow();
		jngl::errorMessage(e.what());
	}
#endif
}

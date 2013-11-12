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
		const int dpiFactor = std::max{std::min{
			jngl::getDesktopWidth() / 1280,
			jngl::getDesktopHeight() / 720
		}, 1};
		GetOptions().SetFallback("windowWidth", 1280 * dpiFactor);
		GetOptions().SetFallback("windowHeight", 720 * dpiFactor);
		GetOptions().SetFallback("fullscreen", 0);
		GetOptions().SetFallback("startJunks", 0);
		GetOptions().SetFallback("startLevel", 0);
		GetOptions().SetFallback("lastHighscoreName", "");
		GetOptions().SetFallback("lastLoginName", "");
		if (!GetProcedure().ShowWindow() && GetOptions().Get<int>("fullscreen")) {
			// Try again without fullscreen
			GetOptions().Set("fullscreen", 0);
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

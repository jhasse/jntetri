#include "engine/procedure.hpp"
#include "engine/options.hpp"
#include "engine/paths.hpp"
#include "intro.hpp"

#include <jngl/all.hpp>

int main(int argc, char** argv)
{
#ifdef NDEBUG
	try
	{
#endif
		if (argc == 2) {
			GetPaths().SetPrefix(argv[1]);
		}
		GetOptions().SetFallback("windowWidth", jngl::getDesktopWidth());
		GetOptions().SetFallback("windowHeight", jngl::getDesktopHeight());
		GetOptions().SetFallback("fullscreen", 1);
		GetOptions().SetFallback("startJunks", 0);
		GetOptions().SetFallback("startLevel", 0);
		GetOptions().SetFallback("lastHighscoreName", "");
		GetOptions().SetFallback("lastLoginName", "");
		if(!GetProcedure().ShowWindow() && GetOptions().Get<int>("fullscreen"))
		{
			// Try again without fullscreen
			GetOptions().Set("fullscreen", 0);
			GetProcedure().ShowWindow();
		}
		jngl::setAntiAliasing(true);
		jngl::setIcon(GetPaths().Prefix() + "share/icons/jntetri.png");
		jngl::setWork(new Intro);
		jngl::mainLoop();
		GetOptions().Save();
#ifdef NDEBUG
	}
	catch(std::exception& e)
	{
		jngl::errorMessage(e.what());
	}
#endif
}

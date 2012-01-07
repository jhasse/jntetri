#include "engine/procedure.hpp"
#include "engine/options.hpp"
#include "engine/paths.hpp"
#include "intro.hpp"

#include <jngl.hpp>

int main()
{
	try
	{
		system("echo $PWD");
		GetOptions().SetFallback("windowWidth", jngl::GetDesktopWidth());
		GetOptions().SetFallback("windowHeight", jngl::GetDesktopHeight());
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
		jngl::SetAntiAliasing(true);
		jngl::SetIcon(GetPaths().Prefix() + "share/icons/jntetri.png");
		GetProcedure().SetWork(new Intro);
		GetProcedure().MainLoop();
		GetOptions().Save();
	}
	catch(std::exception& e)
	{
		jngl::ErrorMessage(e.what());
	}
}

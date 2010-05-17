#include "engine/procedure.hpp"
#include "engine/options.hpp"
#include "engine/paths.hpp"
#include "intro.hpp"

#include <jngl.hpp>

int main()
{
	try
	{
		if(!GetProcedure().ShowWindow() && GetOptions().GetFullscreen())
		{
			// Try again without fullscreen
			GetOptions().SetFullscreen(false);
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

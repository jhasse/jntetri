#include "engine/procedure.hpp"
#include "engine/options.hpp"
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
		GetProcedure().SetWork(new Intro);
		GetProcedure().MainLoop();
		GetOptions().Save();
	}
	catch(std::exception& e)
	{
		jngl::ErrorMessage(e.what());
	}
}

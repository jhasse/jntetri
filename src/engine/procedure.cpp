#include "procedure.hpp"
#include "options.hpp"
#include "screen.hpp"
#include "../constants.hpp"
#include "../intro.hpp"

#include <jngl.hpp>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

Procedure::Procedure() : oldTime_(jngl::getTime()), needDraw_(true), fps_(0), fpsTemp_(0), changeWork_(false), running_(true), showFps_(false)
{
}

Procedure& GetProcedure()
{
	return Procedure::Handle();
}

bool Procedure::ShowWindow()
{
	const Options& options = GetOptions();
	try
	{
		jngl::hideWindow();
		jngl::showWindow(programDisplayName + " " + programVersion,
						 options.Get<int>("windowWidth"),
						 options.Get<int>("windowHeight"),
						 options.Get<int>("fullscreen") != 0);
	}
	catch(std::runtime_error& err)
	{
		jngl::errorMessage(err.what());
		return false;
	}
	return true;
}

const double Procedure::timePerStep_ = 1.0/100.0;

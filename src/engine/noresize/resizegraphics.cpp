#include "../resizegraphics.hpp"
#include "../paths.hpp"
#include "../options.hpp"
#include "../screen.hpp"

#include <jngl.hpp>
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

ResizeGraphics::ResizeGraphics() : originalSize_(240)
{
}

bool ResizeGraphics::Finished()
{
	GetPaths().SetGraphics(GetPaths().Data() + "gfx_wiz/x" + boost::lexical_cast<std::string>(originalSize_) + "/");
	return true;
}

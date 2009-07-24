#include "work.hpp"
#include "procedure.hpp"

#include <jngl.hpp>

void Work::QuitEvent()
{
    GetProcedure().Quit();
}

Work::~Work()
{
}

void Work::StepWidgets()
{
	std::vector<boost::shared_ptr<Widget> >::iterator end = widgets_.end();
	for(std::vector<boost::shared_ptr<Widget> >::iterator it = widgets_.begin(); it != end; ++it)
	{
		(*it)->Step();
	}
}

void Work::DrawWidgets() const
{
	std::vector<boost::shared_ptr<Widget> >::const_iterator end = widgets_.end();
	for(std::vector<boost::shared_ptr<Widget> >::const_iterator it = widgets_.begin(); it != end; ++it)
	{
		(*it)->Draw();
	}
}

void Work::AddWidget(boost::shared_ptr<Widget> widget)
{
	widgets_.push_back(widget);
}

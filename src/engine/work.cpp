#include "work.hpp"
#include "procedure.hpp"
#include "../gui/widget.hpp"

#include <jngl.hpp>
#include <algorithm>

void Work::QuitEvent()
{
    GetProcedure().Quit();
}

Work::~Work()
{
}

void Work::FocusNext()
{
	focusedWidget_->SetFocus(false);
	if(focusedWidget_ == widgets_.back())
	{
		focusedWidget_ = widgets_.front();
	}
	else
	{
		std::vector<boost::shared_ptr<Widget> >::iterator it = std::find(widgets_.begin(), widgets_.end(), focusedWidget_);
		++it;
		focusedWidget_ = *it;
	}
}

void Work::StepWidgets()
{
	focusedWidget_->SetFocus(true);
	std::vector<boost::shared_ptr<Widget> >::iterator end = widgets_.end();
	for(std::vector<boost::shared_ptr<Widget> >::iterator it = widgets_.begin(); it != end; ++it)
	{
		(*it)->Step();
	}
	if(jngl::KeyPressed(jngl::key::Tab))
	{
		FocusNext();
	}
	if(!focusedWidget_->GetSensitive())
	{
		FocusNext();
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
	if(!focusedWidget_)
	{
		focusedWidget_ = widget;
	}
	widget->OnAdd(*this);
}

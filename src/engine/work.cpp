#include "work.hpp"
#include "procedure.hpp"
#include "../gui/widget.hpp"

#include <algorithm>
#include <jngl/input.hpp>

Work::Work()
{
}

Work::~Work()
{
}

void Work::FocusPrevious()
{
	focusedWidget_->SetFocus(false);
	if(focusedWidget_ == widgets_.front())
	{
		focusedWidget_ = widgets_.back();
	}
	else
	{
		std::vector<boost::shared_ptr<Widget> >::iterator it = std::find(widgets_.begin(), widgets_.end(), focusedWidget_);
		--it;
		focusedWidget_ = *it;
	}
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

void Work::StepFocus()
{
	if(jngl::keyPressed(jngl::key::Down) || jngl::keyPressed(jngl::key::WizDown) || jngl::keyPressed(jngl::key::Tab))
	{
		FocusNext();
	}
	if(!focusedWidget_->GetSensitive())
	{
		FocusNext();
	}
	if(jngl::keyPressed(jngl::key::Up) || jngl::keyPressed(jngl::key::WizUp))
	{
		FocusPrevious();
		while(!focusedWidget_->GetSensitive())
		{
			FocusPrevious();
		}
	}
	focusedWidget_->SetFocus(true);
}

void Work::StepWidgets()
{
	std::vector<boost::shared_ptr<Widget> >::iterator end = widgets_.end();
	for(std::vector<boost::shared_ptr<Widget> >::iterator it = widgets_.begin(); it != end; ++it)
	{
		(*it)->Step();
	}
	StepFocus();
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
	StepFocus();
}

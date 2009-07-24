#pragma once

#include "../gui/widget.hpp"

#include <boost/shared_ptr.hpp>
#include <vector>

class Work
{
public:
	virtual void Step() = 0;
	virtual void Draw() const = 0;
	virtual void QuitEvent();
	virtual ~Work();
	void StepWidgets();
	void DrawWidgets() const;
	void AddWidget(boost::shared_ptr<Widget>);
protected:
	std::vector<boost::shared_ptr<Widget> > widgets_;
};
